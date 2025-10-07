#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char s[20], stack[20];

// Predictive parsing table
// Grammar:
// E  → T E'
// E' → + T E' | ε
// T  → F T'
// T' → * F T' | ε
// F  → i | (E)
char m[5][6][5] = {
    {"tb",   "",    "",   "tb",   "",   ""},       // E
    {"",    "+tb",  "",   "",    "e",  "e"},       // E'
    {"fc",   "",    "",   "fc",   "",   ""},       // T
    {"",    "e",   "*fc", "",    "e",  "e"},       // T'
    {"i",    "",    "",  "(e)",   "",   ""}        // F
};

// Number of symbols in each production
int size[5][6] = {
    {2,0,0,2,0,0},   // E
    {0,3,0,0,1,1},   // E'
    {2,0,0,2,0,0},   // T
    {0,1,3,0,1,1},   // T'
    {1,0,0,3,0,0}    // F
};

int main() {
    int i, j, k, n, str1 = 0, str2 = 0;

    printf("\nEnter the input string (end with $): ");
    scanf("%s", s);

    n = strlen(s);

    // Initialize stack with $E
    stack[0] = '$';
    stack[1] = 'e';   // Start symbol E
    i = 1;            // top of stack
    j = 0;            // input pointer

    printf("\nStack\tInput\n");
    printf("_________________________\n");

    while ((stack[i] != '$') && (s[j] != '$')) {
        if (stack[i] == s[j]) {
            // Match terminal
            i--;
            j++;
        } else {
            // Map stack[i] (non-terminal) → row
            switch (stack[i]) {
                case 'e': str1 = 0; break;  // E
                case 'b': str1 = 1; break;  // E'
                case 't': str1 = 2; break;  // T
                case 'c': str1 = 3; break;  // T'
                case 'f': str1 = 4; break;  // F
            }

            // Map input symbol → column
            switch (s[j]) {
                case 'i': str2 = 0; break;
                case '+': str2 = 1; break;
                case '*': str2 = 2; break;
                case '(': str2 = 3; break;
                case ')': str2 = 4; break;
                case '$': str2 = 5; break;
            }

            // Error handling
            if (m[str1][str2][0] == '\0') {
                printf("\nERROR: Invalid string.\n");
                exit(0);
            }
            // ε-production → just pop
            else if (m[str1][str2][0] == 'e') {
                i--;
            }
            // id → replace with 'i'
            else if (m[str1][str2][0] == 'i') {
                stack[i] = 'i';
            }
            // Normal production
            else {
                // Pop non-terminal
                i--;
                // Push RHS in reverse
                for (k = size[str1][str2] - 1; k >= 0; k--) {
                    stack[++i] = m[str1][str2][k];
                }
            }
        }

        // Print current stack
        for (k = 0; k <= i; k++) printf("%c", stack[k]);
        printf("\t");
        // Print remaining input
        for (k = j; k < n; k++) printf("%c", s[k]);
        printf("\n");
    }

    if (stack[i] == '$' && s[j] == '$')
        printf("\nSUCCESS: String parsed correctly.\n");
    else
        return 0;

    return 0;
}
