#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Global variables
int i = 0, j = 0, no = 0;
int tmpch = 90; // ASCII for 'Z' — used to assign temporary variable names like Z, Y, X...
char str[100], left[15], right[15];

// Structure to store operator position and symbol
struct exp
{
    int pos;
    char op;
} k[15];

// Function declarations
void findopr();
void explore();
void fleft(int);
void fright(int);

int main()
{
    printf("\t\tINTERMEDIATE CODE GENERATION\n\n");
    printf("Enter the Expression: ");
    scanf("%s", str); // e.g. a=b+c*d

    printf("\nThe Intermediate Code:\n");
    findopr();  // Step 1: Find operators and their positions
    explore();  // Step 2: Generate intermediate code
    return 0;
}

// Function to find all operators in the expression and record their positions
void findopr()
{
    // Priority order: ':' > '/' > '*' > '+' > '-'
    for (i = 0; str[i] != '\0'; i++)
        if (str[i] == ':')
        {
            k[j].pos = i;
            k[j++].op = ':';
        }

    for (i = 0; str[i] != '\0'; i++)
        if (str[i] == '/')
        {
            k[j].pos = i;
            k[j++].op = '/';
        }

    for (i = 0; str[i] != '\0'; i++)
        if (str[i] == '*')
        {
            k[j].pos = i;
            k[j++].op = '*';
        }

    for (i = 0; str[i] != '\0'; i++)
        if (str[i] == '+')
        {
            k[j].pos = i;
            k[j++].op = '+';
        }

    for (i = 0; str[i] != '\0'; i++)
        if (str[i] == '-')
        {
            k[j].pos = i;
            k[j++].op = '-';
        }

    k[j].op = '\0'; // mark end
}

// Function to generate intermediate code using operator precedence
void explore()
{
    i = 0;
    while (k[i].op != '\0')
    {
        fleft(k[i].pos);   // find left operand
        fright(k[i].pos);  // find right operand

        // Replace the operator in the expression with temporary variable (Z, Y, X,...)
        str[k[i].pos] = tmpch--;

        // Print the generated intermediate code line
        printf("\t%c := %s %c %s\n", str[k[i].pos], left, k[i].op, right);
        i++;
    }

    fright(-1); // final assignment

    if (no == 0)
    {
        fleft(strlen(str));
        printf("\t%s := %s\n", right, left);
        exit(0);
    }

    printf("\t%s := %c\n", right, str[k[--i].pos]);
}

// Function to find the left operand of an operator
void fleft(int x)
{
    int w = 0, flag = 0;
    x--;

    // Traverse backward until operator or start of string
    while (x != -1 && str[x] != '+' && str[x] != '-' && str[x] != '*' &&
           str[x] != '/' && str[x] != '=' && str[x] != ':' && str[x] != '\0')
    {
        if (str[x] != '$' && flag == 0)
        {
            left[w++] = str[x];
            left[w] = '\0';
            str[x] = '$'; // mark as used
            flag = 1;
        }
        x--;
    }
}

// Function to find the right operand of an operator
void fright(int x)
{
    int w = 0, flag = 0;
    x++;

    // Traverse forward until operator or end of string
    while (str[x] != '+' && str[x] != '-' && str[x] != '*' &&
           str[x] != '/' && str[x] != '=' && str[x] != ':' &&
           str[x] != '\0')
    {
        if (str[x] != '$' && flag == 0)
        {
            right[w++] = str[x];
            right[w] = '\0';
            str[x] = '$'; // mark as used
            flag = 1;
        }
        x++;
    }
}
