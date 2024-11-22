%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#define TABLE_SIZE 100 
typedef struct node Node;
typedef struct list List; 
int yylex();
void yyerror(const char *s);
extern FILE *yyin;
int error_flag = 0;
int stopping = 1;
int temp_var_count = 0; // Counter for temporary variables
int tac_count = 0;      // Counter for TAC lines
char tac[10000][10000];     // Assuming we won't exceed 100 TAC lines, and each TAC line is at most 100 chars
int offset = 0 ;
char var_list[1000][1000];      
int var_count = 0;



typedef struct HashNode {
    char* name;        // Symbol name
    char* type;        // Type of the symbol (e.g., int, float, etc.)
    int offset;        // Offset for the symbol
    struct HashNode* next; // Pointer to the next node (for collision resolution)
} HashNode;
typedef struct SymbolTable {
    HashNode** table;  // Pointer to an array of hash nodes
} SymbolTable;
typedef struct Type {
    char type[1000]; // Name of the type
    int width;     // Width of the type
} Type;
// Function to create a new Type instance
Type* createType(char* value, int width) {
    Type* n = (Type*)malloc(sizeof(Type)); // Allocate memory for the Type
    if (n == NULL) { // Check if malloc was successful
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    
    // Copy the value into the type field
    strncpy(n->type, value, sizeof(n->type) - 1); // Use strncpy to prevent buffer overflow
    n->type[sizeof(n->type) - 1] = '\0'; // Ensure null termination
    n->width = width; // Set the width

    return n; // Return the newly created Type instance
}
// Function to create a new HashNode
HashNode* createHashNode(const char* name, const char* type, int offset) {
    HashNode* newNode = (HashNode*)malloc(sizeof(HashNode));
    newNode->name = strdup(name); // Duplicate the string for the name
    newNode->type = strdup(type); // Duplicate the string for the type
    newNode->offset = offset;
    newNode->next = NULL; // Initialize next to NULL
    return newNode;
}
// Function to create a symbol table
SymbolTable* createSymbolTable() {
    SymbolTable* symbolTable = (SymbolTable*)malloc(sizeof(SymbolTable));
    symbolTable->table = (HashNode*)malloc(TABLE_SIZE * sizeof(HashNode));

    for (int i = 0; i < TABLE_SIZE; i++) {
        symbolTable->table[i] = NULL; // Initialize all table entries to NULL
    }

    return symbolTable;
}
// Hash function to compute the index for a given name
unsigned int hash(const char* name) {
    unsigned int hashValue = 0;
    while (*name) {
        hashValue = (hashValue << 5) + *name++; // Hashing algorithm
    }
    return hashValue % TABLE_SIZE; // Return index within the table size
}
// Function to insert a new symbol into the symbol table
// Function to lookup a symbol by name, returns the symbol if found, NULL if not found
HashNode* lookupSymbol(SymbolTable* symbolTable, const char* name) {
    if(symbolTable == NULL)
    {
        return NULL;
    }
    unsigned int index = hash(name);
    //printf("%d\n",index);
    HashNode* current = symbolTable->table[index];

    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current; // Symbol found
        }
        current = current->next; // Move to the next node
    }
    return NULL; // Symbol not found
}

