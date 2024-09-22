%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "y.tab.h"  

%}

%%
[ \t\n]+                            ; // Ignore whitespace
[0-3]?[0-9]                        { yylval.day = atoi(yytext); return DAY; }
[0-9]{4}                           { yylval.year = atoi(yytext); return YEAR; }
[A-Za-z]{3}                        { yylval.month = strdup(yytext); return MONTH; }
-                                  { return '-'; }
";"                                { return ';'; }  
.                                  { return yytext[0];  } 
%%

int yywrap() {
    return 1;  
}

