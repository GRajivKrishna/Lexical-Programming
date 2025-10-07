#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Maximum number of labels we may record */
int label[50];
int no = 0;

/* Helper used to check whether a label number was recorded earlier */
int check_label(int k)
{
    int i;
    for (i = 0; i < no; i++)
    {
        if (k == label[i])
            return 1;
    }
    return 0;
}

int main()
{
    FILE *fp1, *fp2;
    char fname[100];
    char op[32];
    char operand1[32], operand2[32], result[32];
    int ch;           /* use int to test EOF safely */
    int i = 0;        /* generic counters */

    printf("\nEnter filename of the intermediate code: ");
    /* read filename (no & for arrays) */
    if (scanf("%99s", fname) != 1)
    {
        fprintf(stderr, "No filename provided.\n");
        return 1;
    }

    fp1 = fopen(fname, "r");
    fp2 = fopen("target.txt", "w");
    if (fp1 == NULL || fp2 == NULL)
    {
        fprintf(stderr, "\nError opening file(s).\n");
        if (fp1) fclose(fp1);
        if (fp2) fclose(fp2);
        return 1;
    }

    /*
      Input format assumptions (based on your sample input):
      - Each statement starts with an operator token 'op'
        Examples:
          + t1 t3 t4     (op = "+", operand1 = "t1", operand2 = "t3", result = "t4")
          / t3 t2 t2     (op = "/", operand1 = "t3", operand2 = "t2", result = "t2")
          uminus t2 t2   (op = "uminus", operand1 = "t2", result = "t2")
          print t2       (op = "print", result = "t2")
          goto 10        (op = "goto", label-number = "10")
          []= a i x      (op = "[]=", array base a, index i, value x) 
    */

    /* Read tokens until EOF. Each statement begins with a token 'op' */
    while (fscanf(fp1, "%31s", op) == 1)
    {
        /* Blank line for readability in target file */
        fprintf(fp2, "\n");

        /* If the current input position corresponds to a label number we recorded earlier,
           print the label marker before emitting code for this statement.
           Here we treat 'i' as statement count starting from 1 */
        i++;
        if (check_label(i))
            fprintf(fp2, "label#%d:\n", i);

        /* Handle named multi-character ops first */
        if (strcmp(op, "print") == 0)
        {
            /* print <result>  --> OUT <result> */
            if (fscanf(fp1, "%31s", result) != 1) result[0] = '\0';
            fprintf(fp2, "\tOUT %s\n", result);
        }
        else if (strcmp(op, "goto") == 0)
        {
            /* goto <labelnum> */
            if (fscanf(fp1, "%31s", operand1) != 1) operand1[0] = '\0';
            /* emit unconditional jump to label#<operand1> */
            fprintf(fp2, "\tJMP label#%s\n", operand1);
            /* record that label number so we can print label markers later */
            label[no++] = atoi(operand1);
        }
        else if (strcmp(op, "[]=") == 0)
        {
            /* []= base index result  --> STORE base[index], result */
            if (fscanf(fp1, "%31s %31s %31s", operand1, operand2, result) != 3)
            {
                operand1[0] = operand2[0] = result[0] = '\0';
            }
            fprintf(fp2, "\tSTORE %s[%s], %s\n", operand1, operand2, result);
        }
        else if (strcmp(op, "uminus") == 0)
        {
            /* uminus operand result  -> LOAD -operand,STORE result */
            if (fscanf(fp1, "%31s %31s", operand1, result) != 2)
            {
                operand1[0] = result[0] = '\0';
            }
            fprintf(fp2, "\tLOAD -%s, R1\n", operand1);
            fprintf(fp2, "\tSTORE R1, %s\n", result);
        }
        else
        {
            /* Assume op is a single character operator like + - * / % = > < */
            char opc = op[0];

            switch (opc)
            {
            case '*':
            case '+':
            case '-':
            case '/':
            case '%':
                /* binary operator: op operand1 operand2 result */
                if (fscanf(fp1, "%31s %31s %31s", operand1, operand2, result) != 3)
                {
                    operand1[0] = operand2[0] = result[0] = '\0';
                }
                /* Load operands into registers and perform operation */
                fprintf(fp2, "\tLOAD %s, R0\n", operand1);
                fprintf(fp2, "\tLOAD %s, R1\n", operand2);
                if (opc == '*')
                    fprintf(fp2, "\tMUL R1, R0\n");
                else if (opc == '+')
                    fprintf(fp2, "\tADD R1, R0\n");
                else if (opc == '-')
                    fprintf(fp2, "\tSUB R1, R0\n");
                else if (opc == '/')
                    fprintf(fp2, "\tDIV R1, R0\n");
                else if (opc == '%')
                {
                    /* For modulus, compute DIV then remainder logic (kept simple) */
                    fprintf(fp2, "\tDIV R1, R0\n");
                    fprintf(fp2, "\tMOD R1, R0\n"); /* MOD is illustrative */
                }
                fprintf(fp2, "\tSTORE R0, %s\n", result);
                break;

            case '=':
                /* assignment: = operand result  -> STORE result, operand (or MOV) */
                if (fscanf(fp1, "%31s %31s", operand1, result) != 2)
                {
                    operand1[0] = result[0] = '\0';
                }
                /* We'll load operand1 then store into result */
                fprintf(fp2, "\tLOAD %s, R0\n", operand1);
                fprintf(fp2, "\tSTORE R0, %s\n", result);
                break;

            case '>':
            case '<':
                /* relational jump: > operand1 operand2 labelnum  (JGT operand2, label#labelnum) */
                if (fscanf(fp1, "%31s %31s %31s", operand1, operand2, result) != 3)
                {
                    operand1[0] = operand2[0] = result[0] = '\0';
                }
                fprintf(fp2, "\tLOAD %s, R0\n", operand1);
                if (opc == '>')
                    fprintf(fp2, "\tJGT %s, label#%s\n", operand2, result);
                else
                    fprintf(fp2, "\tJLT %s, label#%s\n", operand2, result);
                label[no++] = atoi(result);
                break;

            default:
                /* Unknown operator token — consume the rest of the line to avoid infinite loop */
                /* Attempt to skip rest of tokens on this line */
                if (fgets(result, sizeof(result), fp1) == NULL)
                {
                    /* nothing */
                }
                fprintf(fp2, "\t; Unknown op '%s' — skipped\n", op);
                break;
            } /* switch */
        }     /* else single-char ops */
    }         /* while reading tokens */

    fclose(fp1);
    fclose(fp2);

    /* Now print the generated target file to stdout */
    fp2 = fopen("target.txt", "r");
    if (fp2 == NULL)
    {
        fprintf(stderr, "Error opening the generated target file.\n");
        return 1;
    }

    while ((ch = fgetc(fp2)) != EOF)
    {
        putchar(ch);
    }
    fclose(fp2);

    return 0;
}
