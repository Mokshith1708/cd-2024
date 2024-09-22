%{
#include <stdio.h>
#include <stdlib.h>	
#include <stdbool.h>
FILE *yyin; // Declare yyin as a global file pointer
bool check(char *inp);
%}

re1 1*[1*01*01*01*01*]
ws [ \t\n]              
%%
{ws}+ ;   
{re1}*           {
                     if (check(yytext)) {
                     printf("%s - belongs to the given language\n",yytext);
                   } 
                   else {
                  printf("%s - doesn't belongs to the given language\n",yytext);
                   }
                               }
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
    printf("{w ∈ {x.xR | x ∈ {0, 1}∗} and the number of 0’s in w is divisible by 4}\n");
    // Call yylex() to start lexical analysis
    yylex();

    // Close the file
    fclose(fp);
    return 0;
}

bool check(char *inp) {
    int x = 0, y = 0;
    // Count occurrences of 'x' and 'y'
   //printf("%s\n",inp);
    while (*(inp+x) != '\0') {
        //inp++;
        x++;
    }
    // printf("%s %d\n",inp,x);
   // cout<<inp<<" "<<x<<endl;
    if(x%2)
    {
     return false;
    }
    for(int i=0;i<x/2;i++)
    {
        if(inp[i] != inp[x-i-1])
        {
        return false;
    }
    }
    return true; 
}


