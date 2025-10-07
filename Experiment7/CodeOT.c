#include <stdio.h>
#include <string.h>

// Structure to represent an operation (e.g. t1 = a + b)
struct op
{
    char l;      // Left-hand side variable
    char r[20];  // Right-hand side expression
} op[10], pr[10]; // op[] holds original code, pr[] holds optimized code

int main()
{
    int a, i, k, j, n, z = 0, m, q;
    char *p, *l;
    char temp, t;
    char *tem;

    printf("Enter the Number of Expressions: ");
    scanf("%d", &n);

    // Input: read intermediate code
    for (i = 0; i < n; i++)
    {
        printf("Left: ");
        scanf(" %c", &op[i].l); // variable name (e.g., t1)
        printf("Right: ");
        scanf("%s", op[i].r);   // expression (e.g., a+b)
    }

    // Display original intermediate code
    printf("\nIntermediate Code:\n");
    for (i = 0; i < n; i++)
    {
        printf("%c = %s\n", op[i].l, op[i].r);
    }

    // Step 1: Dead Code Elimination
    // Keep only those statements whose results are used later
    for (i = 0; i < n - 1; i++)
    {
        temp = op[i].l; // LHS variable name
        for (j = 0; j < n; j++)
        {
            p = strchr(op[j].r, temp); // check if this variable is used in any RHS
            if (p)
            {
                pr[z].l = op[i].l;
                strcpy(pr[z].r, op[i].r);
                z++;
                break;
            }
        }
    }

    // Add the last statement (usually an output assignment)
    pr[z].l = op[n - 1].l;
    strcpy(pr[z].r, op[n - 1].r);
    z++;

    printf("\nAfter Dead Code Elimination:\n");
    for (k = 0; k < z; k++)
    {
        printf("%c = %s\n", pr[k].l, pr[k].r);
    }

    // Step 2: Propagate Expressions (replace redundant variable usage)
    for (m = 0; m < z; m++)
    {
        tem = pr[m].r;
        for (j = m + 1; j < z; j++)
        {
            p = strstr(tem, pr[j].r); // find repeated subexpressions
            if (p)
            {
                t = pr[j].l;
                pr[j].l = pr[m].l; // replace redundant variable
                for (i = 0; i < z; i++)
                {
                    l = strchr(pr[i].r, t);
                    if (l)
                    {
                        a = l - pr[i].r;
                        pr[i].r[a] = pr[m].l;
                    }
                }
            }
        }
    }

    printf("\nAfter Common Subexpression Elimination:\n");
    for (i = 0; i < z; i++)
    {
        printf("%c = %s\n", pr[i].l, pr[i].r);
    }

    // Step 3: Remove Duplicate Expressions
    for (i = 0; i < z; i++)
    {
        for (j = i + 1; j < z; j++)
        {
            q = strcmp(pr[i].r, pr[j].r);
            if ((pr[i].l == pr[j].l) && !q)
            {
                pr[i].l = '\0'; // mark duplicate as deleted
            }
        }
    }

    // Step 4: Final Optimized Code Output
    printf("\nOptimized Code:\n");
    for (i = 0; i < z; i++)
    {
        if (pr[i].l != '\0')
        {
            printf("%c = %s\n", pr[i].l, pr[i].r);
        }
    }

    return 0;
}
