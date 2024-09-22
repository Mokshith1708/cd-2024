%{
#include <stdio.h>
#include <stdlib.h>

FILE *yyin; // Declare yyin as a global file pointer
%}
/* 
Regular expressions for various URL components
numb [0-9]+     
 This regular expression matches one or more digits.
alpha [a-zA-Z]
 This regular expression matches any single letter (either uppercase or lowercase).
alphanumb [0-9a-zA-Z._-]
 This regular expression matches alphanumeric characters, dots, underscores, and hyphens.
name {alpha}{alphanumb}*
 This regular expression matches a sequence of letters followed by any combination of alphanumeric characters, dots, underscores, and hyphens.
scheme (http|https|ftp|mailto|file)
 This regular expression matches a URL scheme such as http, https, ftp, mailto, or file.
uandp ({name}:{name}@)?
 This regular expression matches an optional username and password in the format username:password@.
host [0-9a-zA-Z.-]+
 This regular expression matches the host part of the URL, including digits, letters, dots, and hyphens.
port {numb}
 This regular expression matches the port number.
path (\/[0-9a-zA-Z._-]+)*
 This regular expression matches the path of the URL, starting with a slash followed by any combination of alphanumeric characters, dots, underscores, and hyphens.
query \?{name}={name}(&{name}={name})*
 This regular expression matches the query parameters of the URL, starting with a question mark followed by key-value pairs separated by '&'.
frag \#[a-zA-Z0-9_-]+
 This regular expression matches the fragment part of the URL, starting with a hash followed by alphanumeric characters, underscores, or hyphens.
colon "://"|":"
 This regular expression matches either "://" or ":" which are used as separators in URLs.
ws [ \t\n]
 This regular expression matches whitespace characters (spaces, tabs, and newlines).
email [a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}
 This regular expression matches email addresses.
*/

numb [0-9]+     
alpha [a-zA-Z]
alphanumb [0-9a-zA-Z._-]
name {alpha}{alphanumb}*
scheme (http|https|ftp|mailto|file)
uandp ({name}:{name}@)?
host [0-9a-zA-Z.-]+
port {numb}
path (\/[0-9a-zA-Z._-]+)*
query \?{name}={name}(&{name}={name})*
frag \#[a-zA-Z0-9_-]+
colon "://"|":"
ws [ \t\n]
email [a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}
%%

{ws}+                 ; // Ignore whitespace
mailto:{email}        { 
                        printf("\nSCHEME: mailto\n");
                        printf("EMAIL: %s\n", yytext + 7); 
                      }
{scheme}{colon}           {
                           int len = strlen(yytext);
                            printf("\nSCHEME: %.*s\n",len-3,yytext); }
{uandp}               { printf("USERNAME&PASSWARD: %s\n", yytext); }
{host}                { printf("HOST: %s\n", yytext); }
:{port}               { printf("PORT: %s\n", yytext); }
{path}                { printf("PATH: %s\n", yytext); }
{query}               { printf("QUERY: %s\n", yytext); }
{frag}                { printf("FRAGMENT: %s\n", yytext); }
.                    ; // Ignore any other characters
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
     printf("The Tokens for each link are:\n");
    yylex();

    // Close the file
    fclose(fp);
    return 0;
}

