/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "prob.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include <ctype.h> 
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
int var_list_brac[1000]; 
int var_count = 0;
int bracError = 0;
int is_number(const char *str) {
    char *endptr;
    
    // Empty strings or strings with only spaces are not valid numbers
    if (*str == '\0' || isspace(*str)) return 0;

    // Attempt to parse the string as a double
    strtod(str, &endptr);

    // `strtod` sets `endptr` to point to the first invalid character. 
    // A valid number will have `endptr` point to the end of the string.
    return *endptr == '\0' && *str != '\0'; // `*str != '\0'` ensures non-empty input
}

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
 char value[1000]; // typecasting
 List* trueList;
 bool notassign;
 List* falseList;
 List* nextList;
 char type[1000];
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
Node* createNode(char* value, List* trueList, List* falseList, List* nextList , char * type){
 Node* n = (Node*)malloc(sizeof(Node));
 strcpy(n->value, value);
 n->nextList = nextList;
 n->trueList = trueList;
 n->falseList = falseList;
  strcpy(n->type, type);
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

HashNode* checkSymbolExistsInAnyTable(const char* name) {
    for (int i = symbolTableCount; i >=0; i--) {
        if (lookupSymbol(symbolTableArray[i], name) != NULL) {
            return lookupSymbol(symbolTableArray[i],name);  // Symbol found in this table
        }
    }
    return NULL;  // Symbol not found in any table
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


char* max(char* t1, char* t2) {
    // Handling "int" and "float" types
    char* type = (char*)malloc(sizeof(char) * 1000);
    if (strcmp(t1, "float") == 0 || strcmp(t2, "float") == 0) {
        sprintf(type, "%s", "float");
        return type;
    }
    sprintf(type, "%s", "int");
    return type;
}

char* widen(char* var, char* t, char* w){
    if(strcmp(t,w) == 0) return var;
    else if(strcmp(t, "int") == 0 && strcmp(w, "float") == 0){
        char* temp = gen_label();
        sprintf(tac[tac_count++],"%s = (float) %s\n", temp, var);
        return temp;
    }
}

void substring(const char *source, char *dest, int start, int length) {
    // Copy `length` characters from `source + start` to `dest`
    strncpy(dest, source + start, length);
    dest[length] = '\0';  // Null-terminate the destination string
}


#line 558 "y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    IDENTIFIER = 258,              /* IDENTIFIER  */
    NUMBER = 259,                  /* NUMBER  */
    EQ = 260,                      /* EQ  */
    PEQ = 261,                     /* PEQ  */
    MEQ = 262,                     /* MEQ  */
    MLEQ = 263,                    /* MLEQ  */
    DEQ = 264,                     /* DEQ  */
    MM = 265,                      /* MM  */
    PP = 266,                      /* PP  */
    LB = 267,                      /* LB  */
    RB = 268,                      /* RB  */
    GT = 269,                      /* GT  */
    LT = 270,                      /* LT  */
    GTE = 271,                     /* GTE  */
    LTE = 272,                     /* LTE  */
    NE = 273,                      /* NE  */
    EE = 274,                      /* EE  */
    AND = 275,                     /* AND  */
    OR = 276,                      /* OR  */
    IF = 277,                      /* IF  */
    ELSE = 278,                    /* ELSE  */
    T = 279,                       /* T  */
    F = 280,                       /* F  */
    WHILE = 281,                   /* WHILE  */
    NN = 282,                      /* NN  */
    INT = 283,                     /* INT  */
    FLOAT = 284,                   /* FLOAT  */
    CHAR = 285,                    /* CHAR  */
    LS = 286,                      /* LS  */
    RS = 287                       /* RS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define IDENTIFIER 258
#define NUMBER 259
#define EQ 260
#define PEQ 261
#define MEQ 262
#define MLEQ 263
#define DEQ 264
#define MM 265
#define PP 266
#define LB 267
#define RB 268
#define GT 269
#define LT 270
#define GTE 271
#define LTE 272
#define NE 273
#define EE 274
#define AND 275
#define OR 276
#define IF 277
#define ELSE 278
#define T 279
#define F 280
#define WHILE 281
#define NN 282
#define INT 283
#define FLOAT 284
#define CHAR 285
#define LS 286
#define RS 287

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 488 "prob.y"

    char* value;   // For storing string values like identifiers and numbers
     struct node* node;
     int instr;
     struct list* list;
     struct Type* type;
     

#line 684 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_IDENTIFIER = 3,                 /* IDENTIFIER  */
  YYSYMBOL_NUMBER = 4,                     /* NUMBER  */
  YYSYMBOL_EQ = 5,                         /* EQ  */
  YYSYMBOL_PEQ = 6,                        /* PEQ  */
  YYSYMBOL_MEQ = 7,                        /* MEQ  */
  YYSYMBOL_MLEQ = 8,                       /* MLEQ  */
  YYSYMBOL_DEQ = 9,                        /* DEQ  */
  YYSYMBOL_MM = 10,                        /* MM  */
  YYSYMBOL_PP = 11,                        /* PP  */
  YYSYMBOL_LB = 12,                        /* LB  */
  YYSYMBOL_RB = 13,                        /* RB  */
  YYSYMBOL_GT = 14,                        /* GT  */
  YYSYMBOL_LT = 15,                        /* LT  */
  YYSYMBOL_GTE = 16,                       /* GTE  */
  YYSYMBOL_LTE = 17,                       /* LTE  */
  YYSYMBOL_NE = 18,                        /* NE  */
  YYSYMBOL_EE = 19,                        /* EE  */
  YYSYMBOL_AND = 20,                       /* AND  */
  YYSYMBOL_OR = 21,                        /* OR  */
  YYSYMBOL_IF = 22,                        /* IF  */
  YYSYMBOL_ELSE = 23,                      /* ELSE  */
  YYSYMBOL_T = 24,                         /* T  */
  YYSYMBOL_F = 25,                         /* F  */
  YYSYMBOL_WHILE = 26,                     /* WHILE  */
  YYSYMBOL_NN = 27,                        /* NN  */
  YYSYMBOL_INT = 28,                       /* INT  */
  YYSYMBOL_FLOAT = 29,                     /* FLOAT  */
  YYSYMBOL_CHAR = 30,                      /* CHAR  */
  YYSYMBOL_LS = 31,                        /* LS  */
  YYSYMBOL_RS = 32,                        /* RS  */
  YYSYMBOL_33_ = 33,                       /* '+'  */
  YYSYMBOL_34_ = 34,                       /* '-'  */
  YYSYMBOL_35_ = 35,                       /* '*'  */
  YYSYMBOL_36_ = 36,                       /* '/'  */
  YYSYMBOL_37_ = 37,                       /* '%'  */
  YYSYMBOL_38_ = 38,                       /* '('  */
  YYSYMBOL_39_ = 39,                       /* ')'  */
  YYSYMBOL_40_ = 40,                       /* ';'  */
  YYSYMBOL_41_ = 41,                       /* ','  */
  YYSYMBOL_YYACCEPT = 42,                  /* $accept  */
  YYSYMBOL_program = 43,                   /* program  */
  YYSYMBOL_stmt_list = 44,                 /* stmt_list  */
  YYSYMBOL_stmt = 45,                      /* stmt  */
  YYSYMBOL_DECLARE_STMT = 46,              /* DECLARE_STMT  */
  YYSYMBOL_comp = 47,                      /* comp  */
  YYSYMBOL_A = 48,                         /* A  */
  YYSYMBOL_CONTROL_FLOW_STMT = 49,         /* CONTROL_FLOW_STMT  */
  YYSYMBOL_ASSIGN_STMT = 50,               /* ASSIGN_STMT  */
  YYSYMBOL_B = 51,                         /* B  */
  YYSYMBOL_ASSIGN = 52,                    /* ASSIGN  */
  YYSYMBOL_expr = 53,                      /* expr  */
  YYSYMBOL_N = 54,                         /* N  */
  YYSYMBOL_M = 55,                         /* M  */
  YYSYMBOL_O = 56,                         /* O  */
  YYSYMBOL_P = 57,                         /* P  */
  YYSYMBOL_Q = 58,                         /* Q  */
  YYSYMBOL_term = 59,                      /* term  */
  YYSYMBOL_UM = 60,                        /* UM  */
  YYSYMBOL_DOUBLE = 61                     /* DOUBLE  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   274

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  42
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  20
/* YYNRULES -- Number of rules.  */
#define YYNRULES  91
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  143

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   287


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    37,     2,     2,
      38,    39,    35,    33,    41,    34,     2,    36,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    40,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   514,   514,   533,   545,   555,   563,   569,   570,   583,
     589,   590,   594,   637,   644,   662,   664,   684,   685,   687,
     709,   716,   727,   737,   744,   863,   864,   865,   869,   870,
     871,   872,   873,   877,   887,   897,   907,   917,   927,   932,
     940,   948,   954,   961,   969,   975,   981,   987,   993,   995,
     996,  1000,  1007,  1008,  1009,  1014,  1020,  1030,  1047,  1062,
    1093,  1122,  1159,  1192,  1199,  1209,  1210,  1211,  1212,  1213,
    1214,  1215,  1216,  1217,  1218,  1219,  1220,  1221,  1222,  1223,
    1224,  1225,  1226,  1227,  1228,  1229,  1230,  1231,  1232,  1235,
    1238,  1241
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "IDENTIFIER", "NUMBER",
  "EQ", "PEQ", "MEQ", "MLEQ", "DEQ", "MM", "PP", "LB", "RB", "GT", "LT",
  "GTE", "LTE", "NE", "EE", "AND", "OR", "IF", "ELSE", "T", "F", "WHILE",
  "NN", "INT", "FLOAT", "CHAR", "LS", "RS", "'+'", "'-'", "'*'", "'/'",
  "'%'", "'('", "')'", "';'", "','", "$accept", "program", "stmt_list",
  "stmt", "DECLARE_STMT", "comp", "A", "CONTROL_FLOW_STMT", "ASSIGN_STMT",
  "B", "ASSIGN", "expr", "N", "M", "O", "P", "Q", "term", "UM", "DOUBLE", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-48)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-14)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -48,    12,   -48,   -48,    94,   -48,    98,     5,    21,    26,
      15,    -3,   -48,   -48,   -48,   178,   -48,   -48,   -48,   -48,
     178,    33,   -48,     0,   -48,     4,    44,   159,   -48,    69,
      46,   -48,   -48,   -48,   126,   -48,   -48,   -48,   -48,   -48,
     -48,    94,     7,    25,   -48,   124,    94,   -48,   -48,   105,
     -28,   -48,   -48,   -48,   -48,   -48,   178,   178,   178,   178,
     178,   178,   -48,   -48,   178,   178,   178,   178,   178,   -48,
     178,   196,    16,    61,    71,    73,   143,   -48,   -48,   -48,
      65,   -48,   203,   178,   -48,   -48,   -48,   178,    83,    86,
     -48,    49,    49,    49,    49,    24,    24,   178,   178,     2,
       2,   151,   151,   151,   237,   -48,   145,   -48,   -48,   -48,
     -48,   -48,   180,   -48,   -48,   -48,   -48,   -48,   211,   237,
      58,   105,    38,   -13,   -48,   -48,   -48,   -48,   -48,   -48,
      94,   -48,    63,   178,    77,    94,   -48,   237,   -48,   -48,
     -48,    94,   -48
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
      54,     0,    55,     1,     0,    11,    58,    64,    91,    90,
      56,     0,    50,    51,    53,     0,    25,    26,    27,    89,
       0,    53,     4,     0,     7,     0,     0,     0,    49,     0,
       0,    87,    91,    90,    62,    86,    81,    80,    79,    78,
       9,     0,     0,     0,    47,     0,     0,     5,     6,    18,
      15,    28,    29,    30,    31,    32,     0,     0,     0,     0,
       0,     0,    53,    52,     0,     0,     0,     0,     0,    10,
       0,    57,    63,    91,    90,     0,    60,    83,    85,    88,
      53,    23,     0,     0,    48,    38,     3,     0,     0,     0,
      12,    39,    40,    41,    42,    43,    44,     0,     0,    33,
      34,    35,    36,    37,    24,    75,    61,    74,    69,    68,
      67,    66,    59,    82,    84,    77,     8,    53,     0,    19,
       0,    17,    45,    46,    71,    73,    76,    70,    72,    65,
       0,    53,    15,     0,    20,     0,    14,    16,    52,    22,
      53,     0,    21
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -48,   -48,    55,   -42,   -48,   -17,   -48,   -48,   -48,   -48,
     -47,    -6,    -7,   -14,   -48,   -48,   -48,   -48,   -48,    -5
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     1,    21,    22,    23,    90,    50,    24,    25,    26,
      70,    27,    98,    46,     2,     4,    41,    28,    29,    30
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      43,    34,    87,    88,    86,    60,    61,    62,    35,    44,
       6,     7,     3,    89,    45,    36,    37,     8,     9,   107,
      60,    61,    62,    63,    75,    38,   108,   109,    40,    79,
      39,    12,    13,    -2,    15,    42,    82,    66,    67,    68,
      47,    19,   -14,   -14,    48,    20,    81,    49,    97,    76,
      91,    92,    93,    94,    95,    96,    60,    61,    99,   100,
     101,   102,   103,    83,   104,   110,   106,    60,    61,    62,
      63,   115,    71,    72,   133,   111,   112,   118,   116,    73,
      74,   119,    64,    65,    66,    67,    68,   120,   134,   121,
     132,   122,   123,   139,    88,     5,    80,     6,     7,   142,
     138,   126,    31,   130,     8,     9,    10,   129,    32,    33,
      51,    52,    53,    54,    55,   136,    11,   135,    12,    13,
      14,    15,    16,    17,    18,    84,   141,   137,    19,    77,
      78,   140,    20,     0,   -13,     0,    32,    33,    56,    57,
      58,    59,    60,    61,    62,    63,   113,   114,   124,   125,
       0,     0,     0,    32,    33,    32,    33,    64,    65,    66,
      67,    68,     0,    85,    51,    52,    53,    54,    55,    60,
      61,    62,    63,    56,    57,    58,    59,    60,    61,    62,
      63,     6,     7,   127,   128,     0,     0,     0,     8,     9,
      32,    33,    64,    65,    66,    67,    68,     0,     0,    69,
     105,     0,    12,    13,     0,    15,    32,    33,     0,     0,
       0,     0,    19,     0,     0,     0,    20,    56,    57,    58,
      59,    60,    61,    62,    63,    56,    57,    58,    59,    60,
      61,    62,    63,     0,     0,     0,    64,    65,    66,    67,
      68,     0,   117,     0,    64,    65,    66,    67,    68,     0,
     131,    56,    57,    58,    59,    60,    61,    62,    63,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      64,    65,    66,    67,    68
};

