#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>


//this function provides the basic implementation of fork(), wait(), and exec() system calls;
void sys_calls() {
    printf("Hello world! (pid: %d)\n", (int) getpid());
    int rc = fork();

    if (rc < 0) {
        //fork failed
        fprintf(stderr, "Fork failed\n");
        exit(1);
    } else if (rc == 0) {
        //child process
        printf("Hello world! From child (pid: %d)\n", (int)getpid());
        char *args[3];
        args[0] = strdup("ls");
        args[1] = strdup("-a");
        args[2] = NULL;
        execvp(args[0], args);
        printf("This line should not be printed\n");
    } else {
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
    if(ch=='y' || ch=='Y'){
        while(1) {
           fork();   
        } 

    }
    //if not, don't execute
    else if(ch=='n' || ch=='N'){

        printf("Smart choice\n");
    
    }
    else{
		printf("Invalid input\n");
	}
}

void sys_execle() {
    // define environment variables
    char *env_vars[] = {"HOME=/home/user", "USER=user", NULL};
    
    // execute the "ls" command with environment variables
    if (execle("/bin/ls", "ls", "-l", NULL, env_vars) == -1) {
        perror("execle");
        exit(EXIT_FAILURE);
    }

    // this code should never be reached
    printf("This line should not be printed\n");
}

int main() {
    printf("Basic system calls: \n");
    sys_calls();
    printf("\n\n-------------------------\n\n");
    printf("forkbomb: \n");
    forkbomb();
    printf("\n\n-------------------------\n\n");
    printf("execle(): \n");
    sys_execle();

    return 0;
}
