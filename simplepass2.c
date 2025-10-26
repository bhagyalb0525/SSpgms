#include <stdio.h>
#include <string.h>

int main() {
    FILE *fin, *optab, *symtab;
    char label[20], opcode[20], operand[20], loc[10];
    char opname[20], opcodeval[10];
    char symname[20], symaddr[10];
    char start[10];

    fin = fopen("input.txt", "r");
    optab = fopen("optab.txt", "r");
    symtab = fopen("symbol.txt", "r");

    // Read first line (check if START)
    fscanf(fin, "%s %s %s %s", loc, label, opcode, operand);
    if (strcmp(opcode, "START") == 0) {
        strcpy(start, operand);  // store starting address
        printf("H^%s^00%s^\n", label, start);
        fscanf(fin, "%s %s %s %s", loc, label, opcode, operand);
    }

    printf("T^");

    // Process lines till END
    while (strcmp(opcode, "END") != 0) {
        int found = 0;

        // Search opcode in OPTAB
        rewind(optab);
        while (fscanf(optab, "%s %s", opname, opcodeval) != EOF) {
            if (strcmp(opcode, opname) == 0) {
                found = 1;

                // Search operand in SYMTAB
                rewind(symtab);
                while (fscanf(symtab, "%s %s", symaddr, symname) != EOF) {
                    if (strcmp(operand, symname) == 0) {
                        printf("%s%s^", opcodeval, symaddr);
                        break;
                    }
                }
                break;
            }
        }

        // Handle WORD & BYTE
        if (found == 0) {
            if (strcmp(opcode, "WORD") == 0)
                printf("0000%s^", operand);
            else if (strcmp(opcode, "BYTE") == 0)
                printf("%d^", operand[2]); // prints ASCII value of character
        }

        // Read next line
        fscanf(fin, "%s %s %s %s", loc, label, opcode, operand);
    }

    printf("\nE^00%s^\n", start);

    fclose(fin);
    fclose(optab);
    fclose(symtab);
    return 0;
}
