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

#include "wc.c"
#include "df.c"
#include "grep.c"
#include "cmatrix.c"

#define MAX_CMD_LEN 256
#define MAX_NUM_ARGUMENTS 16
#define MAX_LINE_LENGTH 1024
#define true 1

void prompt();
void main_loop();


int main(void) {
    signal(SIGINT, sigint_handler); // ctrl + c

    system("clear");

    main_loop();

    return 0;
}



//prompt to display the machine name & username
void prompt() {
  printf("\033]0;Emir and Lejla\007"); //sets the name of the shell to "Emir and Lejla"

  char *username;
  char hostname[HOST_NAME_MAX + 1];

  username = getlogin();
  gethostname(hostname, HOST_NAME_MAX + 1);

  printf("\033[1;31m"); //set color to red
  printf("%s@%s", username, hostname);
  printf("\033[0m"); //reset color

  printf(":~$ ");
}




void main_loop() {
  char *cmd[MAX_CMD_LEN];
  char *args[MAX_NUM_ARGUMENTS];
  char input[MAX_CMD_LEN];
  int status;
  char *delimiters = " \t\r\n";
  int pfd[2];

  while (1) {
    prompt();
    fgets(input, MAX_CMD_LEN, stdin);
    input[strlen(input) - 1] = '\0';

    // check if the input contains a pipe character
    int has_pipe = 0;
    for (int i = 0; input[i] != '\0'; i++) {
      if (input[i] == '|') {
        has_pipe = 1;
        break;
      }
    }

    if (has_pipe) {
      // split the input into two commands
      char *pipe_cmds[2];
      pipe_cmds[0] = strtok(input, "|");
      pipe_cmds[1] = strtok(NULL, "|");

      // create a pipe
      if (pipe(pfd) == -1) {
        perror("pipe");
        exit(1);
      }

      // fork the first child to execute the first command
      pid_t pid1 = fork();
      if (pid1 == 0) {
        // redirect the standard output to the write end of the pipe
        close(STDOUT_FILENO);
        dup2(pfd[1], STDOUT_FILENO);
        close(pfd[0]);
        close(pfd[1]);

        // parse the arguments of the first command
        char *token = strtok(pipe_cmds[0], delimiters);
        int i = 0;
        while (token != NULL && i < MAX_NUM_ARGUMENTS) {
          args[i] = token;
          token = strtok(NULL, delimiters);
          i++;
        }
        args[i] = NULL;

        // execute the first command
        execvp(args[0], args);
        perror(args[0]);
        exit(1);
      } else if (pid1 < 0) {
        perror("fork");
        exit(1);
      }

      // fork the second child to execute the second command
      pid_t pid2 = fork();
      if (pid2 == 0) {
        // redirect the standard input to the read end of the pipe
        close(STDIN_FILENO);
        dup2(pfd[0], STDIN_FILENO);
        close(pfd[0]);
        close(pfd[1]);

        // parse the arguments of the second command
        char *token = strtok(pipe_cmds[1], delimiters);
        int i = 0;
        while (token != NULL && i < MAX_NUM_ARGUMENTS) {
          args[i] = token;
          token = strtok(NULL, delimiters);
          i++;
        }
        args[i] = NULL;

        // execute the second command
        execvp(args[0], args);
        perror(args[0]);
        exit(1);
      } else if (pid2 < 0) {
        perror("fork");
        exit(1);
      }

      // close the read and write ends of the pipe in the parent process
      close(pfd[0]);
      close(pfd[1]);

      // wait for the child processes to complete
      waitpid(pid1, &status, 0);
      waitpid(pid2, &status, 0);
    }

    // if it doesn't have piping

    else {

      char *token = strtok(input, delimiters);
      int i = 0;
      while (token != NULL && i < MAX_NUM_ARGUMENTS) {
        args[i] = token;
        token = strtok(NULL, " ");
        i++;
      }

      args[i] = NULL;

      pid_t pid = fork();
        if (pid == 0) {

          if (strcmp(args[0], "wc") == 0) {
            // check if there is a file passed to wc
            if ((strcmp(args[0], "wc") == 0 && args[1] == NULL) || (strcmp(args[0], "wc") == 0 && args[1][0] == '-' && args[2] == NULL)){
              printf("Please provide a file name to wc command\n");
            } else {
                execvp("wc", args);
                perror(args[0]);
                exit(1);
                }
          }

          else if (strcmp(args[0], "grep") == 0) {
            // check if there is a pattern and a file passed to grep
            if (args[1] == NULL || args[2] == NULL) {
              printf("Please provide a pattern and file name to grep command\n");
          } else {
              execvp("grep", args);
              perror(args[0]);
              exit(1);
            }
          }

          else if (strcmp(args[0], "df") == 0) {
            // df doesn't take any arguments by default, so we don't need to check anything here
              execvp("df", args);
              perror(args[0]);
              exit(1);
          }

            else if (strcmp(args[0], "cmatrix") == 0) {
                cmatrix(args[1]);
            }

          else if (strcmp(args[0], "clear") == 0) {
            //cmatrix doesn't take any arguments by default
            system("clear");
          }

          else {
            // command not implemented
            fprintf(stderr, "%s: command not found\n", args[0]);
          }

        } else if (pid > 0) {
            waitpid(pid, &status, 0);

        } else {
            perror("Fork");
            exit(1);
            }
        }
    }
}
