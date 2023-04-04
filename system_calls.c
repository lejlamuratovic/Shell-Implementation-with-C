#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sched.h>


// this function provides the basic implementation of fork(), wait(), and exec() system calls
void sys_calls() {
    printf("Hello world! (pid: %d)\n", (int) getpid());
    int rc = fork();

    if (rc < 0) {
        // fork failed
        fprintf(stderr, "Fork failed\n");
        exit(1);
    } else if (rc == 0) {
        // child process
        printf("Hello world! From child (pid: %d)\n", (int)getpid());
        char *args[3];
        args[0] = strdup("ls");
        args[1] = strdup("-a");
        args[2] = NULL;
        execvp(args[0], args);
        printf("This line should not be printed\n");
    } else {
        // parent process
        int wc = wait(NULL); // wait for child to be executed
        printf("Hello world! I am the parent process (pid: %d) with a child process (pid: %d), and the value returned by the child process is: %d\n", (int)getpid(), rc, wc);
    }
}

void forkbomb() {
    char ch;

    while(1) {
        printf("Are you sure? (y/n): ");
        scanf(" %c", &ch);

        // if yes, execute
        if(ch=='y' || ch=='Y'){
            while(1) {
                fork();   
            } 
        }

        // if not, don't execute
        else if(ch=='n' || ch=='N'){
            printf("Smart choice\n");
            break;
        }

        else {
	    	printf("\nInvalid input\n");
	    }
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

void *child_fn(void *arg) {
    printf("Some example code running under child process.\n");

    int i, space, k = 0;
    int rows = 5;

    for (i = 1; i <= rows; ++i, k = 0) {
        for (space = 1; space <= rows - i; ++space) {
            printf("  ");
        }
        while (k != 2 * i - 1) {
            printf("* ");
            ++k;
      }
      printf("\n");
   }

   exit(0);
}

void sys_clone() {
    printf("Starting clone()...\n");

    void *stack = malloc(1024*1024);
    if (stack == NULL) {
        printf("Unable to allocate stack.\n");
        exit(EXIT_FAILURE);
    }

    pid_t pid = clone((int (*)(void *)) child_fn, stack + (1024 * 1024), SIGCHLD, NULL);
    if (pid == -1) {
        printf("Unable to create child process.\n");
        exit(EXIT_FAILURE);
    }

    printf("Waiting for child process...\n");
    waitpid(pid, NULL, 0);

    free(stack);

    printf("Child process terminated.\n");
}

int main() {
    int choice;
    printf("Enter a number to choose a function to run:\n");
    printf("1. clone()\n");
    printf("2. execle()\n");
    printf("3. fork(), wait(), and exec()\n");
    printf("4. forkbomb\n");
    printf("\nChoose what you want to execute: ");
    scanf("%d", &choice);
    
    switch (choice) {
        case 1:
            printf("clone(): \n");
            sys_clone();
            break;
        case 2:
            printf("execle(): \n");
            sys_execle();
            break;
        case 3:
            printf("Basic system calls: \n");
            sys_calls();
            break;
        case 4:
            printf("forkbomb: \n");
            forkbomb();
            break;
        default:
            printf("Invalid choice\n");
            break;
    }

    return 0;
}