// Function to insert a new symbol into the symbol table
// Only inserts if the symbol does not already exist
int insertSymbol(SymbolTable* symbolTable, const char* name, const char* type, int offset) {
    // Look up the symbol to check if it already exists
    HashNode* existingSymbol = lookupSymbol(symbolTable, name);

    if (existingSymbol) {
        // Symbol exists, check if the type matches
        if (strcmp(existingSymbol->type, type) == 0) {
            // Same type, so it's just a redeclaration
            return 1; // Redeclared with the same type
        } else {
            // Different type, so it's a conflicting declaration
            return 2; // Conflicting types
        }
    }

    // Symbol does not exist, insert it
    unsigned int index = hash(name);
    HashNode* newNode = createHashNode(name, type, offset);

    // Collision resolution: add the new node at the beginning of the linked list
    newNode->next = symbolTable->table[index];
    symbolTable->table[index] = newNode;

    return 0; // Successfully inserted the symbol
}
// Function to delete a symbol from the symbol table
void deleteSymbol(SymbolTable* symbolTable, const char* name) {
    unsigned int index = hash(name);
    HashNode* current = symbolTable->table[index];
    HashNode* previous = NULL;

    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            if (previous == NULL) {
                // Remove the head of the list
                symbolTable->table[index] = current->next;
            } else {
                // Remove the node from the middle or end
                previous->next = current->next;
            }
            free(current->name);
            free(current->type);
            free(current);
            return; // Symbol deleted
        }
        previous = current;
        current = current->next;
    }
}
// Function to free the symbol table
void freeSymbolTable(SymbolTable* symbolTable) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode* current = symbolTable->table[i];
        while (current != NULL) {
            HashNode* temp = current;
            current = current->next;
            free(temp->name);
            free(temp->type);
            free(temp);
        }
    }
    free(symbolTable->table);
    free(symbolTable);
}
// Define a node in the ScopeStack linked list
typedef struct ScopeStackNode {
    SymbolTable* table;
    struct ScopeStackNode* next;
} ScopeStackNode;
// Define the ScopeStack structure itself
typedef struct ScopeStack {
    ScopeStackNode* top; // Pointer to the top node
} ScopeStack;
// Function to create a new ScopeStack
ScopeStack* createScopeStack() {
    ScopeStack* stack = (ScopeStack*)malloc(sizeof(ScopeStack));
    stack->top = NULL; // Initialize top as NULL (empty stack)
    return stack;
}
// Function to check if the ScopeStack is empty
int isScopeStackEmpty(ScopeStack* stack) {
    return stack->top == NULL;
}
// Function to push a new symbol table onto the ScopeStack
void pushScope(ScopeStack* stack, SymbolTable* symbolTable) {
    ScopeStackNode* node = (ScopeStackNode*)malloc(sizeof(ScopeStackNode));
    node->table = symbolTable;
    node->next = stack->top; // Link new node to the current top
    stack->top = node;       // Update top to the new node
}
// Function to pop a symbol table from the ScopeStack
SymbolTable* popScope(ScopeStack* stack) {
    if (isScopeStackEmpty(stack)) {
        printf("ScopeStack underflow\n");
        return NULL; // Return NULL if the stack is empty
    }
    ScopeStackNode* temp = stack->top;
    SymbolTable* poppedTable = temp->table;
    stack->top = stack->top->next; // Move top to the next node
   // free(temp);
    return poppedTable;
}
// Function to peek at the top symbol table of the ScopeStack
SymbolTable* peekScope(ScopeStack* stack) {
    if (isScopeStackEmpty(stack)) {
        printf("ScopeStack is empty\n");
        return NULL; // Return NULL if the stack is empty
    }
    return stack->top->table;
}
// Function to free the ScopeStack
void freeScopeStack(ScopeStack* stack) {
    while (!isScopeStackEmpty(stack)) {
        popScope(stack); // Free each symbol table in the stack
    }
    free(stack);
}
typedef struct OffsetStackNode {
    int offset;
    struct OffsetStackNode* next;
} OffsetStackNode;
typedef struct OffsetStack {
    OffsetStackNode* top;
} OffsetStack;
// Function to create a new stack node
OffsetStackNode* createOffsetStackNode(int offset) {
    OffsetStackNode* node = (OffsetStackNode*)malloc(sizeof(OffsetStackNode));
    if (!node) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    node->offset = offset;
    node->next = NULL;
    return node;
}
// Function to initialize the stack
OffsetStack* createOffsetStack() {
    OffsetStack* stack = (OffsetStack*)malloc(sizeof(OffsetStack));
    if (!stack) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    stack->top = NULL;
    return stack;
}
// Function to check if the stack is empty
bool isOffsetStackEmpty(OffsetStack* stack) {
    return stack->top == NULL;
}
// Function to push an offset onto the stack
void pushOffset(OffsetStack* stack, int offset) {
    OffsetStackNode* node = createOffsetStackNode(offset);
    node->next = stack->top;
    stack->top = node;
}
// Function to pop the top offset from the stack
int popOffset(OffsetStack* stack) {
    if (isOffsetStackEmpty(stack)) {
        printf("Stack underflow\n");
        exit(EXIT_FAILURE);
    }
    OffsetStackNode* temp = stack->top;
    int poppedOffset = temp->offset;
    stack->top = stack->top->next;
    // free(temp);
    return poppedOffset;
}
// Function to get the top offset without popping
int peekOffset(OffsetStack* stack) {
    if (isOffsetStackEmpty(stack)) {
        printf("Stack is empty\n");
        exit(EXIT_FAILURE);
    }
    return stack->top->offset;
}
// Function to free the entire stack
void freeOffsetStack(OffsetStack* stack) {
    while (!isOffsetStackEmpty(stack)) {
        popOffset(stack);
    }
    free(stack);
}
// Function to generate a new temporary variable
char* new_temp() {
    static char temp[10];
    sprintf(temp, "t%d", temp_var_count++);
    return strdup(temp); // Return the temporary variable name
}
void add_tac(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vsprintf(tac[tac_count++], format, args);
    va_end(args);
}
struct node {
 char value[100]; // typecasting
 List* trueList;
 bool notassign;
 List* falseList;
 List* nextList;
};
struct list {
 int addr;
 List* next;
};
char* gen_label() {
 char* label = (char*)malloc(sizeof(char) * 100);
 if (label == NULL) {
 perror("Memory allocation failed");
 exit(EXIT_FAILURE);
 }
 sprintf(label, "t%d", temp_var_count);
 temp_var_count++;
 return label;
}
Node* createNode(char* value, List* trueList, List* falseList, List* nextList){
 Node* n = (Node*)malloc(sizeof(Node));
 strcpy(n->value, value);
 n->nextList = nextList;
 n->trueList = trueList;
 n->falseList = falseList;
 return n;
}
List* makelist(int addr){
 List* list = (List*)malloc(sizeof(List));
 list->addr = addr;
 list->next = NULL;
}
List* merge(List* l1, List* l2) {
 if (l1 == NULL) return l2;
 if (l2 == NULL) return l1;

 List* curr = l1;
 while (curr->next != NULL) {
 curr = curr->next;
 }
 curr->next = l2;
 return l1;
}
void backpatch(List* list, int addr){
 List* curr = list;
 while(curr != NULL){
 sprintf(tac[curr->addr] + strlen(tac[curr->addr]),"%d\n",addr);
 curr = curr->next;
 }
}
void printList(List* list, const char* label) {
 printf("%s: ", label);
 List* curr = list;
 while (curr != NULL) {
 printf("%d -> ", curr->addr);
 curr = curr->next;
 }
 printf("NULL\n");
}
int compareOffsets(const void* a, const void* b) {
    HashNode* nodeA = *(HashNode**)a;
    HashNode* nodeB = *(HashNode**)b;
    return nodeA->offset - nodeB->offset;
}
SymbolTable *currentSymbolTable;
ScopeStack *scopeStack;
OffsetStack *offsetStack;
SymbolTable* symbolTableArray[1000];  // Array to hold up to 1000 symbol table addresses
int symbolTableCount = 0;  
Type* dummy;

