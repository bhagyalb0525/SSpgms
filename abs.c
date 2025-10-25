#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
    FILE *fp;
    int i, j, staddr1;
    char name[10], line[100], name1[10], staddr[10];

    printf("Enter Program Name: ");
    scanf("%s", name);

    fp = fopen("input.txt", "r");
    if (fp == NULL) {
        printf("Error: Cannot open input file.\n");
        return 1;
    }

    // Read header record
    fscanf(fp, "%s", line);

    // Extract program name correctly (between first and second '^')
    i = 2; j = 0;
    while (line[i] != '^' && line[i] != '\0') {
        name1[j++] = line[i++];
    }
    name1[j] = '\0';

    printf("\nProgram name from object file: %s\n", name1);

    // Check if the entered name matches program name in object file
    if (strcmp(name, name1) == 0) {
        printf("\nAddress\tContents\n");

        // Read subsequent records until EOF
        while (fscanf(fp, "%s", line) != EOF) {

            // If line starts with 'T', it's a Text record
            if (line[0] == 'T') {
                // Extract starting address (after 2nd '^')
                i = 2; j = 0;
                while (line[i] != '^' && line[i] != '\0') {
                    staddr[j++] = line[i++];
                }
                staddr[j] = '\0';

                // Convert starting address (hex string) to integer
                staddr1 = (int)strtol(staddr, NULL, 16);

                // Move to object codes (after skipping two more '^' fields)
                // Each '^' separates address, length, and codes
                int caretCount = 0;
                while (line[i] != '\0' && caretCount < 2) {
                    if (line[i] == '^')
                        caretCount++;
                    i++;
                }

                // Now 'i' is at the start of object codes
                while (line[i] != '\0') {
                    if (line[i] != '^') {
                        // Print memory address and next 6 hex digits (each obj code = 3 bytes)
                        printf("%04X\t%c%c%c%c%c%c\n", staddr1, line[i], line[i+1], line[i+2], line[i+3], line[i+4], line[i+5]);
                        staddr1 += 3;
                        i += 6;
                    } else {
                        i++;
                    }
                }
            }

            // Stop if End record found
            else if (line[0] == 'E') {
                break;
            }
        }
    } else {
        printf("Program name mismatch!\n");
    }

    fclose(fp);
    return 0;
}
