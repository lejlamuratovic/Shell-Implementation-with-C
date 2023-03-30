#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //getlogin() and getusername()
#include <limits.h> //HOST_NAME_MAX constant

//declaring libraries
void prompt();

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
