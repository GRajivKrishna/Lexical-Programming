#include <stdio.h>
#include <string.h>

#define SIZE 10

int main() {
    char production[SIZE][SIZE];
    int num;

    printf("Enter Number of Productions: ");
    scanf("%d", &num);

    printf("Enter the grammar productions (like E->E+T or A->aB):\n");
    for (int i = 0; i < num; i++) {
        scanf("%s", production[i]);
    }

    printf("\nGRAMMAR:\n");
    for (int i = 0; i < num; i++) {
        printf("%s\n", production[i]);
    }

    printf("\nChecking for Left Recursion...\n");

    for (int i = 0; i < num; i++) {
        char non_terminal = production[i][0];
        int index = 3; // after "->"

        if (non_terminal == production[i][index]) {
            // Left recursive production found
            printf("\n%s is left recursive.\n", production[i]);

            char alpha[SIZE], beta[SIZE];
            int k = 0, j = 0;

            // Extract alpha (after NT again)
            index++; 
            while (production[i][index] != '\0') {
                alpha[k++] = production[i][index++];
            }
            alpha[k] = '\0';

            // Extract beta (if any production exists with different start)
            // Here we assume simple single production form A->Aα | β
            printf("Enter beta part (non-left-recursive production for %c): ", non_terminal);
            scanf("%s", beta);

            // Print grammar without left recursion
            printf("\nGrammar without left recursion:\n");
            printf("%c -> %s%c'\n", non_terminal, beta, non_terminal);
            printf("%c' -> %s%c' | ε\n", non_terminal, alpha, non_terminal);

        } else {
            printf("\n%s is not left recursive.\n", production[i]);
        }
    }

    return 0;
}