int checkSymbolExistsInAnyTable(const char* name) {
    for (int i = 0; i <= symbolTableCount; i++) {
        if (lookupSymbol(symbolTableArray[i], name) != NULL) {
            return 1;  // Symbol found in this table
        }
    }
    return 0;  // Symbol not found in any table
}



void printSymbolTables() {
    
    for (int i = 0 ; i <symbolTableCount; i++) {
        
        SymbolTable* table = symbolTableArray[i];
        printf("Symbol Table %d:\n", i+1);
        printf("%-10s %-10s %-15s\n", "Name", "Type", "Offset (Hex)");
        printf("----------------------------------------\n");

        // Step 1: Collect all entries in an array for sorting
        HashNode* entries[100];  // Adjust size as needed
        int count = 0;

        for (int j = 0; j < TABLE_SIZE; j++) {
            HashNode* current = table->table[j];
            while (current != NULL) {
                entries[count++] = current;
                current = current->next;
            }
        }

        // Step 2: Sort entries by offset
        qsort(entries, count, sizeof(HashNode*), compareOffsets);

        // Step 3: Print sorted entries
        for (int k = 0; k < count; k++) {
            printf("%-10s %-10s 0x%04x\n", entries[k]->name, entries[k]->type, entries[k]->offset & 0xFFFF);
        }
        if(count==0 && i==0)
        {
            printf("Nothing to store in global scope\n");
        }
        else if(count==0)
        {
            printf("Nothing to store in this scope\n");
        }

        printf("----------------------------------------\n\n");
    }
}
// Function to print the contents of a symbol table
void printSymbolTable(SymbolTable* symbolTable) {
    if (symbolTable == NULL) {
        printf("Symbol table is empty.\n");
        return;
    }
    
    printf("Symbol Table:\n");
    printf("%-10s %-10s %-10s\n", "Name", "Type", "Offset");
    printf("----------------------------\n");
    
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode* current = symbolTable->table[i];
        
        // Iterate over the linked list at this index
        while (current != NULL) {
            printf("%-10s %-10s %-10d\n", current->name, current->type, current->offset);
            current = current->next;
        }
    }
    printf("----------------------------\n");
}

%}

%union {
    char* value;   // For storing string values like identifiers and numbers
     struct node* node;
     int instr;
     struct list* list;
     struct Type* type;
     
}

