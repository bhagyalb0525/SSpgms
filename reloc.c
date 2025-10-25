#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
    FILE *fp;
    int i, j, startAddr, progLen, loadAddr, relocAddr, staddr1;
    char name[10], line[100], name1[10], staddr[10], lengthStr[10];

    printf("Enter Program Name: ");
    scanf("%s", name);

    printf("Enter Load Address (in hex): ");
    scanf("%x", &loadAddr);

    fp = fopen("input.txt", "r");
    if (fp == NULL) {
        printf("Error: Cannot open input file.\n");
        return 1;
    }

    // Read Header Record
    fscanf(fp, "%s", line);

    // Extract Program Name
    i = 2; j = 0;
    while (line[i] != '^' && line[i] != '\0') {
        name1[j++] = line[i++];
    }
    name1[j] = '\0';

    // Extract Starting Address
    i++; j = 0;
    while (line[i] != '^' && line[i] != '\0') {
        staddr[j++] = line[i++];
    }
    staddr[j] = '\0';
    startAddr = (int)strtol(staddr, NULL, 16);

    // Extract Program Length
    i++; j = 0;
    while (line[i] != '^' && line[i] != '\0') {
        lengthStr[j++] = line[i++];
    }
    lengthStr[j] = '\0';
    progLen = (int)strtol(lengthStr, NULL, 16);

    // Compute Relocation Address
    relocAddr = loadAddr - startAddr;

    printf("\nLoad Address: %04X", loadAddr);
    printf("\nProgram: %s", name1);
    printf("\nProgram Length: %d bytes\n\n", progLen);
    printf("Address\tContents\n");

    // Read remaining lines until EOF
    while (fscanf(fp, "%s", line) != EOF) {
        if (line[0] == 'T') {
            // Extract starting address of this text record
            i = 2; j = 0;
            while (line[i] != '^' && line[i] != '\0') {
                staddr[j++] = line[i++];
            }
            staddr[j] = '\0';
            staddr1 = (int)strtol(staddr, NULL, 16);
            staddr1 += relocAddr; // Apply relocation

            // Skip next 2 fields (^length^)
            int caretCount = 0;
            while (line[i] != '\0' && caretCount < 2) {
                if (line[i] == '^')
                    caretCount++;
                i++;
            }

            // Print object codes with relocated addresses
            while (line[i] != '\0') {
                if (line[i] != '^') {
                    printf("%04X\t%c%c%c%c%c%c\n", staddr1, line[i], line[i+1],
                           line[i+2], line[i+3], line[i+4], line[i+5]);
                    staddr1 += 3;
                    i += 6;
                } else {
                    i++;
                }
            }
        } else if (line[0] == 'E') {
            break;
        }
    }

    fclose(fp);
    return 0;
}
