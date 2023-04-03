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


void df(char* option) {
    struct statvfs vfs;
    
    if (statvfs(".", &vfs) == -1) {
        perror("statvfs");
        return;
    }

    if (strcmp(option, "-h") == 0) {

        // use the SI suffixes (powers of 1000) for the human-readable output
        printf("Filesystem    Size  Used Avail Use%% Mounted on\n");
        printf("/dev/root    %4.1fG %4.1fG %4.1fG %3.0f%% /",
              (float)vfs.f_frsize * vfs.f_blocks / 1e9,
              (float)vfs.f_frsize * (vfs.f_blocks - vfs.f_bfree) / 1e9,
              (float)vfs.f_frsize * vfs.f_bfree / 1e9,
              100.0 * (vfs.f_blocks - vfs.f_bfree) / vfs.f_blocks);
    } 
    
    else if (strcmp(option, "-H") == 0) {
    
        // use the decimal SI suffixes (powers of 1024) for the human-readable output
        printf("Filesystem    Size  Used Avail Use%% Mounted on\n");
        printf("/dev/root    %4.1fGi %4.1fGi %4.1fGi %3.0f%% /",
              (float)vfs.f_frsize * vfs.f_blocks / 1e9,
              (float)vfs.f_frsize * (vfs.f_blocks - vfs.f_bfree) / 1e9,
              (float)vfs.f_frsize * vfs.f_bfree / 1e9,
              100.0 * (vfs.f_blocks - vfs.f_bfree) / vfs.f_blocks);
    } 
    
    else {
      
        // use the default format for the non-option case
        printf("Filesystem     1K-blocks    Used Available Use%% Mounted on\n");
        printf("/dev/root    %10lu %6lu %10lu %3.0f%% /",
              vfs.f_blocks * (vfs.f_frsize / 1024),
              (vfs.f_blocks - vfs.f_bfree) * (vfs.f_frsize / 1024),
              vfs.f_bfree * (vfs.f_frsize / 1024),
              100.0 * (vfs.f_blocks - vfs.f_bfree) / vfs.f_blocks);
    }
}


