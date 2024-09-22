%{
#include <stdio.h>
#include <stdlib.h>	
#include <stdbool.h>

int yylex();
int yyerror(char *);
extern FILE *yyin;
%}

%token ONE ZERO

%%
a1 : s ';' {printf("Accepcted\n");} a1 | {printf("done\n");} ;
s : ZERO s1 ZERO | ONE s ONE | ;   
s1 : ONE s1 ONE | ZERO s ZERO ;
%%
int yyerror(char *)
{
printf("error\n");
}

int main(int argc,char *argv[])
{
    if(argc!=2)
    {
        exit(EXIT_FAILURE);
    }
    yyin = fopen(argv[1],"r");

    yyparse();
    return 0;
}
