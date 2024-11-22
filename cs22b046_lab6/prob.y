%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

int yylex();
void yyerror(const char *s);
extern FILE *yyin;
int error_flag = 0;

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

%}

%union {
    char* sval;   // For storing string values like identifiers and numbers
}

%token <sval> IDENTIFIER NUMBER EQ PEQ MEQ MLEQ DEQ MM PP
%type <sval> expr 
%type <sval> term
%type <sval> UM
%type <sval> DOUBLE ASSIGN
%left '+' '-'
%left '*' '/' '%'
%nonassoc PP MM 
%nonassoc '(' ')'

%%

program:
    /* empty */
  | program stmt
  ;

stmt:
    IDENTIFIER ASSIGN expr ';' { if(!error_flag){printf("Accepted\n"); 
                                 // printf("%s - %s\n", $2, $1);
                                 if(strcmp($2, "=") == 0)
                                 {
                                    add_tac("%s = %s", $1, $3);
                                 }
                                 else if(strcmp($2, "+=") == 0){
                                    char *new = new_temp();
                                    add_tac("%s = %s", new, $3);
                                    char *new1 = new_temp();
                                    add_tac("%s = %s + %s", new1, $1,new);
                                    add_tac("%s = %s",$1, new1);
                                 }
                                 else if(strcmp($2, "-=") == 0){
                                    char *new = new_temp();
                                    add_tac("%s = %s", new, $3);
                                     char *new1 = new_temp();
                                    add_tac("%s = %s - %s", new1, $1,new);
                                    add_tac("%s = %s",$1, new1);
                                 }
                                 else if(strcmp($2, "*=") == 0){
                                    char *new = new_temp();
                                    add_tac("%s = %s", new, $3);
                                     char *new1 = new_temp();
                                    add_tac("%s = %s * %s", new1, $1,new);
                                    add_tac("%s = %s",$1, new1);
                                 }
                                 else{
                                    char *new = new_temp();
                                    add_tac("%s = %s", new, $3);
                                     char *new1 = new_temp();
                                    add_tac("%s = %s / %s", new1, $1,new);
                                    add_tac("%s = %s",$1, new1);
                                 }
                                 
                                 for (int i = 0; i < tac_count; i++) {
                                   printf("%s\n", tac[i]);
                                   }
                                 error_flag=0; } 
                                 tac_count = 0 ;
                                 temp_var_count = 0; error_flag=0;
                                 printf("\n"); }   
  | error ';' { if(!error_flag){printf("Rejected - Invalid expression\n");yyerror("Rejected - Invalid expression");} yyerrok; yyclearin;  tac_count = 0,printf("\n"); ;
                                   temp_var_count = 0;error_flag=0; }
  ;

ASSIGN: EQ { $$ = strdup("="); }
       |PEQ { $$ = strdup("+="); }
       |MEQ { $$ = strdup("-="); }
       |MLEQ { $$ = strdup("*="); }
       |DEQ  { $$ = strdup("/="); }
       ;

expr:
    expr '+' expr { //printf("hi +\n");
                    $$ = new_temp();
                    add_tac("%s = %s + %s", $$, $1, $3);
                   }
  | expr '-' expr { //printf("hi -\n");
                    $$ = new_temp();
                    add_tac("%s = %s - %s", $$, $1, $3);
                   }
  | expr '*' expr {  //printf("hi *\n");
                    $$ = new_temp();
                    add_tac("%s = %s * %s", $$, $1, $3);
                   }
  | expr '/' expr  {//printf("hi /\n");
                    $$ = new_temp();
                    add_tac("%s = %s / %s", $$, $1, $3);
                   }
  | expr '%' expr  { //printf("hi %\n");
                    $$ = new_temp();
                    add_tac("%s = %s %% %s", $$, $1, $3);
                   }
  | '(' expr ')'   {// printf("hi (\n"); 
                        $$ = $2 ;}
  | '(' expr error { printf("Rejected - Right paranthesis missing\n"); error_flag = 1; yyerror("Rejected - Invalid operand for modulus operator");  yyerrok;  } 
  | term { $$=$1 ;}
  ;
  

term: UM IDENTIFIER  {
                       if($1 != NULL && strcmp($1, "-") == 0)
                       {
                       $$ = new_temp();
                       add_tac("%s = -%s", $$, $2);
                       }
                       else
                       {
                         $$ = $2; 
                       }
                     }
     | UM DOUBLE IDENTIFIER 
                           {
                               if($1 != NULL && strcmp($1, "-") == 0)
                               {
                                 $$ = new_temp();
                                 if(strcmp($2, "--") == 0)
                                 {
                                   printf("Rejected - Expression is not assignable\n");   error_flag=1;
                                 }
                                 else
                                 {
                                 char* new = new_temp();
                                 char* new1 = new_temp();
                                   add_tac("%s = %s + 1",new1, $3);
                                   add_tac("%s = %s", new, $3);
                                   add_tac("%s = -%s",$$,new);
                                   add_tac("%s = %s",$3, new1);
                                 }
                               }
                               else
                               {
                                $$ = new_temp();
                                if(strcmp($2, "--") == 0)
                                 { char* new1 = new_temp();
                                   add_tac("%s = %s - 1", new1, $3);
                                   add_tac("%s = %s", $$, $3);
                                   add_tac("%s = %s", $3, new1);
                                 }
                                 else
                                 { char* new1 = new_temp();
                                   add_tac("%s = %s + 1", new1, $3);
                                   add_tac("%s = %s", $$, $3);
                                    add_tac("%s = %s", $3, new1);
                                   
                                 }
                               }
                               
                           }
     | UM IDENTIFIER DOUBLE 
                           {
                               if($1 != NULL && strcmp($1, "-") == 0)
                               {
                                 $$ = new_temp();
                                 if(strcmp($3, "--") == 0)
                                 {
                                   char* new = new_temp();
                                   char* new1 = new_temp();
                                   add_tac("%s = %s", new, $2);
                                   add_tac("%s = -%s",$$,new);
                                   add_tac("%s = %s - 1", new1, $2);
                                   add_tac("%s = %s", $2, new1);
                                 }
                                 else
                                 {
                                   char* new = new_temp();
                                   char* new1 = new_temp();
                                   add_tac("%s = %s", new, $2);
                                   add_tac("%s = -%s",$$,new);
                                   add_tac("%s = %s + 1", new1, $2);
                                   add_tac("%s = %s", $2, new1);
                                 }
                               }
                               else
                               {
                                $$ = new_temp();
                                if(strcmp($3, "--") == 0)
                                 {  char* new1 = new_temp();
                                   add_tac("%s = %s", $$, $2);
                                   add_tac("%s = %s - 1", new1, $2);
                                   add_tac("%s = %s", $2, new1);
                                 }
                                 else
                                 { char* new1 = new_temp();
                                   add_tac("%s = %s", $$, $2);
                                  add_tac("%s = %s + 1", new1, $2);
                                   add_tac("%s = %s", $2, new1);                                 
                                 }
                               }
                               
                           }
     | UM NUMBER{
                   
                       if($1 != NULL && strcmp($1, "-") == 0)
                       {
                       $$ = strcat($1,$2);
                       }
                       else
                       {
                         $$ = $2; 
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
     ;

UM: '-' { $$ = strdup("-"); }
   |  /* empty */ { $$ = "not needed"; }
;

DOUBLE: PP { $$ = strdup("++"); }
        | MM { $$ = strdup("--"); };



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

