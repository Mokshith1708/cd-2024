%{
#include <stdio.h>
#include <stdlib.h>

FILE *yyin; // Declare yyin as a global file pointer
%}
/*
re1 is for the language where the string has 'a' before z and also at the end. Same way re2,re3 and re4 are for b,c and d instead a.
*/
re1 ([abcd])+az([abcd])+a
re2 ([abcd])+bz([abcd])+b
re3 ([abcd])+cz([abcd])+c
re4 ([abcd])+dz([abcd])+d          
ws [ \t\n]              
%%
{ws}+ ;                 
{re1}|{re2}|{re3}|{re4} {
                         // re1 is for the language where the string has a before z and also at the end. Same way re2,re3 and re4 are b,c and d;
                        printf("%s - belongs to the given language\n", yytext);
                        }
.* ;     { printf("%s - doesn't belongs to the given language\n", yytext);}
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
    printf("In the given strings :\n");
    // Call yylex() to start lexical analysis
    yylex();

    // Close the file
    fclose(fp);
    return 0;
}

