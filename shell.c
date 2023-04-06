#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // getlogin() && getusername()
#include <limits.h> // HOST_NAME_MAX
#include <string.h> // strtok()
#include <sys/wait.h> // wait()
#include <ctype.h> // isspace()
#include <errno.h> // error codes for various system errors
#include <dirent.h> // functions to access directories and their contents
#include <sys/ioctl.h> // functions to control I/O operations on devices
#include <signal.h> // functions to handle SIGINT
#include <sys/statvfs.h> // functions to get information about file systems like df()
#include <sys/types.h>
#include <fcntl.h> // for flags in open


#include "my_wc.c"
#include "my_cmatrix.c"
#include "my_grep.c"
#include "my_df.c"

#define MAX_CMD_LEN 256
#define MAX_NUM_ARGUMENTS 16
#define MAX_LINE_LENGTH 1024
#define true 1

char delimiters[] = " \t\r\n\v\f"; // declare delimiters as a global variable

void prompt();
void main_loop();
void check_pipe(char *input, int *has_pipe, char **pipe_cmds);
void execute_pipe(char **pipe_cmds);
void execute_regular(char *input);
char** tokenize(char* str);
void redirectOut(char *fileName);
void redirectIn(char *fileName);
void execute_redirection(char *input, int in, int out);

int main(void) {
  signal(SIGINT, sigint_handler); // ctrl + c

  system("clear");

  printf("\033[0;32m"); // sets text color to green
  printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
  printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⡶⠟⠛⠛⠛⠛⠻⢶⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
  printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⣾⠋⣼⣿⠇⠀⠀⠀⠀⠀⠙⣷⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
  printf("⠀⠀⠀⠀⠀⣠⣤⣶⠀⢿⡀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡿⠀⣶⣤⣄⠀⠀⠀⠀⠀\n");
  printf("⠀⠀⠀⣴⠿⠿⣿⣿⣧⡈⠛⠷⠶⢶⣶⣶⡶⠶⠾⠛⢁⣼⣿⡿⠿⠿⣦⠀⠀⠀\n");
  printf("⠀⠀⢸⡇⢰⣶⡄⢹⣿⣿⣷⣶⣦⣤⣤⣤⣤⣴⣶⣾⣿⣿⠋⢰⣶⠆⢸⡇⠀⠀\n");
  printf("⠀⠀⠀⠳⣄⣉⣁⣼⣿⣿⣿⣿⡿⠋⣁⣈⠙⢿⣿⣿⣿⣿⣦⣈⣁⣤⠟⠀⠀⠀\n");
  printf("⠀⠀⠀⠀⠀⠙⠛⠿⢿⣿⣿⣿⣧⡈⠛⠛⢁⣼⣿⣿⣿⡿⠿⠛⠋⠀⠀⠀⠀⠀\n");
    printf("\033[0;33m"); // sets text color to yellow
  //printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢉⣉⣉⠁⠈⣉⣉⡉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
  printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣾⣿⣿⣿⣿⣿⣿⣷⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
  printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⣿⣿⣿⣿⣿⣿⣿⣿⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
  printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀\n\n\n\n\n");

  main_loop();

  return 0;

}

//prompt to display the machine name & username
void prompt() {
  printf("\033]0;Space Shell\007"); //sets the name of the shell to "Emir and Lejla"

  char *username;
  char hostname[HOST_NAME_MAX + 1];

  username = getlogin();
  gethostname(hostname, HOST_NAME_MAX + 1);

  printf("\033[1;32m"); // sets text color to neon green
  printf("%s@%s", username, hostname);
  printf("\033[0m"); //reset color

  printf(":~$ ");
}


void main_loop() {
  char input[MAX_LINE_LENGTH];

  /** here we save the standard input and output file descriptors using the dup() function before the loop, 
      and we restore them at the end of each iteration using the dup2() function. 
      In the end we close the copies of the file descriptors after the loop ends ----> this is used for reditection*/
  int stdin_copy = dup(STDIN_FILENO);
  int stdout_copy = dup(STDOUT_FILENO);

  while (true) {
    prompt();

    // get input
    if (fgets(input, MAX_LINE_LENGTH, stdin) == NULL) {
      printf("\n");
      break;
    }

    // remove trailing newline
    input[strcspn(input, "\n")] = 0;

    // check for pipe
    int has_pipe = 0;
    char *pipe_cmds[2];
    check_pipe(input, &has_pipe, pipe_cmds);

    //check for redirection
    int in = 0; 
    int out = 0;

    for (int i = 0; input[i] != '\0'; i++) {
      if (input[i] == '>') {
        out = 1;
        break;
      } 

      if(input[i] == '<') {
        in = 1;
        break;
      }
    }

    // execute command(s)
    if (has_pipe) {
      execute_pipe(pipe_cmds);
    } else if(out) {
      execute_redirection(input, in, out);
    } else if(in) {
      execute_redirection(input, in, out);
    } else {
      execute_regular(input);
    }

    // restore standard input and output
    dup2(stdin_copy, STDIN_FILENO);
    dup2(stdout_copy, STDOUT_FILENO);
  }

  close(stdin_copy);
  close(stdout_copy);
}


