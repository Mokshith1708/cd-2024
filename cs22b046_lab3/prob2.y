%{
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int yylex();
int yyerror(char *);
extern FILE *yyin;

char input_buffer[256]; // Buffer to store the input
%}

%token ADD NEG INT FLOAT IMAGINARY

%%

a1 : s ';' { 
        printf("Input: %s\n", input_buffer); // Print the input
        printf("Accepted\n"); 
        input_buffer[0] = '\0'; // Clear the buffer for the next input
    } a1
   | /* empty */ { printf("done\n"); }
   ;

s  : f1 s1 ADD s1 IMAGINARY
   | f1 s1 ADD IMAGINARY s1
   | f1 s1 NEG s1 IMAGINARY
   | f1 s1 NEG IMAGINARY s1
   ;

s1 : INT | FLOAT ;
f1 : NEG | /* empty */ ;
%%

int yyerror(char *s) {
    printf("Input: %s\n", input_buffer); // Print the input when an error occurs
    printf("error: %s\n", s);
    input_buffer[0] = '\0'; // Clear the buffer for the next input
    return 0;
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

