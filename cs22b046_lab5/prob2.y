%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yylex();
void yyerror(const char *s);
extern FILE *yyin;
int error_flag = 0;
%}

%union {
    char* sval;   // For storing string values like identifiers and numbers
}

%token <sval> IDENTIFIER NUMBER

%type <sval> expr term factor

%left '+' '-'
%left '*' '/' '%'

%%

program:
    /* empty */
  | program stmt
  ;

stmt:
    IDENTIFIER '=' expr ';' { if(!error_flag){printf("Accepted\n");} error_flag=0; }
  | error ';' { printf("Rejected - Invalid expression\n");yyerror("Rejected - Invalid expression"); yyerrok; yyclearin;}
  ;

expr:
    expr '+' term { $$ = strdup($1); }
  | expr '-' term { $$ = strdup($1); }
  | term { $$ = strdup($1); }
  ;

term:
    term '*' factor { $$ = strdup($1); }
  | term '/' factor
  | term '%' factor {
      if (strchr($1, '.') || strchr($3, '.')) {
          printf("Rejected - Invalid operand for modulus operator\n");
          yyerror("Rejected - Invalid operand for modulus operator");
          error_flag=1;
          yyerrok; yyclearin;
      } else {
          int left = atoi($1);
          int right = atoi($3);
              $$ = strdup($1);
      }
  }
  | factor { $$ = strdup($1); }
  ;

factor:
    '(' expr ')' { $$ = strdup($2); }
  | '-' factor { $$ = strdup($2); }
  | IDENTIFIER { $$ = strdup($1); }
  | NUMBER { $$ = strdup($1); }
  | '(' expr error { printf("Rejected - Right paranthesis missing\n");yyerror("Rejected - Invalid expression"); yyerrok; yyclearin;}
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

