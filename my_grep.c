#include <stdio.h>
#include <string.h>

#define BUFFER_LENGTH 1024

int my_grep(char* pattern, char* filename) {
    char lineBuffer[BUFFER_LENGTH+1];
    FILE *fp = NULL;
    size_t count = 0;

    fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error: Unable to open file %s.\n", filename);
        return -1;
    }

    while (fgets(lineBuffer, BUFFER_LENGTH, fp)) {
        if (strstr(lineBuffer, pattern)) {
            count++;
            printf("%s", lineBuffer);
        }
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