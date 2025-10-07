/* Declaring two counters: one for number 
   of lines and one for number of characters */
%{
#include <stdio.h>
int no_of_lines = 0;
int no_of_chars = 0;
%}

/* Rule Section:
   - Rule 1 counts lines
   - Rule 2 counts characters
   - Rule 3 exits when "end" is encountered */

%%
\n      { ++no_of_lines; }
.       { ++no_of_chars; }
end     { return 0; }
%%

/* User Code Section */
int yywrap() { return 1; }

int main(int argc, char **argv)
{
    yylex();
    printf("number of lines = %d, number of chars = %d\n", no_of_lines, no_of_chars);
    return 0;
}