void redirectIn(char *fileName) {
  int in = open(fileName, O_RDONLY);
  dup2(in, 0);
  close(in);
}

void redirectOut(char *fileName) {
  int out = open(fileName, O_WRONLY | O_TRUNC | O_CREAT, 0600);
  dup2(out, 1);
  close(out);
}


//function to check if there is a pipe
void check_pipe(char *input, int *has_pipe, char **pipe_cmds) {
  *has_pipe = 0;
  for (int i = 0; input[i] != '\0'; i++) {
    if (input[i] == '|') {
      *has_pipe = 1;
      break;
    }
  }

  if (*has_pipe) {
    pipe_cmds[0] = strtok(input, "|");
    pipe_cmds[1] = strtok(NULL, "|");
  }
}


char** tokenize(char* str) {
  char* delimiters = " \t\n"; 

  char* token = strtok(str, delimiters);
  int i = 0;

  char** args = malloc(sizeof(char*) * (MAX_NUM_ARGUMENTS + 1));
  while (token != NULL && i < MAX_NUM_ARGUMENTS) {
    args[i] = token;
    token = strtok(NULL, delimiters);
    i++;
  }

  args[i] = NULL;

  return args;
}

//function that will execute pipes
void execute_pipe(char **pipe_cmds) {
  int pfd[2];
  int status;

  if (pipe(pfd) == -1) {
    perror("pipe");
    exit(1);
  }

  pid_t pid1 = fork();
  if (pid1 == 0) {
    close(STDOUT_FILENO);
    dup2(pfd[1], STDOUT_FILENO);
    close(pfd[0]);
    close(pfd[1]);

    char **args = tokenize(pipe_cmds[0]);

    execvp(args[0], args);
    perror(args[0]);
    exit(1);
  } else if (pid1 < 0) {
    perror("fork");
    exit(1);
  }

  pid_t pid2 = fork();
  if (pid2 == 0) {
    close(STDIN_FILENO);
    dup2(pfd[0], STDIN_FILENO);
    close(pfd[0]);
    close(pfd[1]);

    char **args = tokenize(pipe_cmds[1]);

    execvp(args[0], args);
    perror(args[0]);
    exit(1);
  } else if (pid2 < 0) {
    perror("fork");
    exit(1);
  }

  close(pfd[0]);
  close(pfd[1]);

  waitpid(pid1, &status, 0);
  waitpid(pid2, &status, 0);
}

//function to execute redirection 
void execute_redirection(char *input, int in, int out) {
  char *command;
  char *fileName;
  int status;

  if (in) {
    command = strtok(input, "<");
    fileName = strtok(NULL, delimiters);
    redirectIn(fileName);
  } else {
    command = strtok(input, ">");
    fileName = strtok(NULL, delimiters);
    redirectOut(fileName);
  }

  char **args = tokenize(command);

  pid_t pid = fork();
  if (pid == 0) {
    execvp(args[0], args);
    perror(args[0]);
    exit(1);
  } else if (pid < 0) {
    perror("fork");
    exit(1);
  } else {
    waitpid(pid, &status, 0);
  }
}


//function to execute regular commands
void execute_regular(char *input) {
  int status;
  char **args = tokenize(input);

  pid_t pid = fork();
  if (pid == 0) {

    if (strcmp(args[0], "wc") == 0) {
      // check if there is a file passed to wc
      if ((strcmp(args[0], "wc") == 0 && args[1] == NULL) || (strcmp(args[0], "wc") == 0 && args[1][0] == '-' && args[2] == NULL)) {
        fprintf(stderr, "Error: wc requires file argument\n");
      } 
      
      else {
        //used for wc to get the length of the array
        int argc = 0;
        while (args[argc] != NULL) {
          argc++;
        }

        my_wc(argc, args);
      } 
    }


    else if (strcmp(args[0], "grep") == 0) {
      if (args[1] == NULL || args[2] == NULL) {
      fprintf(stderr, "Error: grep requires both pattern and file arguments\n");
    } else {
      my_grep(args[1], args[2]);
      }
    }


    else if (strcmp(args[0], "df") == 0) {
      // df doesn't take any arguments by default, so we don't need to check anything here
      my_df(args[1]);
    }


    else if (strcmp(args[0], "cmatrix") == 0) {
      // cmatrix doesn't take any arguments by default, so we don't need to check anything here
      my_cmatrix(args[1]);
    }


    else if (strcmp(args[0], "clear") == 0) {
      system("clear");
    }


    else {
      // command not implemented
      fprintf(stderr, "%s: command not found\n", args[0]);
    }
  } 
        
  else if (pid > 0) {
    waitpid(pid, &status, 0);
  } 
  
  else {
    perror("Fork");
    exit(1);
  } 
}
