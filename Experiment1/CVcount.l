%{
#include <stdio.h>
int vowels = 0, consonants = 0;
%}

%%
[aAeEiIoOuU]   { vowels++; }
[b-df-hj-np-tv-zB-DF-HJ-NP-TV-Z] { consonants++; }
end         { return 0; }
%%

int yywrap() { return 1; }

int main() {
    yylex();
    printf("Vowels: %d, Consonants: %d\n", vowels, consonants);
    return 0;
}
