%{
#include <stdio.h>
%}

%%
[0-9]+       { printf("Digit: %s\n", yytext); }
[+\-\*/]    { printf("Operator: %s\n", yytext); }
\n           { return 0; }
.            { /* ignore other characters */ }
%%

int yywrap() { return 1; }

int main() {
    yylex();
    return 0;
}
