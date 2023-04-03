#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //getlogin() && getusername()
#include <limits.h> //HOST_NAME_MAX
#include <string.h> //strtok()
#include <sys/wait.h> //wait()
#include <ctype.h> //isspace()
#include <errno.h> //Provides error codes for various system errors
#include <dirent.h> // Provides functions to access directories and their contents
#include <sys/ioctl.h> //Provides functions to control I/O operations on devices
#include <signal.h> // Provides functions to handle signals like SIGINT
#include <sys/statvfs.h> // Provides functions to get information about file systems like df()

#define BUFFER_LENGTH 1024
#define BUFFER_SIZE 4096

void grep(char* pattern, char* filename) {
    
    char lineBuffer[BUFFER_LENGTH+1];
    FILE *fp = NULL;
    size_t count = 0;

    fp = fopen(filename, "r");
    
    if(!fp) {
        fprintf(stderr, "Error: Unable to open file %s.\n", filename);
        return;
    }
    
    while(fgets(lineBuffer, BUFFER_LENGTH, fp)) {
        if(strstr(lineBuffer, pattern)) {
            count++;
            printf("%s", lineBuffer);
        }
        if(ferror(fp)) {
            fprintf(stderr, "Error: Failed to read file %s.\n", filename);
            break;
        }
    }

    fclose(fp);

    if(count == 0) {
        printf("'%s' not found in file '%s'\n", pattern, filename);
    }
    else {
        printf("'%s' was found %lu time(s) in file '%s'\n", pattern, count, filename);
    }
}