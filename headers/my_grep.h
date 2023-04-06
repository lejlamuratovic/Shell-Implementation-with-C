#include <stdio.h>
#include <string.h>

#define BUFFER_LENGTH 1024

int my_grep(char* pattern, char* filename) {
    char lineBuffer[BUFFER_LENGTH+1];
    FILE *fp = NULL;
    size_t count = 0;

    // open the file for reading 
    fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error: Unable to open file %s.\n", filename);
        return -1;
    }

    // read each line of file
    while (fgets(lineBuffer, BUFFER_LENGTH, fp)) {
        // check if the line contains the pattern
        if (strstr(lineBuffer, pattern)) {
            count++;
            printf("%s", lineBuffer);
        }
        // check for any errors while reading file
        if (ferror(fp)) {
            fprintf(stderr, "Error: Failed to read file %s.\n", filename);
            break;
        }
    }

    fclose(fp);

    if (count == 0) {
        printf("'%s' not found in file '%s'\n", pattern, filename);
    } else {
        printf("'%s' was found %lu time(s) in file '%s'\n", pattern, count, filename);
    }

    return 0;
}