%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "y.tab.h"

extern char* yytext; // The matched text
%}

%%

[ \t\n]+              /* Ignore whitespace */
[a-zA-Z_][a-zA-Z0-9_]* { yylval.sval = strdup(yytext); return IDENTIFIER; }
[0-9]+(\.[0-9]+)?     { yylval.sval = strdup(yytext); return NUMBER; }
"+"                   { yylval.sval = strdup(yytext); return '+'; }
"-"                   { yylval.sval = strdup(yytext); return '-'; }
"*"                   { yylval.sval = strdup(yytext);  return '*'; }
"/"                   { yylval.sval = strdup(yytext);  return '/'; }
"%"                   { yylval.sval = strdup(yytext);  return '%'; }
"="                   { yylval.sval = strdup(yytext); return EQ; }
"+="                   {yylval.sval = strdup(yytext);  return PEQ; }
"-="                   { yylval.sval = strdup(yytext); return MEQ; }
"*="                   { yylval.sval = strdup(yytext); return MLEQ; }
"/="                   { yylval.sval = strdup(yytext); return DEQ; }
"--"                   { yylval.sval = strdup(yytext); return MM;}
"++"                   { yylval.sval = strdup(yytext); return PP;}
";"                   { yylval.sval = strdup(yytext);  return ';'; }
"("                   { yylval.sval = strdup(yytext); return '('; }
")"                   { yylval.sval = strdup(yytext); return ')'; }
.                     { return yytext[0]; }
%%

int yywrap() {
    return 1;
}
