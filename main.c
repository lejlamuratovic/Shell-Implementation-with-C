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

#define MAX_CMD_LEN 256
#define MAX_NUM_ARGUMENTS 16
#define BUFFER_LENGTH 1024
#define BUFFER_SIZE 4096

#define R(t,x,y) f=x;x-=t*y;y+=t*f;f=(3-x*x-y*y)/2;x*=f;y*=f; //donut()
#define MAX_LINE_LENGTH 1024
#define true 1

void prompt();
void main_loop();
int wc(int argc, char** argv);
void wc_results();
void grep(char* pattern, char* filename);
void cmatrix();
void cmatrix2();
void df();
int donut();

// Used for cmatrix (ctrl + C)
// Declare a global flag variable to indicate if SIGINT has been received
int stop = 0;
// Define a signal handler for SIGINT
void sigint_handler(int sig) {
    printf("\n");
    stop = 1;
}


int main(void) {

    signal(SIGINT, sigint_handler); //ctrl + c stuff

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

    //char cwd[PATH_MAX]; // this line of code stores the current working directory

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




void wc_results(int lines, int words, int chars, int bytes,int lineFlag,int wordFlag,int charFlag,int byteFlag, char* filename) {
  printf(" ");

  if (lineFlag==1) {printf("%d\t",lines);};
  if (wordFlag==1) {printf("%d\t",words);};
  if (charFlag==1) {printf("%d\t",chars);};
  if (byteFlag==1) {printf("%d\t ",bytes);};

  if(lineFlag == 0 && wordFlag == 0 && charFlag == 0 && byteFlag == 0){
    printf("%d\t%d\t%d\t",lines,words,chars);
  }

  printf("%s\n",filename);
}




int wc(int argc, char** argv) {
    FILE *file;
    int returnValue = 0;
    int byteCount = 0;
    int lineCount = 0;
    int charCount = 0;
    int wordCount = 0;
    int totalByteCount = 0;
    int totalLineCount = 0;
    int totalCharCount = 0;
    int totalWordCount = 0;
    int byteFlag = 0;
    int lineFlag = 0;
    int charFlag = 0;
    int wordFlag = 0;
    int startingFile = 0;
    int isInsideWord = 0;
    int i;
    for(i = 1; i < argc; i++){
      int j;
      if(startingFile != 0){
        break;
      }
      for(j = 0; j<strlen(argv[i])-1;j++){
        if(argv[i][0] == '-'){
          char current = argv[i][j+1];
          switch(current){
            case 'c':
              byteFlag = 1;
              if(charFlag == 1){
                charFlag = 0;
              }
              break;
            case 'l':
              lineFlag = 1;
              break;
            case 'm':
              charFlag = 1;
              if(byteFlag == 1){
                byteFlag = 0;
              }
              break;
            case 'w':
              wordFlag = 1;
              break;
            default:
              printf("invalid option -- '%s'\n",argv[i]);
              return 1;
              break;
          }
        }
        else{
          startingFile = i;
          break;
        }
      }
    }
    int arg;
    for(arg = startingFile; arg < argc; arg++){
      byteCount = 0;
      lineCount = 0;
      charCount = 0;
      wordCount = 0;
      file = fopen(argv[arg], "r");
      if (file){
        const size_t line_size = 400;
        char* readLine = malloc(line_size);
        while (fgets(readLine, line_size, file) != NULL){
              for (i = 0; i < strlen(readLine); i++){
                  if(!isspace(readLine[i])){
                      isInsideWord = 1;
                  }
                  else{
                     if (isInsideWord){
                        isInsideWord = 0;
                        wordCount++;
                     }
                  }
                  charCount++;
                  //SUPUESTO SIN BONUS: CHAR = 1BYTE
                  byteCount++;
              }
              if(readLine[i]=='\0' && i != 399){
                lineCount++;
              }
          }
          if(wordCount==0 && isInsideWord==1){
            wordCount = 1;
          }
          free(readLine);
          totalByteCount += byteCount;
          totalLineCount += lineCount;
          totalCharCount += charCount;
          totalWordCount += wordCount;

          wc_results(lineCount,wordCount,charCount,byteCount,lineFlag,wordFlag,charFlag,byteFlag,argv[arg]);

          fclose(file);
      }
      else
      {
          fprintf(stderr,"%s: %s\n",argv[arg],strerror(errno));
          returnValue = 1; //SE ASUME QUE SI UNO DE LOS ARCHIVOS NO EXISTE, SERA UN ERROR
				//PERO SEGUIRA CORRIENDO EL PROGRAMA HASTA FINALIZAR
				//RETORNANDO UN 1 YA QUE FALLO
      }
      if(arg == argc-1 && startingFile != arg){
        wc_results(totalLineCount,totalWordCount,totalCharCount,totalByteCount,lineFlag,wordFlag,charFlag,byteFlag,"total");
      }
    }
    return returnValue;
}




void grep(char* pattern, char* filename) {
    char lineBuffer[BUFFER_LENGTH+1];
    FILE *fp = NULL;
    size_t count = 0;

    fp = fopen(filename, "r");
    if(!fp) {
        fprintf(stderr, "Error: Unable to open file %s.\n", filename);
        return;
    }

    while(fgets(lineBuffer, BUFFER_LENGTH, fp)) {
        if(strstr(lineBuffer, pattern)) {
            count++;
            printf("%s", lineBuffer);
        }
        if(ferror(fp)) {
            fprintf(stderr, "Error: Failed to read file %s.\n", filename);
            break;
        }
    }

    fclose(fp);

    if(count == 0) {
        printf("'%s' not found in file '%s'\n", pattern, filename);
    }
    else {
        printf("'%s' was found %lu time(s) in file '%s'\n", pattern, count, filename);
    }
}




void cmatrix(char* arg) {

  int flag = 0;

  if(arg != NULL) {
    if(strcmp(arg, "-B") == 0) {
      flag = 1;
    } else if(strcmp(arg, "-r") == 0) {
      flag = 2;
    } else if(strcmp(arg, "-d") == 0) {
        flag = 3;
    } else if (arg != "-B" && arg != "-r" && arg != "-d"){
        printf("Unknown argument: %s\n", arg);
        printf("Available arguments: \n-r: Rainbow mode\n-d: Donut\n-B: Bold mode\n");
        exit(1);
    }
  }





    signal(SIGINT, sigint_handler); // register the signal handler for SIGINT


    //get terminal lines and columns
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    int x,y,loop;
    int row=100,col=400; //max values
    int DART[row][col];
    //printf("\e[?25l"); //hide cursor






    while (!stop) {  // Exit loop if SIGINT has been received
        usleep(30000);
    //You can change the speed of the letter cascade changing the value in the command above
        struct winsize w;
        ioctl(0, TIOCGWINSZ, &w);
        if ( row != w.ws_row || col != w.ws_col )
        {for (x=0;x<row;x++)	{for (y=0;y<col;y++)	{DART[x][y]=32;}}//fill matrix with white spaces
            system("clear");
            usleep(50000);
            row=w.ws_row;
            col=w.ws_col;
            int DART[row][col];
            //draw first linew.ws_col
            for (y=0;y<col;y++)
                {	DART[0][y]=rand() % 90 + 97;
                //the higher the 1st value gets, more spaces appear
                if (DART[0][y]>122) {DART[0][y]=32;}
            }
        }
        for (x=row-1;x>0;x--)
        {
                for (y=0;y<w.ws_col;y++)
                {
                if ( DART[x][y] == 32 && DART[x-1][y] != 32 && DART[x-2][y] == 32 )
                        {DART[x-1][y]=32;}//get rid of single letters
                if ( DART[x][y] == 32 && DART[x-1][y] != 32)
                            {DART[x][y]=rand() % 26 + 97;}
                else
                        { if ( DART[x][y] != 32 && DART[x-1][y] == 32)
                            {DART[x][y]=32;}
                            }
                }
        }
    //rewrite first line
    for (y=0;y<w.ws_col;y++)
    {
    if ( DART[0][y] == 32 ) {DART[0][y]=rand() % 550 + 97;}
    else {DART[0][y]=rand() % 30 + 97;}

        if (DART[0][y]>122) {DART[0][y]=32;}
        }
    //print matrix
    system("clear");






    //FLAGS
//////////////////////////////////////////////////////////////////////////////////////////////

    // Default code ig idk
    if (flag == 0){    //checks if bold flag is active
    for (x=0;x+1<w.ws_row;x++) {
        for (y=0;y<w.ws_col;y++) {
            if (DART[x][y] != 32 && DART[x+1][y] ==32 ) {
                printf("\x1B[1m\x1b[33m%c\x1B[0m",DART[x][y]);     // not bold
                }
            else {
                printf("\x1b[32m%c",DART[x][y]);       // not bold
                }
            }
     printf("\n");
        }
        } //end of not bold flag



    else if (flag == 1){ // makes the text BOLD
        for (x=0;x+1<w.ws_row;x++)
        {   for (y=0;y<w.ws_col;y++)
            {   if (DART[x][y] != 32 && DART[x+1][y] ==32 )
                        {printf("\033[1m\033[33m%c\033[0m",DART[x][y]);}    // bold
                    else {printf("\033[1m\033[32m%c\033[0m",DART[x][y]);}   // bold
            }
     printf("\n");
        }
    }






    // rainbow flag

    else if (flag == 2) {




    for (x=0;x+1<w.ws_row;x++) {
    for (y=0;y<w.ws_col;y++) {
        if (DART[x][y] != 32 && DART[x+1][y] ==32 ) {
            switch (rand() % 6) {
                case 0: printf("\033[1;31m%c\033[0m",DART[x][y]); break; // red
                case 1: printf("\033[1;33m%c\033[0m",DART[x][y]); break; // yellow
                case 2: printf("\033[1;32m%c\033[0m",DART[x][y]); break; // green
                case 3: printf("\033[1;34m%c\033[0m",DART[x][y]); break; // blue
                case 4: printf("\033[1;35m%c\033[0m",DART[x][y]); break; // magenta
                case 5: printf("\033[1;36m%c\033[0m",DART[x][y]); break; // cyan
            }
        }
        else {
            printf("\x1b[38;5;%dm%c", rand() % 256, DART[x][y]);
            }
        }
        printf("\n");
    }
}
///////////////////////////////////////////////////////////////////////////////////

 else if (flag == 3){
        donut();
        exit(1);
    }

   } stop = 0; // reset the stop flag
}




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
  } else if (strcmp(option, "-H") == 0) {
    // use the decimal SI suffixes (powers of 1024) for the human-readable output
    printf("Filesystem    Size  Used Avail Use%% Mounted on\n");
    printf("/dev/root    %4.1fGi %4.1fGi %4.1fGi %3.0f%% /",
           (float)vfs.f_frsize * vfs.f_blocks / 1e9,
           (float)vfs.f_frsize * (vfs.f_blocks - vfs.f_bfree) / 1e9,
           (float)vfs.f_frsize * vfs.f_bfree / 1e9,
           100.0 * (vfs.f_blocks - vfs.f_bfree) / vfs.f_blocks);
  } else {
    // use the default format for the non-option case
    printf("Filesystem     1K-blocks    Used Available Use%% Mounted on\n");
    printf("/dev/root    %10lu %6lu %10lu %3.0f%% /",
           vfs.f_blocks * (vfs.f_frsize / 1024),
           (vfs.f_blocks - vfs.f_bfree) * (vfs.f_frsize / 1024),
           vfs.f_bfree * (vfs.f_frsize / 1024),
           100.0 * (vfs.f_blocks - vfs.f_bfree) / vfs.f_blocks);
  }
}




