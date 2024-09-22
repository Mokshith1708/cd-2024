%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *yyin; // Declare yyin as a global file pointer

// Define an array to store token-type and lexeme pairs
#define MAX_TOKENS 1000000
char *token_types[MAX_TOKENS];
char *lexemes[MAX_TOKENS];
int token_count = 0;

void store_token(const char *type, const char *lexeme) {
    // Check for duplicate entries
    for (int i = 0; i < token_count; i++) {
        if (strcmp(token_types[i], type) == 0 && strcmp(lexemes[i], lexeme) == 0) {
            return; // Duplicate found, do not add
        }
    }

    // If no duplicate is found, add the new token
    if (token_count >= MAX_TOKENS) {
        fprintf(stderr, "Token storage limit exceeded.\n");
        exit(EXIT_FAILURE);
    }
    token_types[token_count] = strdup(type);
    lexemes[token_count] = strdup(lexeme);
    token_count++;
    // Print to the terminal
    printf("Token Type: %s , Lexeme: %s\n", type, lexeme);
}

%}

keywords (auto|break|case|char|const|continue|default|do|double|else|enum|extern|float|for|goto|if|int|long|register|return|short|signed|sizeof|static|struct|switch|typedef|union|unsigned|void|volatile|while)
alphauscore [a-zA-Z_]
alphanumb [0-9a-zA-Z_]
numb [0-9]
identifiers {alphauscore}{alphanumb}*
integerconsts ((-?){numb}+)
floatconsts (((-?){numb}+)(\.{numb}*)?)
charconsts '\[^\'\n\]'
stringconsts \"([^\"\\\n]|\\.)*\"
operators (\+|-|\/|\*|\>|\<|\=|\!|%|%=|^=|^|(\|)|(\|)=|(\|\|)|&&|&|&=|~|\?|\+\+|--|\+=|-=|\*=|(\/)=|\%=|<=|>=|==|\!=)
punctuation (;|,|\(|\)|:|\{|\}|:|\[|\])
ws [ \t\n]+

%%
{ws} ;   // Skip white spaces
\/\/.* ;   // Skip single-line comments
\/\*([^*]|\*+[^\/])*\*+\/ ;   // Skip multi-line comments
{keywords}  { store_token("Keyword", yytext); }
{identifiers}  { store_token("Identifier", yytext); }
{integerconsts}  { store_token("Constant (IntegerConstants)", yytext); }
{floatconsts}  { store_token("Constant (FloatConstants)", yytext); }
{charconsts}  { store_token("Constant (CharConstants)", yytext); }
{stringconsts}  { store_token("String", yytext); }
{operators}  { store_token("Operator", yytext); }
{punctuation}  { store_token("Special character/Punctuation", yytext); }
. ;   // Catch-all for any other characters (if needed, adjust if not required)
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
    // Call yylex() to start lexical analysis
    yylex();

    // Close the file
    fclose(fp);

    // Write tokens to SymbolTable.txt
    FILE *output_file = fopen("SymbolTable.txt", "w");
    if (output_file == NULL) {
        perror("Error opening SymbolTable.txt");
        return EXIT_FAILURE;
    }

    fprintf(output_file, "        Token Type                        Lexeme\n");
    fprintf(output_file, "-----------------------------  --------------------------\n");
    for (int i = 0; i < token_count; i++) {
        fprintf(output_file, "%-30s %-30s\n", token_types[i], lexemes[i]);
        free(token_types[i]);
        free(lexemes[i]);
    }
    fclose(output_file);

    return 0;
}

