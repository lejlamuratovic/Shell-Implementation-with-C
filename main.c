#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //getlogin() and getusername()
#include <limits.h> //HOST_NAME_MAX constant
#include <string.h> //strtok()
#include <sys/wait.h> //wait()
#include <ctype.h> //isspace()

#define true 1
#define BUFFER_SIZE 4096

//declaring functions
void prompt();
char* read_line(void);
char** split_line(char *line);
char* read_line();
char** split_line(char *line);
void execArgs(char** parsed);
void execArgsPiped(char** parsed, char** parsedpipe);
void loop_input();
void execute_command(char* input);
void wc(char **filenames);
int count_words(char *line);

int main(void) {
    system("clear");

    loop_input();

    return 0;
}

//prompt to display the machine name & username
void prompt() {
    printf("\033]0;Emir and Lejla\007"); //sets the name of the shell to "Emir and Lejla"

    char *username;
    char hostname[HOST_NAME_MAX + 1];

    username = getlogin();
    gethostname(hostname, HOST_NAME_MAX + 1);

    //char cwd[PATH_MAX]; // this line of code stores the current working directory

    printf("\033[1;31m"); //set color to red
    printf("%s@%s", username, hostname);
    printf("\033[0m"); //reset color

    printf(":~$ ");
}

//read a line of text from standard input and returns a pointer to the input string
char* read_line()
{
    char *line = NULL;
    size_t buflen = 0;
    getline(&line, &buflen, stdin); //getline() automatically resizes the buffer pointed to by line to accommodate the input
    return line;
}

//take a string as input and returns an array of tokens - substrings of the input string that are separated by delimiters
char** split_line(char *line) {
    int length = 0;
    int capacity = 16;
    char **tokens = malloc(capacity * sizeof(char*)); //dynamically allocate memory for the array of tokens

    char *delimiters = " \t\r\n";
    char *token = strtok(line, delimiters); //split input into tokens

    while (token != NULL) {
        tokens[length] = token;
        length++;

        //resize the memory for tokens array to a new capacity if needed
        if (length >= capacity) {
            capacity = (int) (capacity * 1.5);
            tokens = realloc(tokens, capacity * sizeof(char*));
        }

        token = strtok(NULL, delimiters);
    }

    tokens[length] = NULL;
    return tokens;
}

//function where the commands are executed
void execArgs(char** parsed)
{
    // Forking a child
    pid_t pid = fork();

    if (pid == -1) {
        printf("\nFailed forking child..");
        return;
    } else if (pid == 0) {
        if (execvp(parsed[0], parsed) < 0) {
            printf("\nCould not execute command..");
        }
        exit(0);
    } else {
        // waiting for child to terminate
        wait(NULL);
        return;
    }
}

//function where piped commands are executed
void execArgsPiped(char** parsed, char** parsedpipe)
{
    // 0 is read end, 1 is write end
    int pipefd[2];
    pid_t p1, p2;

    if (pipe(pipefd) < 0) {
        printf("\nPipe could not be initialized");
        return;
    }
    p1 = fork();
    if (p1 < 0) {
        printf("\nCould not fork");
        return;
    }

    if (p1 == 0) {
        // Child 1 executing..
        // It only needs to write at the write end
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        if (execvp(parsed[0], parsed) < 0) {
            printf("\nCould not execute command 1..");
            exit(0);
        }
    } else {
        // Parent executing
        p2 = fork();

        if (p2 < 0) {
            printf("\nCould not fork");
            return;
        }

        // Child 2 executing..
        // It only needs to read at the read end
        if (p2 == 0) {
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            if (execvp(parsedpipe[0], parsedpipe) < 0) {
                printf("\nCould not execute command 2..");
                exit(0);
            }
        } else {
            // parent executing, waiting for two children
            wait(NULL);
            wait(NULL);
        }
    }
}

//function for finding piped commands (if any)
int parsePipe(char* str, char** strpiped)
{
    int i;
    for (i = 0; i < 2; i++) {
        strpiped[i] = strsep(&str, "|");
        if (strpiped[i] == NULL)
            break;
    }

    if (strpiped[1] == NULL)
        return 0; // returns zero if no pipe is found.
    else {
        return 1;
    }
}

//function to repeatadely ask user for input
void loop_input() {
    while (true) {
        prompt();
        char *line = read_line();


        if (line != NULL && strlen(line) > 1) {
            execute_command(line);
        }

        free(line);
    }
}

void execute_command(char* input) {
    // Split the input into arguments
    char** args = split_line(input);
    // Check if the command is "exit"
    if (strcmp(args[0], "exit") == 0) {
        // If it is, exit the program
        exit(0);
    }
    // Check if the command is empty
    if (args[0] == NULL) {
        // If it is, do nothing
        return;
    }
    // Check if the command is piped
    char* piped_args[2];
    int has_pipe = parsePipe(input, piped_args);
    if (has_pipe) {
        // If it is, execute the piped commands
        execArgsPiped(args, piped_args);
    } else {
        // If it is not, execute the command
        execArgs(args);
    }
}


//used for the wc command
int count_words(char *line) {
    int word_count = 0;
    int in_word = 0;

    for (int i = 0; line[i] != '\0'; i++) {
        if (!isspace(line[i])) {
            if (!in_word) {
                in_word = 1;
                word_count++;
            }
        } else {
            in_word = 0;
        }
    }

    return word_count;
}

//function that implements the basic functionality of a wc command
void wc(char **filenames) {
    int total_words = 0;
    int total_lines = 0;
    int total_bytes = 0;

    for (int i = 1; filenames[i] != NULL; i++) {
        int word_count = 0;
        int line_count = 0;

        FILE *file = fopen(filenames[i], "r");
        if (file == NULL) {
            printf("wc: %s: open: No such file or directory\n", filenames[i]);
            continue;
        }

        char buffer[BUFFER_SIZE];

        while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
            total_bytes += strlen(buffer);
            total_lines++;
            line_count++;
            word_count += count_words(buffer);
        }

        fclose(file);

        printf("%7d %7d %7d %s\n", line_count, word_count, total_bytes, filenames[i]);
        total_words += word_count;
    }

    if (filenames[2] != NULL) {
        printf("%7d %7d %7d total\n", total_lines, total_words, total_bytes);
    }
}

