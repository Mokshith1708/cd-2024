%{
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> // Include for bool type

FILE *yyin; // Declare yyin as a global file pointer

bool check(char *inp);
%}

/* The used regular expression is for the language all strings formed using x,y,z. The check function is used to check whether x%2==y%3 . */

alpha [xyz] 
ws [\t\n]

%%
{ws}+ ;
{alpha}* {  
    if (check(yytext)) {
        printf("%s - belongs to the given language\n",yytext);
    } else {
        printf("%s - doesn't belongs to the given language\n",yytext);
    }
}
.* { printf("%s - doesn't belongs to the given language\n",yytext);}
%%

int main(int argc, char *argv[]) {
    FILE *fp;

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
    printf("{w ∈ {x, y, z}∗ | in w the number of xs modulo 2 is equal to the number of ys modulo3}\n");
    // Call yylex() to start lexical analysis
    yylex();

    // Close the file
    fclose(fp);
    return 0;
}

bool check(char *inp) {
    int x = 0, y = 0;
    // Count occurrences of 'x' and 'y'
    while (*inp != '\0') {
        if (*inp == 'x') {
            x++;  // counting the number of x's in th string.
        } else if (*inp == 'y') {
            y++; // counting the number of y's in the string.
        }
        inp++;
    }
    return (x % 2) == (y % 3); // checking whether x%2==y%3 or not to return.
}