%token <value> IDENTIFIER NUMBER EQ PEQ MEQ MLEQ DEQ MM PP LB RB GT LT GTE LTE NE EE AND OR IF ELSE T F WHILE NN INT FLOAT CHAR LS RS
%left GTE LTE GT LT
%left '+' '-'
%left '*' '/' '%'
%nonassoc PP MM
%nonassoc '(' ')'
%left OR
%left AND
%nonassoc NE EE
%nonassoc NN
%nonassoc LB RB
%type <instr> M N 
%type <node> stmt_list stmt expr term  UM DOUBLE ASSIGN ASSIGN_STMT CONTROL_FLOW_STMT  
%type <type> B comp
%nonassoc ELSE
%%

program: O P stmt_list{
        if(!error_flag)
     {  backpatch($3->nextList,tac_count);
        printf("----------Intermediate Code----------\n");
        for (int i = 0; i < tac_count; i++) {
            printf("%d: %s", i, tac[i]);
        }
        
        printf("----------Symbol Tables-----------\n");
        printSymbolTables();
        // SymbolTable *symtab = peekScope(scopeStack);
        temp_var_count = 0;
        tac_count = 0; // Reset intermediate code index
    }
   
    }
       ;

stmt_list: 
          stmt_list M stmt{
            if(!error_flag)
            {

            // printList($1->nextList, "$1->nextList before backpatch");
            backpatch($1->nextList,$2);
          //  printSymbolTables();
            Node* temp = createNode("",NULL,NULL,$3->nextList);
            $$ = temp;
            }
        
        }
          | stmt { 
        if(!error_flag){
            Node* temp = createNode("",NULL,NULL,$1->nextList);
            $$ = temp;

        } 
    }
        ;

stmt:  
   DECLARE_STMT ';' {
    if(!error_flag)
    {
        Node* temp = createNode("",NULL,NULL,NULL);
                        $$ = temp;
    }
                          
  }
  | ASSIGN_STMT ';' {        if(!error_flag)
  {  
                           $$ = $1;  
  }    
                          } 
                 
  | CONTROL_FLOW_STMT { if(!error_flag) {$$ = $1;} } 
  | LB Q stmt_list RB  {  if(!error_flag)
                        {
                        //printf("hi\n");
                        // symbolTableArray[symbolTableCount] = currentSymbolTable;
                        //   //  printSymbolTable(currentSymbolTable);
                        // symbolTableCount++;
                        currentSymbolTable = popScope(scopeStack);
                        offset = popOffset(offsetStack);

                        Node* temp = createNode("",NULL,NULL,$3->nextList);
                        $$ = temp;
                        }
                      }
  | LB RB { 
        if(!error_flag){
            Node* temp = createNode("",NULL,NULL,NULL);
            $$ = temp;
        }
    }
  | expr ';' {if(!error_flag) {$$=$1;}}
  | error { if(!error_flag) {printf("Rejected - Invalid Expression\n");error_flag=1;yyerrok;}}
  ;


DECLARE_STMT : B A comp { 
                   for(int i =0;i<var_count;i++)
                   {
                      
                       if(insertSymbol(currentSymbolTable,var_list[i], $3->type , offset) == 1) 
                       {
                        if(!error_flag)
                        {
                        error_flag = 1;
                        printf("error : redeclaration of '%s'\n",var_list[i]);
                        }
                       }
                       else if(insertSymbol(currentSymbolTable,var_list[i], $3->type , offset) == 2)
                       {
                         error_flag = 1;
                        printf("error : conflicting types for '%s'\n",var_list[i]);
                       }
                      // printSymbolTable(currentSymbolTable);
                       offset= offset + $3->width;
                      // printf("%d",offset);
                      // printf("%s ",var_list[i] );
                       strcpy(var_list[i],"");
                       
                   }
                   var_count =0;
  dummy = createType("",-1);
 };
comp:
      LS NUMBER RS comp {
        if(!error_flag) {
            int num = atoi($2);
            int width = num*$4->width;
           // printf("%d",width);
            char value[1000]; sprintf(value,"array(%d,%s)",num,$4->type);
            $$ = createType(value,width);
        }
    }
    | { $$ = createType(dummy->type,dummy->width); }
    ;
A : A ',' IDENTIFIER ASSIGN expr { sprintf(var_list[var_count],"%s",$3); var_count++; }
    |A ',' IDENTIFIER { sprintf(var_list[var_count],"%s",$3); var_count++; }
    | IDENTIFIER { sprintf(var_list[var_count],"%s",$1); var_count++; }
    | IDENTIFIER ASSIGN expr { sprintf(var_list[var_count],"%s",$1); var_count++; };