int donut() {

    signal(SIGINT, sigint_handler); // register the signal handler for SIGINT


    float z[1760], a=0, e=1, c=1, d=0, f, g, h, G, H, A, t, D;
    char b[1760];
    int i, j, k, x, y, o, N;

    while (!stop) {
        // clear the screen
        memset(b, 32, 1760);
        // initialize variables
        g = 0, h = 1;
        memset(z, 0, 7040);

        // loop through angles
        for (j = 0; j < 90; j++) {
            G = 0, H = 1;
            // loop through angles
            for (i = 0; i < 314; i++) {
                // calculate x and y coordinates
                A = h + 2;
                D = 1 / (G * A * a + g * e + 5);
                t = G * A * e - g * a;
                x = 40 + 30 * D * (H * A * d - t * c);
                y = 12 + 15 * D * (H * A * c + t * d);
                o = x + 80 * y;
                N = 8 * ((g * a - G * h * e) * d - G * h * a - g * e - H * h * c);
                // set character at (x,y) if it's within bounds and has higher depth
                if (22 > y && y > 0 && 80 > x && x > 0 && D > z[o]) {
                    z[o] = D;
                    b[o] = (N > 0 ? N : 0)[".,-~:;=!*#$@"];
                }
                R(.02, H, G);
            }
            R(.07, h, g);
        }

        // print the donut in green color
        for (k = 0; k < 1761; k++) {
            putchar('\033'); // start ANSI escape sequence
            printf("[32m"); // set text color to green
            putchar(k % 80 ? b[k] : '\n');
            putchar('\033'); // start ANSI escape sequence
            printf("[0m"); // reset text color
        }

        // rotate donut
        R(.04, e, a);
        R(.02, d, c);

        // wait before refreshing the screen
        usleep(15000);
        system("clear");
    }stop = 0; // reset the stop flag
}









