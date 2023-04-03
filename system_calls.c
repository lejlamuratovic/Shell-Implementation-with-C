#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>


//this function provides the basic implementation of fork(), wait(), and exec() system calls;
void system_calls() {

    printf("Hello world! (pid: %d)\n", (int) getpid());
    int rc = fork();

    if (rc < 0) {
        //fork failed
        fprintf(stderr, "Fork failed\n");
        exit(1);
    } 
    
    else if (rc == 0) {
        //child process
        printf("Hello world! From child (pid: %d)\n", (int)getpid());
        char *args[3];
        args[0] = strdup("ls");
        args[1] = strdup("-a");
        args[2] = NULL;
        execvp(args[0], args);
        printf("If executing successful, this should not print");
    }
    
    else {
        //parent process
        int wc = wait(NULL); //wait for child to be executed
        printf("Hello world! I am the parent process (pid: %d) with a child process (pid: %d), and the value returned by the child process is: %d\n", (int)getpid(), rc, wc);
    }
}

void forkbomb() {
    char ch;
    printf("Are you sure? (y/n): ");
    scanf("%c", &ch);

    //if yes, execute
    if (ch=='y' || ch=='Y') {
        while(1) {
           fork();   
        } 

    }
    //if not, don't execute
    else if (ch=='n' || ch=='N') {

        printf("Forkbomb aborted\n");
    
    }
    
    else {
		printf("Invalid input\n");
	}
}

int main() {
    system_calls();
    printf("-------------------------\n");
    forkbomb();

    return 0;
}