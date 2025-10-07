#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int main()
{
    int x = 0, n, i = 0, j = 0;
    void *mypointer, *T4Tutorials_address[50]; // increased array size for safety
    char ch, c;
    char T4Tutorials_Array2[100], T4Tutorials_Array3[100]; // input and symbol storage

    printf("Input the expression ending with $ sign: ");

    // Read expression character by character until '$' is encountered
    while ((c = getchar()) != '$')
    {
        T4Tutorials_Array2[i] = c;
        i++;
    }

    n = i - 1; // last valid index

    printf("\nGiven Expression: ");
    for (i = 0; i <= n; i++)
    {
        printf("%c", T4Tutorials_Array2[i]);
    }

    printf("\n\nSymbol Table Display\n");
    printf("Symbol\tAddress\t\tType\n");

    // Traverse through the expression to identify identifiers and operators
    while (j <= n)
    {
        c = T4Tutorials_Array2[j];

        // If character is an alphabet → Identifier
        if (isalpha(c))
        {
            mypointer = malloc(sizeof(char)); // allocate one byte for the symbol
            if (mypointer == NULL)
            {
                printf("Memory allocation failed!\n");
                return 1;
            }

            T4Tutorials_address[x] = mypointer;
            T4Tutorials_Array3[x] = c;

            printf("%c\t%p\tidentifier\n", c, mypointer); // %p for address
            x++;
        }
        else
        {
            // Check for operator symbols
            ch = c;
            if (ch == '+' || ch == '-' || ch == '*' || ch == '=' || ch == '/')
            {
                mypointer = malloc(sizeof(char));
                if (mypointer == NULL)
                {
                    printf("Memory allocation failed!\n");
                    return 1;
                }

                T4Tutorials_address[x] = mypointer;
                T4Tutorials_Array3[x] = ch;

                printf("%c\t%p\toperator\n", ch, mypointer);
                x++;
            }
        }

        j++;
    }

    // Free allocated memory at the end
    for (i = 0; i < x; i++)
        free(T4Tutorials_address[i]);

    return 0;
}
