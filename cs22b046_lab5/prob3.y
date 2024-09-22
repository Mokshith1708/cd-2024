%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yylex();
void yyerror(const char *s);
extern FILE *yyin;
int error_flag = 0;


// Definition of Node structure for syntax tree
struct Node {
    char* value;
    struct Node* left;
    struct Node* right;
};

struct Node* createNode(char* value, struct Node* left, struct Node* right) {
   struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->value = strdup(value);
    node->left = left;
    node->right = right;
    return node;
}

void printPostfix(struct Node* root) {
    if (root == NULL) return;
    printPostfix(root->left);
    printPostfix(root->right);
    printf("%s ", root->value);
}

void freeTree(struct Node* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root->value);
    free(root);
}


%}

%union {
   struct Node* node;   // For storing syntax tree nodes
    char* sval;   // For storing string values like identifiers and numbers
}

%token <sval> IDENTIFIER NUMBER

%type <node> expr term factor

%left '+' '-'
%left '*' '/' '%'


%%

program:
    /* empty */
  | program stmt
  ;

stmt:
    IDENTIFIER '=' expr ';' { if(!error_flag){printf("Accepted\n");           
     struct Node* assignNode = createNode("=", createNode($1, NULL, NULL), $3);
            printPostfix(assignNode);
            printf("\n\n");
            freeTree(assignNode);} error_flag=0; }
  | error ';' { printf("Rejected - Invalid expression\n\n");yyerror("Rejected - Invalid expression"); yyerrok; yyclearin;}
  ;

expr:
    expr '+' term { $$ = createNode("+", $1, $3); }
  | expr '-' term {  $$ = createNode("-", $1, $3); }
  | term { $$ = $1; }
  ;

term:
    term '*' factor {  $$ = createNode("*", $1, $3); }
  | term '/' factor{
        $$ = createNode("/", $1, $3);
    }
  | term '%' factor {
      if (strchr($1->value, '.') || strchr($3->value, '.')) {
          printf("Rejected - Invalid operand for modulus operator\n\n");
          yyerror("Rejected - Invalid operand for modulus operator");
          error_flag=1;
          yyerrok; yyclearin;
      } else {
          $$ = createNode("%", $1, $3);
      }
  }
  | factor {  $$ = $1; }
  ;

factor:
    '(' expr ')' { $$ = $2; }
  | '-' factor { $$ = createNode("-", NULL, $2); }
  | IDENTIFIER {  $$ = createNode($1, NULL, NULL); }
  | NUMBER {  $$ = createNode($1, NULL, NULL); }
  | '(' expr error { printf("Rejected - Right paranthesis missing\n\n");yyerror("Rejected - Invalid expression"); yyerrok; yyclearin;}
  ;

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

