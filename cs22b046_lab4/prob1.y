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

%token ADD NEG INT FLOAT IMAGINARY I

%%

a1 : s a1
   | /* empty */ { printf("done\n"); }
   ;

s  : NEG s1 ADD IMAGINARY ';' { printf("Input: %s\n", input_buffer); printf("Accepted\n"); input_buffer[0] = '\0';}
   | NEG s1 NEG IMAGINARY ';' { printf("Input: %s\n", input_buffer); printf("Accepted\n"); input_buffer[0] = '\0';}
   | s1 ADD IMAGINARY ';' { printf("Input: %s\n", input_buffer); printf("Accepted\n"); input_buffer[0] = '\0';}
   | s1 NEG IMAGINARY ';' { printf("Input: %s\n", input_buffer); printf("Accepted\n"); input_buffer[0] = '\0';}
   | NEG s1 IMAGINARY  ';'   { printf("Input: %s\n", input_buffer); printf("Rejected: Missing operator\n"); input_buffer[0] = '\0';}
   | NEG s1 ADD s1 ';'     { printf("Input: %s\n", input_buffer); printf("Rejected - missing i \n"); input_buffer[0] = '\0';}
   | NEG s1 NEG s1 ';'      { printf("Input: %s\n", input_buffer); printf("Rejected - missing i \n"); input_buffer[0] = '\0';}
   | NEG s1 ADD I  ';'      { printf("Input: %s\n", input_buffer); printf("Rejected - missing imaginary part coefficient \n"); input_buffer[0] = '\0';}
   | NEG s1 NEG I  ';'      { printf("Input: %s\n", input_buffer); printf("Rejected - missing imaginary part coefficient \n"); input_buffer[0] = '\0';}
   | NEG ADD IMAGINARY ';'  { printf("Input: %s\n", input_buffer); printf("Rejected - missing Real part coefficient \n"); input_buffer[0] = '\0';}
   | NEG NEG IMAGINARY ';'    { printf("Input: %s\n", input_buffer); printf("Rejected - missing Real part coefficient \n"); input_buffer[0] = '\0';}
   | s1 IMAGINARY ';'    { printf("Input: %s\n", input_buffer); printf("Rejected: Missing operator\n"); input_buffer[0] = '\0';}
   | s1 ADD s1 ';'       { printf("Input: %s\n", input_buffer); printf("Rejected - missing i \n"); input_buffer[0] = '\0';}
   | s1 NEG s1 ';'       { printf("Input: %s\n", input_buffer); printf("Rejected - missing i \n"); input_buffer[0] = '\0';}
   | s1 ADD I  ';'       { printf("Input: %s\n", input_buffer); printf("Rejected - missing imaginary part coefficient \n"); input_buffer[0] = '\0';}
   | s1 NEG I  ';'       { printf("Input: %s\n", input_buffer); printf("Rejected - missing imaginary part coefficient \n"); input_buffer[0] = '\0';}
   | ADD IMAGINARY ';'   { printf("Input: %s\n", input_buffer); printf("Rejected - missing Real part coefficient \n"); input_buffer[0] = '\0';}
   | NEG IMAGINARY  ';'  { printf("Input: %s\n", input_buffer); printf("Rejected - missing Real part coefficient \n"); input_buffer[0] = '\0';}
   | error ';'            {yyerrok; yyclearin; printf("Input: %s\n", input_buffer); printf("Rejected - Invalid Format \n"); input_buffer[0] = '\0';}
   ;
s1 : INT | FLOAT ;
%%

int yyerror(char *s) {
//    printf("Input: %s\n", input_buffer); // Print the input when an error occurs
//    printf("error: %s\n", s);
//    input_buffer[0] = '\0'; // Clear the buffer for the next input
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

