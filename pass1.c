#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void passOne(char label[10], char opcode[10], char operand[10], char code[10], char mnemonic[3]);
void display();

int main() {
    char label[10], opcode[10], operand[10];
    char code[10], mnemonic[3];

    passOne(label, opcode, operand, code, mnemonic);
    return 0;
}

void passOne(char label[10], char opcode[10], char operand[10], char code[10], char mnemonic[3]) {
    int locctr, start, length;

    FILE *fp1, *fp2, *fp3, *fp4, *fp5;

    // Open all files
    fp1 = fopen("input.txt", "r");
    fp2 = fopen("optab.txt", "r");
    fp3 = fopen("symtab.txt", "w");
    fp4 = fopen("intermediate.txt", "w");
    fp5 = fopen("length.txt", "w");

    // Check if files opened correctly
    if (!fp1 || !fp2 || !fp3 || !fp4 || !fp5) {
        printf("Error opening one or more files.\n");
        exit(1);
    }

    // Read first line from source program
    if (fscanf(fp1, "%s%s%s", label, opcode, operand) != 3) {
        printf("Error reading input file.\n");
        exit(1);
    }

    // Handle START directive
    if (strcmp(opcode, "START") == 0) {
        start = atoi(operand);      // convert starting address to integer
        locctr = start;

        // Write first line to intermediate (START line, no address)
        fprintf(fp4, "\t%s\t%s\t%s\n", label, opcode, operand);

        // Read next line
        fscanf(fp1, "%s%s%s", label, opcode, operand);
    } else {
        locctr = 0;
        start = 0;
    }

    // Process each source line until END
    while (strcmp(opcode, "END") != 0) {

        // Write current line with location counter to intermediate file
        fprintf(fp4, "%d\t%s\t%s\t%s\n", locctr, label, opcode, operand);

        // If label exists, enter it into SYMTAB
        if (strcmp(label, "**") != 0) {
            fprintf(fp3, "%s\t%d\n", label, locctr);
        }

        // Reset OPTAB pointer before searching
        rewind(fp2);

        // Search OPTAB for opcode
        int found = 0;
        while (fscanf(fp2, "%s%s", code, mnemonic) == 2) {
            if (strcmp(opcode, code) == 0) {
                locctr += 3;   // Instruction length for SIC machine
                found = 1;
                break;
            }
        }

        // If not found in OPTAB, handle assembler directives
        if (!found) {
            if (strcmp(opcode, "WORD") == 0) {
                locctr += 3;
            } else if (strcmp(opcode, "RESW") == 0) {
                locctr += (3 * atoi(operand));
            } else if (strcmp(opcode, "RESB") == 0) {
                locctr += atoi(operand);
            } else if (strcmp(opcode, "BYTE") == 0) {
                // Assuming BYTE occupies 1 byte (simple case)
                locctr += 1;
            } else {
                printf("Warning: Invalid opcode %s at address %d\n", opcode, locctr);
            }
        }

        // Read next line
        if (fscanf(fp1, "%s%s%s", label, opcode, operand) != 3)
            break;
    }

    // Write the END line to intermediate file
    fprintf(fp4, "%d\t%s\t%s\t%s\n", locctr, label, opcode, operand);

    // Calculate program length
    length = locctr - start;

    // Close all files
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    fclose(fp4);

    // Display all outputs
    display();

    // Write length to file
    fprintf(fp5, "%d", length);
    fclose(fp5);

    printf("\nProgram length: %d\n", length);
}

// Function to display file contents
void display() {
    int ch;
    FILE *fp1, *fp2, *fp3;

    // Display Input Table
    printf("\n===== INPUT TABLE =====\n");
    fp1 = fopen("input.txt", "r");
    if (fp1) {
        while ((ch = fgetc(fp1)) != EOF)
            putchar(ch);
        fclose(fp1);
    }

    // Display Intermediate File
    printf("\n\n===== INTERMEDIATE FILE =====\n");
    fp2 = fopen("intermediate.txt", "r");
    if (fp2) {
        while ((ch = fgetc(fp2)) != EOF)
            putchar(ch);
        fclose(fp2);
    }

    // Display Symbol Table
    printf("\n\n===== SYMBOL TABLE =====\n");
    fp3 = fopen("symtab.txt", "r");
    if (fp3) {
        while ((ch = fgetc(fp3)) != EOF)
            putchar(ch);
        fclose(fp3);
    }
}
