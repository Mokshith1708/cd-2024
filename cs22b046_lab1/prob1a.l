%{
#include <stdio.h>
#include <stdlib.h>	
FILE *yyin; // Declare yyin as a global file pointer
%}

/*re1 and re3 are regular expressions for a string not containing two consecutive zeros and re2 is for three consecutive ones. 
                 This combinely gives the required regular expression*/

re1 0?(10|1)*     
re2 1{3}           
re3 (01|1)*0?       
ws [ \t\n]              
%%
{ws}+ ;             
{re1}{re2}{re3} { printf("%s - belongs to the given language\n",  yytext); } 
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
    printf("{w ∈ {0, 1}∗ | w contains 111 as a substring and does not contain 00 as a substring}\n");
    // Call yylex() to start lexical analysis
    yylex();

    // Close the file
    fclose(fp);
    return 0;
}
