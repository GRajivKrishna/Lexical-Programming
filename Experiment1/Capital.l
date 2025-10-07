/* Definition Section has one variable which 
   can be accessed inside yylex() and main() */

%{
#include <stdio.h>
int count = 0;
%}

/* Rule Section has three rules: 
   - First rule matches capital letters 
   - Second rule matches any character except newline 
   - Third rule stops processing on newline */

%%
[A-Z] { printf("%s capital letter\n", yytext); count++; }
.     { printf("%s not a capital letter\n", yytext); }
\n    { return 0; }
%%

/* Code Section: prints the number of capital letters */
int yywrap() { return 1; }

int main() {
    yylex();
    printf("\nNumber of Capital letters in the given input - %d\n", count);
    return 0;
}
