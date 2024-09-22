%{
#include <stdio.h>
#include <stdlib.h>

int yylex();
int yyerror(const char *);
extern FILE *yyin;

// Declare the global counts
int counts[3] = {0, 0, 0}; // counts[0] for A, counts[1] for B, counts[2] for C

%}

%union {
    int counts[3]; // Array to hold counts
}

%token A B C
%type <counts> a k s1 s2 s3 

%%

a: k {
        // Check if counts of A, B, C are all different
        if ($1[0] != $1[1] && $1[1] != $1[2] && $1[0] != $1[2]) {
            printf("a^%d b^%d c^%d : Accepted \n", $1[0], $1[1], $1[2]);
        } else {
            printf("a^%d b^%d c^%d : Rejected as condition i!=j!=k is not satisfied \n", $1[0], $1[1], $1[2]);
        }
    } a 
  | /* empty */ {
        printf("Done.\n");
    };

k: s1 s2 s3 ';' {
        $$[0] = $1[0];
        $$[1] = $2[1];
        $$[2] = $3[2];

} 
   | error ';' {yyerrok; yyclearin; printf("Rejected - Invalid Format \n") ;}
    ;
    
s1: A s1 {
        // Update counts for B
        $$[0] = $2[0]+1;
        $$[1] = $2[1];
        $$[2] = $2[2];
    }
  |  {
        // Pass counts from s3
        $$[0] = 0;
        $$[1] = 0;
        $$[2] = 0;
    }
    ;
    
s2: B s2 {
        // Update counts for B
        $$[0] = $2[0];
        $$[1] = $2[1] + 1;
        $$[2] = $2[2];
    }
  |  {
        // Pass counts from s3
        $$[0] = 0;
        $$[1] = 0;
        $$[2] = 0;
    }
    ;

s3: C s3 {
        // Update counts for C
        $$[0] = $2[0];
        $$[1] = $2[1];
        $$[2] = $2[2] + 1;
    }
  | /* empty */ {
        // Initialize counts to zero
        $$[0] = 0;
        $$[1] = 0;
        $$[2] = 0;
    }
    ;

%%

int yyerror(const char *s) {
    //fprintf(stderr, "Error: %s\n", s);
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

