%{
#include "y.tab.h"
%}

%%
[ \t\n]+  ;  // Ignore whitespace and newlines
a       { yylval.counts[0] = 0; return A; }
b       { yylval.counts[1] = 0; return B; }
c       { yylval.counts[2] = 0; return C; }
";"     { return ';'; }
.       { return yytext[0];  } 
%%

int yywrap() {
    return 1;
}