CONTROL_FLOW_STMT : IF '(' expr ')' M stmt   {
                                  if(!error_flag)
                               {        backpatch($3->trueList,$5);
                                  Node* temp = createNode("",NULL,NULL,merge($3->falseList,$6->nextList));
                                  $$ = temp;
                                  }
                               }    
  | IF '(' expr ')' M stmt ELSE N M stmt 
                                      {
                                       if(!error_flag)
                                      {
                                       backpatch($3->trueList,$5);
                                       backpatch($3->falseList,$9);
                                       Node* _temp = createNode("",NULL,NULL,merge($6->nextList,makelist($8)));
                                       Node* temp = createNode("",NULL,NULL,merge(_temp->nextList,$10->nextList));
                                       $$ = temp;
                                       }
                                      }
  | WHILE M '(' expr ')' M stmt {
                                  if(!error_flag)
                              {
                               backpatch($7->nextList,$2);
                               backpatch($4->trueList,$6);
                               Node* temp = createNode("",NULL,NULL,$4->falseList);
                               $$ = temp;
                               add_tac("goto %d\n",$2);
                               }
                               }
  | IF '(' ')' {if(!error_flag){
            printf("Rejected - empty bool expr inside if\n");
            error_flag = 1;
        }}
  ;
  
  
ASSIGN_STMT : expr ASSIGN expr  { if(!error_flag)
    				  {
                                      if($1->notassign)
                                      {
                                        printf("Rejected-cannot assign to a postfix expression\n");
                                        error_flag = 1;
                                      }    
                                      else
                                      {
                                      if(strcmp($2->value, "=") == 0)
                                 {
                                    add_tac("%s = %s\n", $1, $3->value);
                                 }
                                 else if(strcmp($2->value, "+=") == 0){
                                    char *new = new_temp();
                                    add_tac("%s = %s\n", new, $3->value);
                                    char *new1 = new_temp();
                                    add_tac("%s = %s + %s\n", new1, $1,new);
                                    add_tac("%s = %s\n",$1, new1);
                                 }
                                 else if(strcmp($2->value, "-=") == 0){
                                    char *new = new_temp();
                                    add_tac("%s = %s\n", new, $3->value);
                                     char *new1 = new_temp();
                                    add_tac("%s = %s - %s\n", new1, $1,new);
                                    add_tac("%s = %s\n",$1, new1);
                                 }
                                 else if(strcmp($2->value, "*=") == 0){
                                    char *new = new_temp();
                                    add_tac("%s = %s\n", new, $3->value);
                                     char *new1 = new_temp();
                                    add_tac("%s = %s * %s\n", new1, $1,new);
                                    add_tac("%s = %s\n",$1, new1);
                                 }
                                 else{
                                    char *new = new_temp();
                                    add_tac("%s = %s\n", new, $3->value);
                                     char *new1 = new_temp();
                                    add_tac("%s = %s / %s\n", new1, $1,new);
                                    add_tac("%s = %s\n",$1, new1);
                                 }
                                
                                  Node* _temp = createNode("",NULL,NULL,NULL);
                                  $$ = _temp;
                                
                                 
                                      }
    
                                 } }
                               ;




B: INT {dummy=$$ = createType($1, 4);}
   | FLOAT {dummy=$$ = createType($1, 4);}
   | CHAR  {dummy=$$ = createType($1, 1);}
   ;


ASSIGN: EQ { $$ = createNode(strdup("="), NULL, NULL, NULL); }
       |PEQ { $$ = createNode(strdup("+="), NULL, NULL, NULL); }
       |MEQ { $$ = createNode(strdup("-="), NULL, NULL, NULL); }
       |MLEQ { $$ = createNode(strdup("*="), NULL, NULL, NULL); }
       |DEQ  { $$ = createNode(strdup("/="), NULL, NULL, NULL); }
       ;

