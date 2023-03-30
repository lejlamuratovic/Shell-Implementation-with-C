#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //getlogin() and getusername()
#include <limits.h> //HOST_NAME_MAX constant

//declaring functions
void prompt();
char* read_line(void);
char** split_line(char *line);

int main(void) {
    printf("\033]0;Emir and Lejla\007"); //sets the name of the shell to "Emir and Lejla"

    system("clear");

    prompt();

    return 0;
}

//prompt to display the machine name & username
void prompt() {
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