static const yytype_int16 yycheck[] =
{
      14,     6,    49,    31,    46,    18,    19,    20,     3,    15,
       3,     4,     0,    41,    20,    10,    11,    10,    11,     3,
      18,    19,    20,    21,    29,     4,    10,    11,    13,    34,
       4,    24,    25,     0,    27,    38,    42,    35,    36,    37,
      40,    34,    18,    19,    40,    38,    39,     3,    62,     3,
      56,    57,    58,    59,    60,    61,    18,    19,    64,    65,
      66,    67,    68,    38,    70,     4,    71,    18,    19,    20,
      21,    76,     3,     4,   121,     4,     3,    83,    13,    10,
      11,    87,    33,    34,    35,    36,    37,     4,   130,     3,
      32,    97,    98,   135,    31,     1,    41,     3,     4,   141,
      23,   106,     4,   117,    10,    11,    12,   112,    10,    11,
       5,     6,     7,     8,     9,   132,    22,   131,    24,    25,
      26,    27,    28,    29,    30,     1,   140,   133,    34,     3,
       4,   138,    38,    -1,    40,    -1,    10,    11,    14,    15,
      16,    17,    18,    19,    20,    21,     3,     4,     3,     4,
      -1,    -1,    -1,    10,    11,    10,    11,    33,    34,    35,
      36,    37,    -1,    39,     5,     6,     7,     8,     9,    18,
      19,    20,    21,    14,    15,    16,    17,    18,    19,    20,
      21,     3,     4,     3,     4,    -1,    -1,    -1,    10,    11,
      10,    11,    33,    34,    35,    36,    37,    -1,    -1,    40,
       4,    -1,    24,    25,    -1,    27,    10,    11,    -1,    -1,
      -1,    -1,    34,    -1,    -1,    -1,    38,    14,    15,    16,
      17,    18,    19,    20,    21,    14,    15,    16,    17,    18,
      19,    20,    21,    -1,    -1,    -1,    33,    34,    35,    36,
      37,    -1,    39,    -1,    33,    34,    35,    36,    37,    -1,
      39,    14,    15,    16,    17,    18,    19,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    34,    35,    36,    37
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    43,    56,     0,    57,     1,     3,     4,    10,    11,
      12,    22,    24,    25,    26,    27,    28,    29,    30,    34,
      38,    44,    45,    46,    49,    50,    51,    53,    59,    60,
      61,     4,    10,    11,    61,     3,    10,    11,     4,     4,
      13,    58,    38,    55,    53,    53,    55,    40,    40,     3,
      48,     5,     6,     7,     8,     9,    14,    15,    16,    17,
      18,    19,    20,    21,    33,    34,    35,    36,    37,    40,
      52,     3,     4,    10,    11,    61,     3,     3,     4,    61,
      44,    39,    53,    38,     1,    39,    45,    52,    31,    41,
      47,    53,    53,    53,    53,    53,    53,    55,    54,    53,
      53,    53,    53,    53,    53,     4,    61,     3,    10,    11,
       4,     4,     3,     3,     4,    61,    13,    39,    53,    53,
       4,     3,    53,    53,     3,     4,    61,     3,     4,    61,
      55,    39,    32,    52,    45,    55,    47,    53,    23,    45,
      54,    55,    45
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    42,    43,    44,    44,    45,    45,    45,    45,    45,
      45,    45,    46,    46,    47,    47,    48,    48,    48,    48,
      49,    49,    49,    49,    50,    51,    51,    51,    52,    52,
      52,    52,    52,    53,    53,    53,    53,    53,    53,    53,
      53,    53,    53,    53,    53,    53,    53,    53,    53,    53,
      53,    53,    54,    55,    56,    57,    58,    59,    59,    59,
      59,    59,    59,    59,    59,    59,    59,    59,    59,    59,
      59,    59,    59,    59,    59,    59,    59,    59,    59,    59,
      59,    59,    59,    59,    59,    59,    59,    59,    59,    60,
      61,    61
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     3,     3,     1,     2,     2,     1,     4,     2,
       2,     1,     3,     0,     4,     0,     5,     3,     1,     3,
       6,    10,     7,     3,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     4,     4,     2,     3,     1,
       1,     1,     0,     0,     0,     0,     0,     2,     1,     3,
       2,     3,     2,     2,     1,     4,     3,     3,     3,     3,
       4,     4,     4,     4,     3,     3,     4,     3,     2,     2,
       2,     2,     3,     3,     3,     3,     2,     2,     3,     1,
       1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: O P stmt_list  */
#line 514 "prob.y"
                      {
        if(!error_flag)
     {  backpatch((yyvsp[0].node)->nextList,tac_count);
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
#line 1858 "y.tab.c"
    break;

  case 3: /* stmt_list: stmt_list M stmt  */
#line 533 "prob.y"
                          {
            if(!error_flag)
            {

            // printList($1->nextList, "$1->nextList before backpatch");
            backpatch((yyvsp[-2].node)->nextList,(yyvsp[-1].instr));
          //  printSymbolTables();
            Node* temp = createNode("",NULL,NULL,(yyvsp[0].node)->nextList,"");
            (yyval.node) = temp;
            }
        
        }
#line 1875 "y.tab.c"
    break;

  case 4: /* stmt_list: stmt  */
#line 545 "prob.y"
                 { 
        if(!error_flag){
            Node* temp = createNode("",NULL,NULL,(yyvsp[0].node)->nextList,"");
            (yyval.node) = temp;

        } 
    }
#line 1887 "y.tab.c"
    break;

  case 5: /* stmt: DECLARE_STMT ';'  */
#line 555 "prob.y"
                    {
    if(!error_flag)
    {
        Node* temp = createNode("",NULL,NULL,NULL,"");
                        (yyval.node) = temp;
    }
                          
  }
#line 1900 "y.tab.c"
    break;

  case 6: /* stmt: ASSIGN_STMT ';'  */
#line 563 "prob.y"
                    {        if(!error_flag)
  {  
                           (yyval.node) = (yyvsp[-1].node);  
  }    
                          }
#line 1910 "y.tab.c"
    break;

  case 7: /* stmt: CONTROL_FLOW_STMT  */
#line 569 "prob.y"
                      { if(!error_flag) {(yyval.node) = (yyvsp[0].node);} }
#line 1916 "y.tab.c"
    break;

  case 8: /* stmt: LB Q stmt_list RB  */
#line 570 "prob.y"
                       {  if(!error_flag)
                        {
                        //printf("hi\n");
                        // symbolTableArray[symbolTableCount] = currentSymbolTable;
                        //   //  printSymbolTable(currentSymbolTable);
                        // symbolTableCount++;
                        currentSymbolTable = popScope(scopeStack);
                        offset = popOffset(offsetStack);

                        Node* temp = createNode("",NULL,NULL,(yyvsp[-1].node)->nextList,"");
                        (yyval.node) = temp;
                        }
                      }
#line 1934 "y.tab.c"
    break;

  case 9: /* stmt: LB RB  */
#line 583 "prob.y"
          { 
        if(!error_flag){
            Node* temp = createNode("",NULL,NULL,NULL,"");
            (yyval.node) = temp;
        }
    }
#line 1945 "y.tab.c"
    break;

  case 10: /* stmt: expr ';'  */
#line 589 "prob.y"
             {if(!error_flag) {(yyval.node)=(yyvsp[-1].node);}}
#line 1951 "y.tab.c"
    break;

  case 11: /* stmt: error  */
#line 590 "prob.y"
          { if(!error_flag) {printf("Rejected - Invalid Expression\n");error_flag=1;yyerrok;}}
#line 1957 "y.tab.c"
    break;

  case 12: /* DECLARE_STMT: B A comp  */
#line 594 "prob.y"
                        { 
                   if(!error_flag)
                   {
                    
                   for(int i =0;i<var_count;i++)
                   {   
                     // printf("%s %d\n",$3->type,bracError);
                      char aa[5];
                      substring((yyvsp[0].type)->type, aa, 0, 5);
                     // printf("%s\n",aa);
                      int yy = strcmp(aa,"array");
                     // printf("%d\n",var_list_brac[i]);
                     // printf("%s\n",var_list[i]);
                      if(var_list_brac[i] == 1  && !strcmp(aa,"array"))
                      { 
                        //printf("hi\n");
                        error_flag = 1;
                        printf("error : Not Assignable\n");
                        bracError = 0;
                        break;
                      }
                       if(insertSymbol(currentSymbolTable,var_list[i], (yyvsp[0].type)->type , offset) == 1) 
                       {
                        if(!error_flag)
                        {
                        error_flag = 1;
                        printf("error : redeclaration of '%s'\n",var_list[i]);
                        }
                       }
                       else if(insertSymbol(currentSymbolTable,var_list[i], (yyvsp[0].type)->type , offset) == 2)
                       {
                         error_flag = 1;
                        printf("error : conflicting types for '%s'\n",var_list[i]);
                       }
                      // printSymbolTable(currentSymbolTable);
                       offset= offset + (yyvsp[0].type)->width;
                      // printf("%d",offset);
                      // printf("%s ",var_list[i] );
                       strcpy(var_list[i],"");
                    
                   }}
                   var_count =0;
  dummy = createType("",-1);}
#line 2005 "y.tab.c"
    break;

  case 14: /* comp: LS NUMBER RS comp  */
#line 644 "prob.y"
                        {

        if(!error_flag) {
            
            int num = atoi((yyvsp[-2].value));
            int width = num*(yyvsp[0].type)->width;
           // printf("%d",width);
            char value[1000]; sprintf(value,"array(%d,%s)",num,(yyvsp[0].type)->type);
            (yyval.type) = createType(value,width);
            // if(bracError == 1)
            // {
            //     error_flag = 1;
            //     printf("%s",value);
            //     printf("error : Not Assignable\n");
            //     bracError = 0;
            // }
        }
    }
#line 2028 "y.tab.c"
    break;

  case 15: /* comp: %empty  */
#line 662 "prob.y"
      { (yyval.type) = createType(dummy->type,dummy->width); }
#line 2034 "y.tab.c"
    break;

  case 16: /* A: A ',' IDENTIFIER ASSIGN expr  */
#line 664 "prob.y"
                                 { sprintf(var_list[var_count],"%s",(yyvsp[-2].value)); var_list_brac[var_count] = 1;  var_count++; 
                                    
                                    
            char* t = gen_label();
            if(strcmp(dummy->type,(yyvsp[0].node)->type) != 0){
                sprintf(tac[tac_count],"%s = (%s) %s\n", t, dummy->type, (yyvsp[0].node));
                var_list_brac[var_count] = 0;
                tac_count++;
            }else{
                sprintf(tac[tac_count],"%s = %s\n", t, (yyvsp[0].node));
                 var_list_brac[var_count] = 0;
                tac_count++;
            }
            sprintf(tac[tac_count],"%s = %s\n", (yyvsp[-2].value), t);
            var_list_brac[var_count] = 0;
            tac_count++;
                       
                       
                                    
                                    }
#line 2059 "y.tab.c"
    break;

  case 17: /* A: A ',' IDENTIFIER  */
#line 684 "prob.y"
                      { sprintf(var_list[var_count],"%s",(yyvsp[0].value));var_list_brac[var_count] = 0;  var_count++; }
#line 2065 "y.tab.c"
    break;

  case 18: /* A: IDENTIFIER  */
#line 685 "prob.y"
                 { sprintf(var_list[var_count],"%s",(yyvsp[0].value)); var_list_brac[var_count] = 0;var_count++; 
                       }
#line 2072 "y.tab.c"
    break;

  case 19: /* A: IDENTIFIER ASSIGN expr  */
#line 687 "prob.y"
                             { sprintf(var_list[var_count],"%s",(yyvsp[-2].value)); var_list_brac[var_count] = 1;  var_count++; 
            char* t = gen_label();
            if(strcmp(dummy->type,(yyvsp[0].node)->type) != 0){
                sprintf(tac[tac_count],"%s = (%s) %s\n", t, dummy->type, (yyvsp[0].node));
                var_list_brac[var_count] = 0;
                tac_count++;
            }else{
                sprintf(tac[tac_count],"%s = %s\n", t, (yyvsp[0].node));
                 var_list_brac[var_count] = 0;
                tac_count++;
            }
            sprintf(tac[tac_count],"%s = %s\n", (yyvsp[-2].value), t);
            var_list_brac[var_count] = 0;
            tac_count++;
                       
                       
    
    
    }
#line 2096 "y.tab.c"
    break;

  case 20: /* CONTROL_FLOW_STMT: IF '(' expr ')' M stmt  */
#line 709 "prob.y"
                                             {
                                  if(!error_flag)
                               {        backpatch((yyvsp[-3].node)->trueList,(yyvsp[-1].instr));
                                  Node* temp = createNode("",NULL,NULL,merge((yyvsp[-3].node)->falseList,(yyvsp[0].node)->nextList),"");
                                  (yyval.node) = temp;
                                  }
                               }
#line 2108 "y.tab.c"
    break;

  case 21: /* CONTROL_FLOW_STMT: IF '(' expr ')' M stmt ELSE N M stmt  */
#line 717 "prob.y"
                                      {
                                       if(!error_flag)
                                      {
                                       backpatch((yyvsp[-7].node)->trueList,(yyvsp[-5].instr));
                                       backpatch((yyvsp[-7].node)->falseList,(yyvsp[-1].instr));
                                       Node* _temp = createNode("",NULL,NULL,merge((yyvsp[-4].node)->nextList,makelist((yyvsp[-2].instr))),"");
                                       Node* temp = createNode("",NULL,NULL,merge(_temp->nextList,(yyvsp[0].node)->nextList),"");
                                       (yyval.node) = temp;
                                       }
                                      }
#line 2123 "y.tab.c"
    break;

  case 22: /* CONTROL_FLOW_STMT: WHILE M '(' expr ')' M stmt  */
#line 727 "prob.y"
                                {
                                  if(!error_flag)
                              {
                               backpatch((yyvsp[0].node)->nextList,(yyvsp[-5].instr));
                               backpatch((yyvsp[-3].node)->trueList,(yyvsp[-1].instr));
                               Node* temp = createNode("",NULL,NULL,(yyvsp[-3].node)->falseList,"");
                               (yyval.node) = temp;
                               add_tac("goto %d\n",(yyvsp[-5].instr));
                               }
                               }
#line 2138 "y.tab.c"
    break;

  case 23: /* CONTROL_FLOW_STMT: IF '(' ')'  */
#line 737 "prob.y"
               {if(!error_flag){
            printf("Rejected - empty bool expr inside if\n");
            error_flag = 1;
        }}
#line 2147 "y.tab.c"
    break;

  case 24: /* ASSIGN_STMT: expr ASSIGN expr  */
#line 744 "prob.y"
                                { if(!error_flag)
    				  {
                                      if((yyvsp[-2].node)->notassign)
                                      {
                                        printf("Rejected-cannot assign to a postfix expression\n");
                                        error_flag = 1;
                                      }    
                                      else
                                      {
                                if(is_number((yyvsp[-2].node)->value))
                                {
                                    error_flag = 1;
                                    printf("Rejected-cannot assign to a number\n");
                                }
                                if(strcmp((yyvsp[-1].node)->value, "=") == 0)
                                 {
                                
                                        if(strcmp((yyvsp[-2].node)->type,(yyvsp[0].node)->type) != 0){
                                                  //  sprintf(intermediate_code[idx++],"%s = (%s) %s\n", t, $1->type, $3);
                                                   char* t = gen_label();
                                                   add_tac("%s = (%s) %s\n", t, (yyvsp[-2].node)->type, (yyvsp[0].node));
                                                    add_tac("%s = %s\n", (yyvsp[-2].node), t);
                                          }
                                    else{
                                    add_tac("%s = %s\n", (yyvsp[-2].node), (yyvsp[0].node)->value);
                                    }
                                 }
                                 else if(strcmp((yyvsp[-1].node)->value, "+=") == 0){
                                    
                                        if(strcmp((yyvsp[-2].node)->type,(yyvsp[0].node)->type) != 0){
                                            char* t = gen_label();
                                                   add_tac("%s = (%s) %s\n", t, (yyvsp[-2].node)->type, (yyvsp[0].node));
                                                 //  char *new = new_temp();
                                    
                                    char *new1 = new_temp();
                                    add_tac("%s = %s + %s\n", new1, (yyvsp[-2].node),t);
                                    add_tac("%s = %s\n",(yyvsp[-2].node), new1);
                                          }
                                    else{
                                    char *new = new_temp();
                                    add_tac("%s = %s\n", new, (yyvsp[0].node)->value);
                                    char *new1 = new_temp();
                                    add_tac("%s = %s + %s\n", new1, (yyvsp[-2].node),new);
                                    add_tac("%s = %s\n",(yyvsp[-2].node), new1);
                                    }
                                 }
                                 else if(strcmp((yyvsp[-1].node)->value, "-=") == 0){
                                    
                                        if(strcmp((yyvsp[-2].node)->type,(yyvsp[0].node)->type) != 0){
                                            char* t = gen_label();
                                                   add_tac("%s = (%s) %s\n", t, (yyvsp[-2].node)->type, (yyvsp[0].node));
                                                 //   add_tac("%s = %s\n", $1, t);
                                                  //   char *new = new_temp();
                                  //  add_tac("%s = %s\n", new, t);
                                     char *new1 = new_temp();
                                    add_tac("%s = %s - %s\n", new1, (yyvsp[-2].node),t);
                                    add_tac("%s = %s\n",(yyvsp[-2].node), new1);
                                          }
                                    else{
                                    char *new = new_temp();
                                    add_tac("%s = %s\n", new, (yyvsp[0].node)->value);
                                     char *new1 = new_temp();
                                    add_tac("%s = %s - %s\n", new1, (yyvsp[-2].node),new);
                                    add_tac("%s = %s\n",(yyvsp[-2].node), new1);
                                    }
                                 }
                                 else if(strcmp((yyvsp[-1].node)->value, "*=") == 0){
                                    
                                        if(strcmp((yyvsp[-2].node)->type,(yyvsp[0].node)->type) != 0){
                                            char* t = gen_label();
                                                   add_tac("%s = (%s) %s\n", t, (yyvsp[-2].node)->type, (yyvsp[0].node));
                                                //    add_tac("%s = %s\n", $1, t);
                                                    // char *new = new_temp();
                                   // add_tac("%s = %s\n", new, t);
                                     char *new1 = new_temp();
                                    add_tac("%s = %s * %s\n", new1, (yyvsp[-2].node),t);
                                    add_tac("%s = %s\n",(yyvsp[-2].node), new1);
                                          }
                                    else{
                                    char *new = new_temp();
                                    add_tac("%s = %s\n", new, (yyvsp[0].node)->value);
                                     char *new1 = new_temp();
                                    add_tac("%s = %s * %s\n", new1, (yyvsp[-2].node),new);
                                    add_tac("%s = %s\n",(yyvsp[-2].node), new1);
                                    }
                                 }
                                 else{
                                  
                                        if(strcmp((yyvsp[-2].node)->type,(yyvsp[0].node)->type) != 0){
                                               char* t = gen_label();
                                                   add_tac("%s = (%s) %s\n", t, (yyvsp[-2].node)->type, (yyvsp[0].node));
                                                  //  add_tac("%s = %s\n", $1, t);
                                                  //  char *new = new_temp();
                                  //  add_tac("%s = %s\n", new, t);
                                     char *new1 = new_temp();
                                    add_tac("%s = %s / %s\n", new1, (yyvsp[-2].node),t);
                                    add_tac("%s = %s\n",(yyvsp[-2].node), new1);
                                          }
                                    else{      
                                    char *new = new_temp();
                                    add_tac("%s = %s\n", new, (yyvsp[0].node)->value);
                                     char *new1 = new_temp();
                                    add_tac("%s = %s / %s\n", new1, (yyvsp[-2].node),new);
                                    add_tac("%s = %s\n",(yyvsp[-2].node), new1);
                                    }
                                 }
                                
                                  Node* _temp = createNode("",NULL,NULL,NULL,"");
                                  (yyval.node) = _temp;
                                
                                 
                                      }
    
                                 } }
#line 2266 "y.tab.c"
    break;

  case 25: /* B: INT  */
#line 863 "prob.y"
       {dummy=(yyval.type) = createType((yyvsp[0].value), 4);}
#line 2272 "y.tab.c"
    break;

  case 26: /* B: FLOAT  */
#line 864 "prob.y"
           {dummy=(yyval.type) = createType((yyvsp[0].value), 4);}
#line 2278 "y.tab.c"
    break;

  case 27: /* B: CHAR  */
#line 865 "prob.y"
           {dummy=(yyval.type) = createType((yyvsp[0].value), 1);}
#line 2284 "y.tab.c"
    break;

  case 28: /* ASSIGN: EQ  */
#line 869 "prob.y"
           { (yyval.node) = createNode(strdup("="), NULL, NULL, NULL,""); }
#line 2290 "y.tab.c"
    break;

  case 29: /* ASSIGN: PEQ  */
#line 870 "prob.y"
            { (yyval.node) = createNode(strdup("+="), NULL, NULL, NULL,""); }
#line 2296 "y.tab.c"
    break;

  case 30: /* ASSIGN: MEQ  */
#line 871 "prob.y"
            { (yyval.node) = createNode(strdup("-="), NULL, NULL, NULL,""); }
#line 2302 "y.tab.c"
    break;

  case 31: /* ASSIGN: MLEQ  */
#line 872 "prob.y"
             { (yyval.node) = createNode(strdup("*="), NULL, NULL, NULL,""); }
#line 2308 "y.tab.c"
    break;

  case 32: /* ASSIGN: DEQ  */
#line 873 "prob.y"
             { (yyval.node) = createNode(strdup("/="), NULL, NULL, NULL,""); }
#line 2314 "y.tab.c"
    break;

  case 33: /* expr: expr '+' expr  */
#line 877 "prob.y"
                  { //printf("hi +\n");
                     if(!error_flag)
                      {char * t = max((yyvsp[-2].node)->type,(yyvsp[0].node)->type);
                        char *a1 = widen((yyvsp[-2].node)->value, (yyvsp[-2].node)->type,t);
                        char* a2 = widen ((yyvsp[0].node)->value,(yyvsp[0].node)->type,t);
                    char* label = new_temp();
                    add_tac("%s = %s + %s\n", label, a1, a2);
                    (yyval.node) = createNode(label, NULL, NULL, NULL,t);
                    }
                   }
#line 2329 "y.tab.c"
    break;

  case 34: /* expr: expr '-' expr  */
#line 887 "prob.y"
                  { //printf("hi -\n");
                      if(!error_flag)
            {   char * t = max((yyvsp[-2].node)->type,(yyvsp[0].node)->type);
                        char *a1 = widen((yyvsp[-2].node)->value, (yyvsp[-2].node)->type,t);
                        char* a2 = widen ((yyvsp[0].node)->value,(yyvsp[0].node)->type,t);
                char* label = new_temp();
                    add_tac("%s = %s - %s\n", label, a1,a2);
                    (yyval.node) = createNode(label, NULL, NULL, NULL,t);
                    }
                   }
#line 2344 "y.tab.c"
    break;

  case 35: /* expr: expr '*' expr  */
#line 897 "prob.y"
                  {  //printf("hi *\n");
                     if(!error_flag)
            {  char * t = max((yyvsp[-2].node)->type,(yyvsp[0].node)->type);
                        char *a1 = widen((yyvsp[-2].node)->value, (yyvsp[-2].node)->type,t);
                        char* a2 = widen ((yyvsp[0].node)->value,(yyvsp[0].node)->type,t);
                char* label = new_temp();
                    add_tac("%s = %s * %s\n", label, a1,a2);
                    (yyval.node) = createNode(label, NULL, NULL, NULL,t);
                    }
                   }
#line 2359 "y.tab.c"
    break;

  case 36: /* expr: expr '/' expr  */
#line 907 "prob.y"
                   {//printf("hi /\n");
                    if(!error_flag)
            {   char * t = max((yyvsp[-2].node)->type,(yyvsp[0].node)->type);
                        char *a1 = widen((yyvsp[-2].node)->value, (yyvsp[-2].node)->type,t);
                        char* a2 = widen ((yyvsp[0].node)->value,(yyvsp[0].node)->type,t);
                 char* label = new_temp();
                    add_tac("%s = %s / %s\n", label, a1,a2);
                    (yyval.node) = createNode(label, NULL, NULL, NULL,t);
                   }
                   }
#line 2374 "y.tab.c"
    break;

  case 37: /* expr: expr '%' expr  */
#line 917 "prob.y"
                   { //printf("hi %\n");
                     if(!error_flag)
            {   char * t = max((yyvsp[-2].node)->type,(yyvsp[0].node)->type);
                        char *a1 = widen((yyvsp[-2].node)->value, (yyvsp[-2].node)->type,t);
                        char* a2 = widen ((yyvsp[0].node)->value,(yyvsp[0].node)->type,t);
                char* label = new_temp();
                    add_tac("%s = %s %% %s\n", label, a1,a2);
                    (yyval.node) = createNode(label, NULL, NULL, NULL,t);
                    }
                   }
#line 2389 "y.tab.c"
    break;

  case 38: /* expr: '(' expr ')'  */
#line 927 "prob.y"
                   {// printf("hi (\n"); 
                       if(!error_flag)
                        {  Node* temp = createNode((yyvsp[-1].node)->value,(yyvsp[-1].node)->trueList,(yyvsp[-1].node)->falseList,NULL,(yyvsp[-1].node)->type);
                        (yyval.node) = temp;}
                        }
#line 2399 "y.tab.c"
    break;

  case 39: /* expr: expr GT expr  */
#line 932 "prob.y"
                  { //printf("hi %\n");
                     if(!error_flag)
            {Node* temp = createNode("",makelist(tac_count),makelist(tac_count+1),NULL,"");
                     add_tac("if %s %s %s goto ", (yyvsp[-2].node)->value, ">", (yyvsp[0].node)->value);
                     add_tac("goto ");
                     (yyval.node) = temp;
                     }
                   }
#line 2412 "y.tab.c"
    break;

  case 40: /* expr: expr LT expr  */
#line 940 "prob.y"
                  { //printf("hi %\n");
                     if(!error_flag)
                    { Node* temp = createNode("",makelist(tac_count),makelist(tac_count+1),NULL,"");
                     add_tac("if %s %s %s goto ", (yyvsp[-2].node)->value, "<", (yyvsp[0].node)->value);
                     add_tac("goto ");
                     (yyval.node) = temp;
                     }
                   }
#line 2425 "y.tab.c"
    break;

  case 41: /* expr: expr GTE expr  */
#line 948 "prob.y"
                   { //printf("hi %\n");
                    Node* temp = createNode("",makelist(tac_count),makelist(tac_count+1),NULL,"");
                     add_tac("if %s %s %s goto ", (yyvsp[-2].node)->value, ">=", (yyvsp[0].node)->value);
                     add_tac("goto ");
                     (yyval.node) = temp;
                   }
#line 2436 "y.tab.c"
    break;

  case 42: /* expr: expr LTE expr  */
#line 954 "prob.y"
                   { //printf("hi %\n");
                    if(!error_flag)
            { Node* temp = createNode("",makelist(tac_count),makelist(tac_count+1),NULL,"");
                     add_tac("if %s %s %s goto ", (yyvsp[-2].node)->value, "<=", (yyvsp[0].node)->value);
                     add_tac("goto ");
                     (yyval.node) = temp;}
                   }
#line 2448 "y.tab.c"
    break;

  case 43: /* expr: expr NE expr  */
#line 961 "prob.y"
                  { 
                    if(!error_flag)
            { Node* temp = createNode("",makelist(tac_count),makelist(tac_count+1),NULL,"");
                     add_tac("if %s %s %s goto ", (yyvsp[-2].node)->value, "!=", (yyvsp[0].node)->value);
                     add_tac("goto ");
                     (yyval.node) = temp;
                     }
                   }
#line 2461 "y.tab.c"
    break;

  case 44: /* expr: expr EE expr  */
#line 969 "prob.y"
                  {   if(!error_flag)
            {Node* temp = createNode("",makelist(tac_count),makelist(tac_count+1),NULL,"");
                     add_tac("if %s %s %s goto ", (yyvsp[-2].node)->value, "==", (yyvsp[0].node)->value);
                     add_tac("goto ");
                     (yyval.node) = temp;}
                   }
#line 2472 "y.tab.c"
    break;

  case 45: /* expr: expr AND M expr  */
#line 975 "prob.y"
                    { 
                          if(!error_flag)
            { Node* temp = createNode("",(yyvsp[0].node)->trueList,merge((yyvsp[-3].node)->falseList,(yyvsp[0].node)->falseList),NULL,"");
                          backpatch((yyvsp[-3].node)->trueList,(yyvsp[-1].instr));
                          (yyval.node) = temp;}
                     }
#line 2483 "y.tab.c"
    break;

  case 46: /* expr: expr OR N expr  */
#line 981 "prob.y"
                    { 
                       if(!error_flag)
            {Node* temp = createNode("",merge((yyvsp[-3].node)->trueList,(yyvsp[0].node)->trueList),(yyvsp[0].node)->falseList,NULL,"");
                      backpatch((yyvsp[-3].node)->falseList,(yyvsp[-1].instr));
                      (yyval.node) = temp;}
                    }
#line 2494 "y.tab.c"
    break;

  case 47: /* expr: NN expr  */
#line 987 "prob.y"
            {
   
          if(!error_flag)
            {Node* temp = createNode("",(yyvsp[0].node)->falseList,(yyvsp[0].node)->trueList,NULL,"");
                      (yyval.node) = temp;}
      }
#line 2505 "y.tab.c"
    break;

  case 48: /* expr: '(' expr error  */
#line 993 "prob.y"
                   { printf("Rejected - Right paranthesis missing\n"); error_flag = 1; yyerror("Rejected - Invalid operand for modulus operator");  yyerrok;  }
#line 2511 "y.tab.c"
    break;

  case 49: /* expr: term  */
#line 995 "prob.y"
         { (yyval.node)=(yyvsp[0].node) ;}
#line 2517 "y.tab.c"
    break;

  case 50: /* expr: T  */
#line 996 "prob.y"
      {  if(!error_flag)
            {Node* temp = createNode("",makelist(tac_count),NULL,NULL,"");
            sprintf(tac[tac_count++], "goto ");
            (yyval.node) = temp;}}
#line 2526 "y.tab.c"
    break;

  case 51: /* expr: F  */
#line 1000 "prob.y"
     {  if(!error_flag)
            {Node* temp = createNode("",NULL,makelist(tac_count),NULL,"");
            sprintf(tac[tac_count++], "goto ");
            (yyval.node) = temp;}}
#line 2535 "y.tab.c"
    break;

  case 52: /* N: %empty  */
#line 1007 "prob.y"
   {(yyval.instr) = tac_count; char *dummy1 = "goto "; add_tac("%s",dummy1); }
#line 2541 "y.tab.c"
    break;

  case 53: /* M: %empty  */
#line 1008 "prob.y"
   {(yyval.instr) = tac_count;}
#line 2547 "y.tab.c"
    break;

  case 54: /* O: %empty  */
#line 1009 "prob.y"
    {
        // init a scope stack and an offset stack 
        scopeStack = createScopeStack();
        offsetStack = createOffsetStack();
    }
#line 2557 "y.tab.c"
    break;

  case 55: /* P: %empty  */
#line 1014 "prob.y"
    {   
        // init the offset and a symbol table 
        offset = 0; currentSymbolTable = createSymbolTable();
        symbolTableArray[symbolTableCount] = currentSymbolTable;
                        symbolTableCount++;
    }
#line 2568 "y.tab.c"
    break;

  case 56: /* Q: %empty  */
#line 1020 "prob.y"
    {  

        pushScope(scopeStack, currentSymbolTable);
        currentSymbolTable = createSymbolTable();
        pushOffset(offsetStack, offset);
        offset = 0;
        symbolTableArray[symbolTableCount] = currentSymbolTable;
                          //  printSymbolTable(currentSymbolTable);
                        symbolTableCount++;
    }
#line 2583 "y.tab.c"
    break;

  case 57: /* term: UM IDENTIFIER  */
#line 1030 "prob.y"
                     {
                        if(!error_flag)
                       {
                       if(checkSymbolExistsInAnyTable((yyvsp[0].value)) == NULL){
                                printf("error : var ' %s ' is not declared in the scope\n",(yyvsp[0].value)); error_flag = 1;
                            }
                        else{
                       char* label = new_temp();
                       add_tac("%s = -%s\n", label, (yyvsp[0].value));
                       Node* temp = createNode(label,NULL,NULL,NULL,checkSymbolExistsInAnyTable((yyvsp[0].value))->type);
                       (yyval.node) = temp;
                        }
                       }
                       

                     }
#line 2604 "y.tab.c"
    break;

  case 58: /* term: IDENTIFIER  */
#line 1047 "prob.y"
                   {
                        if(!error_flag)
                       { 
                            if(checkSymbolExistsInAnyTable((yyvsp[0].value)) == NULL){
                                printf("error : var ' %s ' is not declared in the scope\n",(yyvsp[0].value)); error_flag = 1;
                            }
                           else{
                         Node* temp = createNode((yyvsp[0].value),NULL,NULL,NULL,checkSymbolExistsInAnyTable((yyvsp[0].value))->type);
                         (yyval.node) = temp;
                           }
                         
                         }
                       
                     }
#line 2623 "y.tab.c"
    break;

  case 59: /* term: UM DOUBLE IDENTIFIER  */
#line 1063 "prob.y"
                           {    if(!error_flag)
                              {
                                if(checkSymbolExistsInAnyTable((yyvsp[0].value)) == NULL){
                                printf("error : var ' %s ' is not declared in the scope\n",(yyvsp[0].value)); error_flag = 1;
                            }
                               else {
                                 char* label = new_temp();
                                 if(strcmp((yyvsp[-1].node)->value, "--") == 0)
                                 {
                                   printf("Rejected - Expression is not assignable\n");   error_flag=1;
                                 }
                                 else
                                 {
                                 char* new = new_temp();
                                 char* new1 = new_temp();
                                   add_tac("%s = %s + 1\n",new1, (yyvsp[0].value));
                                   add_tac("%s = %s\n", new, (yyvsp[0].value));
                                   add_tac("%s = -%s\n",label,new);
                                   add_tac("%s = %s\n",(yyvsp[0].value), new1);
                                   Node* temp = createNode(label,NULL,NULL,NULL,checkSymbolExistsInAnyTable((yyvsp[0].value))->type);
                                   (yyval.node) = temp;
                                   
                                 }
                               
                               (yyval.node)->notassign = true;
                              }
                               }
                           }
#line 2656 "y.tab.c"
    break;

  case 60: /* term: DOUBLE IDENTIFIER  */
#line 1094 "prob.y"
                           {    if(!error_flag)
                               {
                                if(checkSymbolExistsInAnyTable((yyvsp[0].value)) == NULL){
                                printf("error : var ' %s ' is not declared in the scope\n",(yyvsp[0].value)); error_flag = 1;
                            }
                            else{
                                char* label = new_temp();
                                if(strcmp((yyvsp[-1].node)->value, "--") == 0)
                                 { char* new1 = new_temp();
                                   add_tac("%s = %s - 1\n", new1, (yyvsp[0].value));
                                   add_tac("%s = %s\n", label, (yyvsp[0].value));
                                   add_tac("%s = %s\n", (yyvsp[0].value), new1);
                                   Node* temp = createNode(label,NULL,NULL,NULL,checkSymbolExistsInAnyTable((yyvsp[0].value))->type);
                                   (yyval.node) = temp;
                                 }
                                 else
                                 { char* new1 = new_temp();
                                   add_tac("%s = %s + 1\n", new1, (yyvsp[0].value));
                                   add_tac("%s = %s\n", label, (yyvsp[0].value));
                                   add_tac("%s = %s\n", (yyvsp[0].value), new1);
                                   Node* temp = createNode(label,NULL,NULL,NULL,checkSymbolExistsInAnyTable((yyvsp[0].value))->type);
                                   (yyval.node) = temp; 
                                 }
                               (yyval.node)->notassign = true;
                               }
                               }
                           }
#line 2688 "y.tab.c"
    break;

  case 61: /* term: UM IDENTIFIER DOUBLE  */
#line 1123 "prob.y"
                           {   
                            if(!error_flag)
                           {     if(checkSymbolExistsInAnyTable((yyvsp[-1].value)) == NULL){
                                printf("error : var ' %s ' is not declared in the scope\n",(yyvsp[-1].value)); error_flag = 1;
                            }       
                            else{     
                                 char* label = new_temp();
                                 if(strcmp((yyvsp[0].node)->value, "--") == 0)
                                 {
                                   char* new = new_temp();
                                   char* new1 = new_temp();
                                   add_tac("%s = %s\n", new, (yyvsp[-1].value));
                                   add_tac("%s = -%s\n",label,new);
                                   add_tac("%s = %s - 1\n", new1, (yyvsp[-1].value));
                                   add_tac("%s = %s\n", (yyvsp[-1].value), new1);
                                   Node* temp = createNode(label,NULL,NULL,NULL,checkSymbolExistsInAnyTable((yyvsp[-1].value))->type);
                                   (yyval.node) = temp;
                                 }
                                 else
                                 {
                                   char* new = new_temp();
                                   char* new1 = new_temp();
                                   add_tac("%s = %s\n", new, (yyvsp[-1].value));
                                   add_tac("%s = -%s\n",label,new);
                                   add_tac("%s = %s + 1\n", new1, (yyvsp[-1].value));
                                   add_tac("%s = %s\n", (yyvsp[-1].value), new1);
                                   Node* temp = createNode(label,NULL,NULL,NULL,checkSymbolExistsInAnyTable((yyvsp[-1].value))->type);
                                   (yyval.node) = temp;
                                 }                                                             
                                (yyval.node)->notassign = true;
                            }
                             }
                           }
#line 2726 "y.tab.c"
    break;

  case 62: /* term: IDENTIFIER DOUBLE  */
#line 1160 "prob.y"
                           {    if(!error_flag)
                              {
                                if(checkSymbolExistsInAnyTable((yyvsp[-1].value)) == NULL){
                                printf("error : var ' %s ' is not declared in the scope\n",(yyvsp[-1].value)); error_flag = 1;
                            }
                            else
                            {
                                char* label = new_temp();
                                if(strcmp((yyvsp[0].node)->value, "--") == 0)
                                 {  char* new1 = new_temp();
                                   add_tac("%s = %s\n", label, (yyvsp[-1].value));
                                   add_tac("%s = %s - 1\n", new1, (yyvsp[-1].value));
                                   add_tac("%s = %s\n", (yyvsp[-1].value), new1);
                                   Node* temp = createNode(label,NULL,NULL,NULL,checkSymbolExistsInAnyTable((yyvsp[-1].value))->type);
                                   (yyval.node) = temp;
                                 }
                                 else
                                 { char* new1 = new_temp();
                                   add_tac("%s = %s\n", label, (yyvsp[-1].value));
                                   add_tac("%s = %s + 1\n", new1, (yyvsp[-1].value));
                                   add_tac("%s = %s\n", (yyvsp[-1].value), new1);    
                                   Node* temp = createNode(label,NULL,NULL,NULL,checkSymbolExistsInAnyTable((yyvsp[-1].value))->type);
                                   (yyval.node) = temp;                             
                                 }
                               
                                (yyval.node)->notassign = true;
                            }
                               }
                                
                           }
#line 2761 "y.tab.c"
    break;

  case 63: /* term: UM NUMBER  */
#line 1192 "prob.y"
                {
                    if(!error_flag)
                     {
                       Node* temp = createNode(strcat((yyvsp[-1].node)->value,(yyvsp[0].value)),NULL,NULL,NULL,"");
                        (yyval.node) = temp;          
                       }
                 }
#line 2773 "y.tab.c"
    break;

  case 64: /* term: NUMBER  */
#line 1199 "prob.y"
              {
                    if(!error_flag)
                       {    
                         Node* temp = createNode((yyvsp[0].value),NULL,NULL,NULL,"");
                         (yyval.node) = temp;
                       }    
                 }
#line 2785 "y.tab.c"
    break;

  case 65: /* term: UM DOUBLE IDENTIFIER DOUBLE  */
#line 1209 "prob.y"
                                    {printf("Rejected - Expression is not assignable\n");   error_flag=1; }
#line 2791 "y.tab.c"
    break;

  case 66: /* term: UM PP NUMBER  */
#line 1210 "prob.y"
                    {printf("Rejected - cannot increment a constant value\n");   error_flag=1; }
#line 2797 "y.tab.c"
    break;

  case 67: /* term: UM MM NUMBER  */
#line 1211 "prob.y"
                    {printf("Rejected - cannot decrement a constant value\n");   error_flag=1; }
#line 2803 "y.tab.c"
    break;

  case 68: /* term: UM NUMBER PP  */
#line 1212 "prob.y"
                    {printf("Rejected - cannot increment a constant value\n");   error_flag=1; }
#line 2809 "y.tab.c"
    break;

  case 69: /* term: UM NUMBER MM  */
#line 1213 "prob.y"
                    {printf("Rejected - cannot decrement a constant value\n");   error_flag=1; }
#line 2815 "y.tab.c"
    break;

  case 70: /* term: UM DOUBLE IDENTIFIER IDENTIFIER  */
#line 1214 "prob.y"
                                       {printf("Rejected - Missing operator\n"); error_flag=1;}
#line 2821 "y.tab.c"
    break;

  case 71: /* term: UM IDENTIFIER DOUBLE IDENTIFIER  */
#line 1215 "prob.y"
                                       {printf("Rejected - Missing operator\n"); error_flag=1;}
#line 2827 "y.tab.c"
    break;

  case 72: /* term: UM DOUBLE IDENTIFIER NUMBER  */
#line 1216 "prob.y"
                                   {printf("Rejected - Missing operator\n");  error_flag=1;}
#line 2833 "y.tab.c"
    break;

  case 73: /* term: UM IDENTIFIER DOUBLE NUMBER  */
#line 1217 "prob.y"
                                   {printf("Rejected - Missing operator\n");   error_flag=1;}
#line 2839 "y.tab.c"
    break;

  case 74: /* term: UM NUMBER IDENTIFIER  */
#line 1218 "prob.y"
                            {printf("Rejected - Missing operator\n");  error_flag=1;}
#line 2845 "y.tab.c"
    break;

  case 75: /* term: UM IDENTIFIER NUMBER  */
#line 1219 "prob.y"
                            {printf("Rejected - Missing operator\n");  error_flag=1;}
#line 2851 "y.tab.c"
    break;

  case 76: /* term: UM IDENTIFIER DOUBLE DOUBLE  */
#line 1220 "prob.y"
                                   {printf("Rejected - Expression is not assignable\n"); error_flag=1;}
#line 2857 "y.tab.c"
    break;

  case 77: /* term: DOUBLE IDENTIFIER DOUBLE  */
#line 1221 "prob.y"
                                 {printf("Rejected - Expression is not assignable\n");   error_flag=1; }
#line 2863 "y.tab.c"
    break;

  case 78: /* term: PP NUMBER  */
#line 1222 "prob.y"
                 {printf("Rejected - cannot increment a constant value\n");   error_flag=1; }
#line 2869 "y.tab.c"
    break;

  case 79: /* term: MM NUMBER  */
#line 1223 "prob.y"
                 {printf("Rejected - cannot decrement a constant value\n");   error_flag=1; }
#line 2875 "y.tab.c"
    break;

  case 80: /* term: NUMBER PP  */
#line 1224 "prob.y"
                 {printf("Rejected - cannot increment a constant value\n");   error_flag=1; }
#line 2881 "y.tab.c"
    break;

  case 81: /* term: NUMBER MM  */
#line 1225 "prob.y"
                 {printf("Rejected - cannot decrement a constant value\n");   error_flag=1; }
#line 2887 "y.tab.c"
    break;

  case 82: /* term: DOUBLE IDENTIFIER IDENTIFIER  */
#line 1226 "prob.y"
                                    {printf("Rejected - Missing operator\n"); error_flag=1;}
#line 2893 "y.tab.c"
    break;

  case 83: /* term: IDENTIFIER DOUBLE IDENTIFIER  */
#line 1227 "prob.y"
                                    {printf("Rejected - Missing operator\n"); error_flag=1;}
#line 2899 "y.tab.c"
    break;

  case 84: /* term: DOUBLE IDENTIFIER NUMBER  */
#line 1228 "prob.y"
                                {printf("Rejected - Missing operator\n");  error_flag=1;}
#line 2905 "y.tab.c"
    break;

  case 85: /* term: IDENTIFIER DOUBLE NUMBER  */
#line 1229 "prob.y"
                                {printf("Rejected - Missing operator\n");   error_flag=1;}
#line 2911 "y.tab.c"
    break;

  case 86: /* term: NUMBER IDENTIFIER  */
#line 1230 "prob.y"
                         {printf("Rejected - Missing operator\n");  error_flag=1;}
#line 2917 "y.tab.c"
    break;

  case 87: /* term: IDENTIFIER NUMBER  */
#line 1231 "prob.y"
                         {printf("Rejected - Missing operator\n");  error_flag=1;}
#line 2923 "y.tab.c"
    break;

  case 88: /* term: IDENTIFIER DOUBLE DOUBLE  */
#line 1232 "prob.y"
                                {printf("Rejected - Expression is not assignable\n"); error_flag=1;}
#line 2929 "y.tab.c"
    break;

  case 89: /* UM: '-'  */
#line 1235 "prob.y"
        {   Node* temp = createNode(strdup("-"),NULL,NULL,NULL,"");
           (yyval.node) = temp;
            }
#line 2937 "y.tab.c"
    break;

  case 90: /* DOUBLE: PP  */
#line 1238 "prob.y"
           {  
            Node* temp = createNode(strdup("++"),NULL,NULL,NULL,"");
           (yyval.node) = temp; }
#line 2945 "y.tab.c"
    break;

  case 91: /* DOUBLE: MM  */
#line 1241 "prob.y"
             {  Node* temp = createNode(strdup("--"),NULL,NULL,NULL,"");
           (yyval.node) = temp; }
#line 2952 "y.tab.c"
    break;


#line 2956 "y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 1246 "prob.y"


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