expr:
    expr '+' expr { //printf("hi +\n");
                     if(!error_flag)
                      {
                    char* label = new_temp();
                    add_tac("%s = %s + %s\n", label, $1->value, $3->value);
                    $$ = createNode(label, NULL, NULL, NULL);
                    }
                   }
  | expr '-' expr { //printf("hi -\n");
                      if(!error_flag)
            {char* label = new_temp();
                    add_tac("%s = %s - %s\n", label, $1->value, $3->value);
                    $$ = createNode(label, NULL, NULL, NULL);
                    }
                   }
  | expr '*' expr {  //printf("hi *\n");
                     if(!error_flag)
            {char* label = new_temp();
                    add_tac("%s = %s * %s\n", label, $1->value, $3->value);
                    $$ = createNode(label, NULL, NULL, NULL);
                    }
                   }
  | expr '/' expr  {//printf("hi /\n");
                    if(!error_flag)
            { char* label = new_temp();
                    add_tac("%s = %s / %s\n", label, $1->value, $3->value);
                    $$ = createNode(label, NULL, NULL, NULL);
                   }
                   }
  | expr '%' expr  { //printf("hi %\n");
                     if(!error_flag)
            {char* label = new_temp();
                    add_tac("%s = %s %% %s\n", label, $1->value, $3->value);
                    $$ = createNode(label, NULL, NULL, NULL);
                    }
                   }
  | '(' expr ')'   {// printf("hi (\n"); 
                       if(!error_flag)
                        {  Node* temp = createNode($2->value,$2->trueList,$2->falseList,NULL);
                        $$ = temp;}
                        }
  | expr GT expr  { //printf("hi %\n");
                     if(!error_flag)
            {Node* temp = createNode("",makelist(tac_count),makelist(tac_count+1),NULL);
                     add_tac("if %s %s %s goto ", $1->value, ">", $3->value);
                     add_tac("goto ");
                     $$ = temp;
                     }
                   }
  | expr LT expr  { //printf("hi %\n");
                     if(!error_flag)
                    { Node* temp = createNode("",makelist(tac_count),makelist(tac_count+1),NULL);
                     add_tac("if %s %s %s goto ", $1->value, "<", $3->value);
                     add_tac("goto ");
                     $$ = temp;
                     }
                   }
  | expr GTE expr  { //printf("hi %\n");
                    Node* temp = createNode("",makelist(tac_count),makelist(tac_count+1),NULL);
                     add_tac("if %s %s %s goto ", $1->value, ">=", $3->value);
                     add_tac("goto ");
                     $$ = temp;
                   }
  | expr LTE expr  { //printf("hi %\n");
                    if(!error_flag)
            { Node* temp = createNode("",makelist(tac_count),makelist(tac_count+1),NULL);
                     add_tac("if %s %s %s goto ", $1->value, "<=", $3->value);
                     add_tac("goto ");
                     $$ = temp;}
                   }
  | expr NE expr  { 
                    if(!error_flag)
            { Node* temp = createNode("",makelist(tac_count),makelist(tac_count+1),NULL);
                     add_tac("if %s %s %s goto ", $1->value, "!=", $3->value);
                     add_tac("goto ");
                     $$ = temp;
                     }
                   }
  | expr EE expr  {   if(!error_flag)
            {Node* temp = createNode("",makelist(tac_count),makelist(tac_count+1),NULL);
                     add_tac("if %s %s %s goto ", $1->value, "==", $3->value);
                     add_tac("goto ");
                     $$ = temp;}
                   }
  | expr AND M expr { 
                          if(!error_flag)
            { Node* temp = createNode("",$4->trueList,merge($1->falseList,$4->falseList),NULL);
                          backpatch($1->trueList,$3);
                          $$ = temp;}
                     }
  | expr OR N expr  { 
                       if(!error_flag)
            {Node* temp = createNode("",merge($1->trueList,$4->trueList),$4->falseList,NULL);
                      backpatch($1->falseList,$3);
                      $$ = temp;}
                    }
  | NN expr {
   
          if(!error_flag)
            {Node* temp = createNode("",$2->falseList,$2->trueList,NULL);
                      $$ = temp;}
      }
  | '(' expr error { printf("Rejected - Right paranthesis missing\n"); error_flag = 1; yyerror("Rejected - Invalid operand for modulus operator");  yyerrok;  } 
  
  | term { $$=$1 ;}
  | T {  if(!error_flag)
            {Node* temp = createNode("",makelist(tac_count),NULL,NULL);
            sprintf(tac[tac_count++], "goto ");
            $$ = temp;}}
  | F{  if(!error_flag)
            {Node* temp = createNode("",NULL,makelist(tac_count),NULL);
            sprintf(tac[tac_count++], "goto ");
            $$ = temp;}}
  ;
  
  
N: {$$ = tac_count; char *dummy = "goto "; add_tac("%s",dummy); };
M: {$$ = tac_count;};
O:  {
        // init a scope stack and an offset stack 
        scopeStack = createScopeStack();
        offsetStack = createOffsetStack();
    };
P:  {   
        // init the offset and a symbol table 
        offset = 0; currentSymbolTable = createSymbolTable();
        symbolTableArray[symbolTableCount] = currentSymbolTable;
                        symbolTableCount++;
    }
Q:  {  

        pushScope(scopeStack, currentSymbolTable);
        currentSymbolTable = createSymbolTable();
        pushOffset(offsetStack, offset);
        offset = 0;
        symbolTableArray[symbolTableCount] = currentSymbolTable;
                          //  printSymbolTable(currentSymbolTable);
                        symbolTableCount++;
    }
