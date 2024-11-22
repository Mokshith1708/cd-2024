%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "y.tab.h"

extern char* yytext; // The matched text
%}

%%

[ \t\n]+              /* Ignore whitespace */
"if"		      { yylval.value = strdup(yytext); return IF; }
"else"		      { yylval.value = strdup(yytext); return ELSE; }
"true"		      { yylval.value = strdup(yytext); return T; }
"false"		      { yylval.value = strdup(yytext); return F; }
"while"               { yylval.value = strdup(yytext); return WHILE;}
[a-zA-Z_][a-zA-Z0-9_]* { yylval.value = strdup(yytext); return IDENTIFIER; }
[0-9]+(\.[0-9]+)?     { yylval.value = strdup(yytext); return NUMBER; }
"+"                   { yylval.value = strdup(yytext); return '+'; }
"-"                   { yylval.value = strdup(yytext); return '-'; }
"*"                   { yylval.value = strdup(yytext);  return '*'; }
"/"                   { yylval.value = strdup(yytext);  return '/'; }
"%"                   { yylval.value = strdup(yytext);  return '%'; }
"="                   { yylval.value = strdup(yytext); return EQ; }
"+="                   {yylval.value = strdup(yytext);  return PEQ; }
"-="                   { yylval.value = strdup(yytext); return MEQ; }
"*="                   { yylval.value = strdup(yytext); return MLEQ; }
"/="                   { yylval.value = strdup(yytext); return DEQ; }
"--"                   { yylval.value = strdup(yytext); return MM;}
"++"                   { yylval.value = strdup(yytext); return PP;}
";"                   { yylval.value = strdup(yytext);  return ';'; }
"("                   { yylval.value = strdup(yytext); return '('; }
")"                   { yylval.value = strdup(yytext); return ')'; }
"{"                   { yylval.value = strdup(yytext); return LB; }
"}"                   { yylval.value = strdup(yytext); return RB; }
">"                   { yylval.value = strdup(yytext); return GT; }
"<"                   { yylval.value = strdup(yytext); return LT; }
">="		      { yylval.value = strdup(yytext); return GTE; }
"<="		      { yylval.value = strdup(yytext); return LTE; }
"!="		      { yylval.value = strdup(yytext); return NE; }
"!"                   { yylval.value = strdup(yytext); return NN; }
"=="		      { yylval.value = strdup(yytext); return EE; }
"&&"		      { yylval.value = strdup(yytext); return AND; }
"||"		      { yylval.value = strdup(yytext); return OR; }
.                     { return yytext[0]; }
%%

int yywrap() {
    return 1;
}

