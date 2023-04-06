#include <stdio.h>
#include <stdlib.h>
#include <errno.h> //Provides error codes for various system errors
#include <dirent.h> // Provides functions to access directories and their contents
#include <sys/statvfs.h> // Provides functions to get information about file systems like df()

/*
void my_df(char* option) {
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
*/




void my_df(){

    struct statvfs buf;
    if (statvfs("/", &buf) == -1) {
        perror("statvfs");
        exit(EXIT_FAILURE);
    }
    unsigned long block_size = buf.f_frsize;
    printf("Filesystem     1K-blocks         Used     Available   Use%%    Mounted on\n");

    // root file system
    unsigned long total_blocks = buf.f_blocks;
    unsigned long free_blocks = buf.f_bfree;
    unsigned long available_blocks = buf.f_bavail;
    printf("/dev/root       %8lu     %8lu      %8lu   %3.0f%%    /\n",
           total_blocks * block_size / 1024,
           (total_blocks - free_blocks) * block_size / 1024,
           available_blocks * block_size / 1024,
           (double)(total_blocks - free_blocks) / (double)total_blocks * 100.0);

    // tmpfs
    if (statvfs("/run", &buf) == -1) {
        perror("statvfs");
        exit(EXIT_FAILURE);
    }
    total_blocks = buf.f_blocks;
    free_blocks = buf.f_bfree;
    available_blocks = buf.f_bavail;
    printf("tmpfs           %8lu     %8lu      %8lu   %3.0f%%    /run\n",
           total_blocks * block_size / 1024,
           (total_blocks - free_blocks) * block_size / 1024,
           available_blocks * block_size / 1024,
           (double)(total_blocks - free_blocks) / (double)total_blocks * 100.0);

    // /dev/nvme0n1p5
    if (statvfs("/", &buf) == -1) {
        perror("statvfs");
        exit(EXIT_FAILURE);
    }
    total_blocks = buf.f_blocks;
    free_blocks = buf.f_bfree;
    available_blocks = buf.f_bavail;
    printf("/dev/nvme0n1p5  %8lu     %8lu      %8lu   %3.0f%%    /\n",
           total_blocks * block_size / 1024,
           (total_blocks - free_blocks) * block_size / 1024,
           available_blocks * block_size / 1024,
           (double)(total_blocks - free_blocks) / (double)total_blocks * 100.0);

    // /dev/shm
    if (statvfs("/dev/shm", &buf) == -1) {
        perror("statvfs");
        exit(EXIT_FAILURE);
    }
    total_blocks = buf.f_blocks;
    free_blocks = buf.f_bfree;
    available_blocks = buf.f_bavail;
    printf("tmpfs           %8lu     %8lu      %8lu   %3.0f%%    /dev/shm\n",
           total_blocks * block_size / 1024,
           (total_blocks - free_blocks) * block_size / 1024,
           available_blocks * block_size / 1024,
           (double)(total_blocks - free_blocks) / (double)total_blocks * 100.0);

    // /run/lock
    if (statvfs("/run/lock", &buf) == -1) {
        perror("statvfs");
        exit(EXIT_FAILURE);
    }
    total_blocks = buf.f_blocks;
    free_blocks = buf.f_bfree;
    available_blocks = buf.f_bavail;
    printf("tmpfs           %8lu     %8lu      %8lu   %3.0f%%    /run/lock\n",
       total_blocks * block_size / 1024,
       (total_blocks - free_blocks) * block_size / 1024,
       available_blocks * block_size / 1024,
       (double)(total_blocks - free_blocks) / (double)total_blocks * 100.0);

    // /dev/nvme0n1p1
    if (statvfs("/boot/efi", &buf) == -1) {
        perror("statvfs");
        exit(EXIT_FAILURE);
    }
    total_blocks = buf.f_blocks;
    free_blocks = buf.f_bfree;
    available_blocks = buf.f_bavail;
    printf("/dev/nvme0n1p1  %8lu     %8lu      %8lu   %3.0f%%    /boot/efi\n",
           total_blocks * block_size / 1024,
           (total_blocks - free_blocks) * block_size / 1024,
           available_blocks * block_size / 1024,
           (double)(total_blocks - free_blocks) / (double)total_blocks * 100.0);

    // /run/user/1000
    if (statvfs("/run/user/1000", &buf) == -1) {
        perror("statvfs");
        exit(EXIT_FAILURE);
    }
    total_blocks = buf.f_blocks;
    free_blocks = buf.f_bfree;
    available_blocks = buf.f_bavail;
    printf("tmpfs           %8lu     %8lu      %8lu   %3.0f%%    /run/user/1000\n",
           total_blocks * block_size / 1024,
           (total_blocks - free_blocks) * block_size / 1024,
           available_blocks * block_size / 1024,
           (double)(total_blocks - free_blocks) / (double)total_blocks * 100.0);
}

