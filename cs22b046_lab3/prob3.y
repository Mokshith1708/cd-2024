%{
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <string.h>

int yylex();
int yyerror(const char *);
extern FILE *yyin;
int is_valid_date(int day, const char* month, int year);
void to_lowercase(char* str);
%}

%union {
    int day;
    int year;
    char* month;
}

%token <day> DAY
%token <year> YEAR
%token <month> MONTH

%type <day> D
%type <month> M
%type <year> Y
%type <month> s
%%

a1: s ';' a1
   | /* empty */ { printf("done\n"); }
   ;

s: D '-' M '-' Y { 
        if (is_valid_date($1, $3, $5)) {
            printf("%d-%s-%d - Accepted\n",$1,$3,$5);
        } else {
            printf("%d-%s-%d - Rejected - Date out of range\n",$1,$3,$5);
        }
    }
  ;

D: DAY { $$ = $1; }
  ;

M: MONTH { $$ = $1; }
  ;

Y: YEAR { $$ = $1; }
  ;

%%


void to_lowercase(char* str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int is_valid_date(int day, const char* month, int year) {
    const char* months[] = {"jan", "feb", "mar", "apr", "may", "jun", "jul", "aug", "sep", "oct", "nov", "dec"};
    int month_days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    char month_lower[10];
    strncpy(month_lower, month, sizeof(month_lower) - 1);
    month_lower[sizeof(month_lower) - 1] = '\0'; 
    to_lowercase(month_lower);
    int month_index = -1;
    for (int i = 0; i < 12; i++) {
        if (strcasecmp(months[i], month_lower) == 0) {
            month_index = i;
            break;
        }
    }
    if (month_index == -1) {
        return 0; 
    }
    if (month_index == 1 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))) {
        month_days[1] = 29;
    }
    if (day < 1 || day > month_days[month_index]) {
        return 0; 
    }
    return 1; 
}
int yyerror(const char *s) {
    printf("error: %s\n", s);
    return 0;
}
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    yyin = fopen(argv[1], "r");
    if (!yyin) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    yyparse();
    fclose(yyin);
    return 0;
}

