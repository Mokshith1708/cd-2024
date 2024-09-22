%{
#include <stdio.h>
#include <stdlib.h>	
#include <stdbool.h>
FILE *yyin; // Declare yyin as a global file pointer
%}

numb [0-9]
alpha [a-zA-Z]     
alphauscore [a-zA-Z_]
alphanumb [0-9a-zA-Z_]
name {alphauscore}{alphanumb}*
dtype (int|float)
negpointnums (((-?){numb})?{numb}*(\.{numb}*)?)
ws [ \t]              
%%
{dtype}{ws}+{name}{ws}*={ws}*{negpointnums}{ws}*(,{ws}*{name}{ws}*={ws}*{negpointnums}{ws}*)*;       { printf("%s - belongs to the given language\n",  yytext); }
.* ;        { printf("%s - doesn't belongs to the given language\n",  yytext); }
%%

int main(int argc, char *argv[]) {
    FILE *fp;
    
    char buffer[256]; // Buffer to store each line of the file

    // Check if the file name is provided as an argument
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Open the file for reading
    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    // Assign the input file to yyin
    yyin = fp;
    // Call yylex() to start lexical analysis
    yylex();

    // Close the file
    fclose(fp);
    return 0;	
}