term: UM IDENTIFIER  {
                        if(!error_flag)
                       {
                       if(checkSymbolExistsInAnyTable($2) == 0){
                                printf("error : var ' %s ' is not declared in the scope\n",$2); error_flag = 1;
                            }
                       char* label = new_temp();
                       add_tac("%s = -%s\n", label, $2);
                       Node* temp = createNode(label,NULL,NULL,NULL);
                       $$ = temp;
                       }
                       

                     }
                     
     | IDENTIFIER  {
                        if(!error_flag)
                       { 
                            if(checkSymbolExistsInAnyTable($1) == 0){
                                printf("error : var ' %s ' is not declared in the scope\n",$1); error_flag = 1;
                            }
                         Node* temp = createNode($1,NULL,NULL,NULL);
                         $$ = temp; 
                         }
                       
                     }                
                     
     | UM DOUBLE IDENTIFIER 
                           {    if(!error_flag)
                              {
                                if(checkSymbolExistsInAnyTable($3) == 0){
                                printf("error : var ' %s ' is not declared in the scope\n",$3); error_flag = 1;
                            }
                               
                                 char* label = new_temp();
                                 if(strcmp($2->value, "--") == 0)
                                 {
                                   printf("Rejected - Expression is not assignable\n");   error_flag=1;
                                 }
                                 else
                                 {
                                 char* new = new_temp();
                                 char* new1 = new_temp();
                                   add_tac("%s = %s + 1\n",new1, $3);
                                   add_tac("%s = %s\n", new, $3);
                                   add_tac("%s = -%s\n",label,new);
                                   add_tac("%s = %s\n",$3, new1);
                                   Node* temp = createNode(label,NULL,NULL,NULL);
                                   $$ = temp;
                                   
                                 }
                               
                               $$->notassign = true;
                               }
                           }
                           
                           
     | DOUBLE IDENTIFIER 
                           {    if(!error_flag)
                               {
                                if(checkSymbolExistsInAnyTable($2) == 0){
                                printf("error : var ' %s ' is not declared in the scope\n",$2); error_flag = 1;
                            }
                                char* label = new_temp();
                                if(strcmp($1->value, "--") == 0)
                                 { char* new1 = new_temp();
                                   add_tac("%s = %s - 1\n", new1, $2);
                                   add_tac("%s = %s\n", label, $2);
                                   add_tac("%s = %s\n", $2, new1);
                                   Node* temp = createNode(label,NULL,NULL,NULL);
                                   $$ = temp;
                                 }
                                 else
                                 { char* new1 = new_temp();
                                   add_tac("%s = %s + 1\n", new1, $2);
                                   add_tac("%s = %s\n", label, $2);
                                   add_tac("%s = %s\n", $2, new1);
                                   Node* temp = createNode(label,NULL,NULL,NULL);
                                   $$ = temp; 
                                 }
                               $$->notassign = true;
                               }
                           }
                           
     | UM IDENTIFIER DOUBLE 
                           {   
                            if(!error_flag)
                           {     if(checkSymbolExistsInAnyTable($2) == 0){
                                printf("error : var ' %s ' is not declared in the scope\n",$2); error_flag = 1;
                            }            
                                 char* label = new_temp();
                                 if(strcmp($3->value, "--") == 0)
                                 {
                                   char* new = new_temp();
                                   char* new1 = new_temp();
                                   add_tac("%s = %s\n", new, $2);
                                   add_tac("%s = -%s\n",label,new);
                                   add_tac("%s = %s - 1\n", new1, $2);
                                   add_tac("%s = %s\n", $2, new1);
                                   Node* temp = createNode(label,NULL,NULL,NULL);
                                   $$ = temp;
                                 }
                                 else
                                 {
                                   char* new = new_temp();
                                   char* new1 = new_temp();
                                   add_tac("%s = %s\n", new, $2);
                                   add_tac("%s = -%s\n",label,new);
                                   add_tac("%s = %s + 1\n", new1, $2);
                                   add_tac("%s = %s\n", $2, new1);
                                   Node* temp = createNode(label,NULL,NULL,NULL);
                                   $$ = temp;
                                 }                                                             
                                $$->notassign = true;
                             }
                           }
                           
                           
                           
     | IDENTIFIER DOUBLE 
                           {    if(!error_flag)
                              {
                                if(checkSymbolExistsInAnyTable($1) == 0){
                                printf("error : var ' %s ' is not declared in the scope\n",$1); error_flag = 1;
                            }
                                char* label = new_temp();
                                if(strcmp($2->value, "--") == 0)
                                 {  char* new1 = new_temp();
                                   add_tac("%s = %s\n", label, $1);
                                   add_tac("%s = %s - 1\n", new1, $1);
                                   add_tac("%s = %s\n", $1, new1);
                                   Node* temp = createNode(label,NULL,NULL,NULL);
                                   $$ = temp;
                                 }
                                 else
                                 { char* new1 = new_temp();
                                   add_tac("%s = %s\n", label, $1);
                                   add_tac("%s = %s + 1\n", new1, $1);
                                   add_tac("%s = %s\n", $1, new1);    
                                   Node* temp = createNode(label,NULL,NULL,NULL);
                                   $$ = temp;                             
                                 }
                               
                                $$->notassign = true;
                                
                               }
                                
                           }
     
                           
     | UM NUMBER{
                    if(!error_flag)
                     {
                       Node* temp = createNode(strcat($1->value,$2),NULL,NULL,NULL);
                        $$ = temp;          
                       }
                 }
     |  NUMBER{
                    if(!error_flag)
                       {
                         Node* temp = createNode($1,NULL,NULL,NULL);
                         $$ = temp;
                       }    
                 }
     
     
                 
     | UM DOUBLE IDENTIFIER  DOUBLE {printf("Rejected - Expression is not assignable\n");   error_flag=1; }
     | UM PP NUMBER {printf("Rejected - cannot increment a constant value\n");   error_flag=1; }
     | UM MM NUMBER {printf("Rejected - cannot decrement a constant value\n");   error_flag=1; }
     | UM NUMBER PP {printf("Rejected - cannot increment a constant value\n");   error_flag=1; }
     | UM NUMBER MM {printf("Rejected - cannot decrement a constant value\n");   error_flag=1; }
     | UM DOUBLE IDENTIFIER IDENTIFIER {printf("Rejected - Missing operator\n"); error_flag=1;}
     | UM IDENTIFIER DOUBLE IDENTIFIER {printf("Rejected - Missing operator\n"); error_flag=1;}
     | UM DOUBLE IDENTIFIER NUMBER {printf("Rejected - Missing operator\n");  error_flag=1;}
     | UM IDENTIFIER DOUBLE NUMBER {printf("Rejected - Missing operator\n");   error_flag=1;}
     | UM NUMBER IDENTIFIER {printf("Rejected - Missing operator\n");  error_flag=1;}
     | UM IDENTIFIER NUMBER {printf("Rejected - Missing operator\n");  error_flag=1;}
     | UM IDENTIFIER DOUBLE DOUBLE {printf("Rejected - Expression is not assignable\n"); error_flag=1;}
     | DOUBLE IDENTIFIER  DOUBLE {printf("Rejected - Expression is not assignable\n");   error_flag=1; }
     | PP NUMBER {printf("Rejected - cannot increment a constant value\n");   error_flag=1; }
     | MM NUMBER {printf("Rejected - cannot decrement a constant value\n");   error_flag=1; }
     | NUMBER PP {printf("Rejected - cannot increment a constant value\n");   error_flag=1; }
     | NUMBER MM {printf("Rejected - cannot decrement a constant value\n");   error_flag=1; }
     | DOUBLE IDENTIFIER IDENTIFIER {printf("Rejected - Missing operator\n"); error_flag=1;}
     | IDENTIFIER DOUBLE IDENTIFIER {printf("Rejected - Missing operator\n"); error_flag=1;}
     | DOUBLE IDENTIFIER NUMBER {printf("Rejected - Missing operator\n");  error_flag=1;}
     | IDENTIFIER DOUBLE NUMBER {printf("Rejected - Missing operator\n");   error_flag=1;}
     | NUMBER IDENTIFIER {printf("Rejected - Missing operator\n");  error_flag=1;}
     | IDENTIFIER NUMBER {printf("Rejected - Missing operator\n");  error_flag=1;}
     | IDENTIFIER DOUBLE DOUBLE {printf("Rejected - Expression is not assignable\n"); error_flag=1;}
     ;

UM: '-' {   Node* temp = createNode(strdup("-"),NULL,NULL,NULL);
           $$ = temp;
            }           ;
DOUBLE: PP {  
            Node* temp = createNode(strdup("++"),NULL,NULL,NULL);
           $$ = temp; }
        | MM {  Node* temp = createNode(strdup("--"),NULL,NULL,NULL);
           $$ = temp; };



%%

void yyerror(const char *s) {
    //printf("%s\n", s);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    yyin = fopen(argv[1], "r");
    if (!yyin) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    
    yyparse();
    
    fclose(yyin);
    return 0;
}

