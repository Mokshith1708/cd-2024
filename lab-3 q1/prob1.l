%{
#include <stdio.h>
#include <stdlib.h>	
#include <stdbool.h>
#include "y.tab.h"
FILE *yyin; 
%}           
%%
'0' return ZERO;
'1' return ONE;      
';' return ';';
. return yytext[0];	      
%% 


