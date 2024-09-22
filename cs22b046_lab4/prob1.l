%{
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "y.tab.h"

extern char input_buffer[256]; // Declare the input buffer from the .y file
%}

%%

[ \t\n]+            ;                   // Ignore whitespace (spaces, tabs, newlines)
"+"                 { strncat(input_buffer, yytext, sizeof(input_buffer) - strlen(input_buffer) - 1); return ADD; } 
"-"                 { strncat(input_buffer, yytext, sizeof(input_buffer) - strlen(input_buffer) - 1); return NEG; } 
[0-9]+              { strncat(input_buffer, yytext, sizeof(input_buffer) - strlen(input_buffer) - 1); return INT; } 
[0-9]+\.[0-9]+      { strncat(input_buffer, yytext, sizeof(input_buffer) - strlen(input_buffer) - 1); return FLOAT; } 
("i"[0-9]+\.[0-9]+|[0-9]+\.[0-9]+"i"|"i"[0-9]+|[0-9]+"i")  { strncat(input_buffer, yytext, sizeof(input_buffer) - strlen(input_buffer) - 1); return IMAGINARY; } 
"i"                 { strncat(input_buffer, yytext, sizeof(input_buffer) - strlen(input_buffer) - 1); return I; } 
";"                 { strncat(input_buffer, yytext, sizeof(input_buffer) - strlen(input_buffer) - 1); return ';'; } 
.                   { strncat(input_buffer, yytext, sizeof(input_buffer) - strlen(input_buffer) - 1); return yytext[0];  } 
%%

int yywrap() {
    return 1;
}

