%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
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





%}

%union {
    char* value;   // For storing string values like identifiers and numbers
     struct node* node;
     int instr;
     struct list* list;
}

%token <value> IDENTIFIER NUMBER EQ PEQ MEQ MLEQ DEQ MM PP LB RB GT LT GTE LTE NE EE AND OR IF ELSE T F WHILE NN
%left '+' '-'
%left '*' '/' '%'
%nonassoc PP MM
%nonassoc '(' ')'
%left OR
%left AND
%nonassoc NE EE
%nonassoc NN
%nonassoc GTE LTE GT LT
%nonassoc LB RB
%type <instr> M N
%type <node> stmt_list stmt expr term  UM DOUBLE ASSIGN
%left ELSE
%%

program: stmt_list{
        if(!error_flag)
     {  backpatch($1->nextList,tac_count);
        printf("----------Intermediate Code----------\n");
        for (int i = 0; i < tac_count; i++) {
            printf("%d: %s", i, tac[i]);
        }
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
    expr ASSIGN expr ';' { if(!error_flag)
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
    
                                 } 
                                 
                          } 
                 
  | IF '(' expr ')' M stmt   {
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
  | LB stmt_list RB  {  if(!error_flag)
                        {
                        Node* temp = createNode("",NULL,NULL,$2->nextList);
                        $$ = temp;
                        }
                      }
  | LB RB { 
        if(!error_flag){
            Node* temp = createNode("",NULL,NULL,NULL);
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
  | expr ';' {if(!error_flag) {$$=$1;}}
  | error { if(!error_flag) {printf("Rejected - Invalid Expression\n");error_flag=1;yyerrok;}}
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
            {Node* temp = createNode("",makelist(tac_count),makelist(tac_count+1),NULL);
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

term: UM IDENTIFIER  {
                       if($1->value != NULL && strcmp($1->value, "-") == 0)
                       {
                        if(!error_flag)
            {
                       char* label = new_temp();
                       add_tac("%s = -%s\n", label, $2);
                       Node* temp = createNode(label,NULL,NULL,NULL);
                       $$ = temp;
                       }
                       }
                       else
                       { if(!error_flag)
            {
                         Node* temp = createNode($2,NULL,NULL,NULL);
                         $$ = temp; }
                       }
                     }
     | UM DOUBLE IDENTIFIER 
                           {    if(!error_flag)
            {
                               if($1->value != NULL && strcmp($1->value, "-") == 0)
                               {
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
                               }
                               else
                               {
                                char* label = new_temp();
                                if(strcmp($2->value, "--") == 0)
                                 { char* new1 = new_temp();
                                   add_tac("%s = %s - 1\n", new1, $3);
                                   add_tac("%s = %s\n", label, $3);
                                   add_tac("%s = %s\n", $3, new1);
                                   Node* temp = createNode(label,NULL,NULL,NULL);
                                   $$ = temp;
                                 }
                                 else
                                 { char* new1 = new_temp();
                                   add_tac("%s = %s + 1\n", new1, $3);
                                   add_tac("%s = %s\n", label, $3);
                                   add_tac("%s = %s\n", $3, new1);
                                   Node* temp = createNode(label,NULL,NULL,NULL);
                                   $$ = temp;
                                   
                                 }
                               }
                               $$->notassign = true;
                               }
                           }
     | UM IDENTIFIER DOUBLE 
                           {    if(!error_flag)
            {
                               if($1->value != NULL && strcmp($1->value, "-") == 0)
                               {
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
                               }
                               else
                               {
                                char* label = new_temp();
                                if(strcmp($3->value, "--") == 0)
                                 {  char* new1 = new_temp();
                                   add_tac("%s = %s\n", label, $2);
                                   add_tac("%s = %s - 1\n", new1, $2);
                                   add_tac("%s = %s\n", $2, new1);
                                   Node* temp = createNode(label,NULL,NULL,NULL);
                                   $$ = temp;
                                 }
                                 else
                                 { char* new1 = new_temp();
                                   add_tac("%s = %s\n", label, $2);
                                   add_tac("%s = %s + 1\n", new1, $2);
                                   add_tac("%s = %s\n", $2, new1);    
                                   Node* temp = createNode(label,NULL,NULL,NULL);
                                   $$ = temp;                             
                                 }
                               }
                                $$->notassign = true;}
                           }
     | UM NUMBER{
                    if(!error_flag)
            {
                       if($1->value != NULL && strcmp($1->value, "-") == 0)
                       {
                       Node* temp = createNode(strcat($1->value,$2),NULL,NULL,NULL);
                        $$ = temp;
                       }
                       else
                       {
                         Node* temp = createNode($2,NULL,NULL,NULL);
                         $$ = temp;
                       }}
                     
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
     ;

UM: '-' {   Node* temp = createNode(strdup("-"),NULL,NULL,NULL);
           $$ = temp;
            }
   |  /* empty */ {   Node* temp = createNode("not needed",NULL,NULL,NULL);
           $$ = temp;}
           
;

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

