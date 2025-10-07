%{
#include <stdio.h>
int words = 0, digits = 0, specials = 0, lines = 0;
%}

%%
[ \t]+          ; // Ignore whitespace
[\n]+             { lines++; }
[a-zA-Z]+         { words++; }
[0-9]+            { digits++; }
.                 { specials++; }
%%

int yywrap() { return 1; }

int main() {
    yylex();
    printf("\n Words: %d\nDigits: %d\nSpecial Characters: %d\nLines: %d\n", words, digits, specials,lines);
    return 0;
}
