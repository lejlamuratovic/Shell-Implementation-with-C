#include <stdio.h>
#include <stdlib.h>
#include <errno.h> //Provides error codes for various system errors
#include <dirent.h> // Provides functions to access directories and their contents
#include <sys/ioctl.h> //Provides functions to control I/O operations on devices
#include <signal.h> // Provides functions to handle signals like SIGINT

#define R(t,x,y) f=x;x-=t*y;y+=t*f;f=(3-x*x-y*y)/2;x*=f;y*=f; //donut()

void donut();

// used for cmatrix (ctrl + C)
// declare a global flag variable to indicate if SIGINT has been received
int stop = 0;
// define a signal handler for SIGINT
void sigint_handler(int sig) {
    printf("\n");
    stop = 1;
}


void my_cmatrix(char* arg) {

    int flag = 0;

    if(arg != NULL) {
        if(strcmp(arg, "-B") == 0) {
        flag = 1;
        } 
        else if(strcmp(arg, "-r") == 0) {
        flag = 2;
        }
        else if(strcmp(arg, "-d") == 0) {
            flag = 3;
        } 
        else if(strcmp(arg, "-s") == 0) {
            flag = 4;
        } 
        else if (strcmp(arg, "-r") != 0 && strcmp(arg, "-d") != 0 && strcmp(arg, "-B") != 0 && strcmp(arg, "-s") != 0){
            printf("Unknown argument");
            printf("Available arguments: \n-r: Rainbow mode\n-d: Donut\n-B: Bold mode\n-s: Space mode\n");
            exit(1);
        }
    }


    signal(SIGINT, sigint_handler); // register the signal handler for SIGINT


    //get terminal lines and columns
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    int x,y;
    int row=100,col=400; //max values
    int DART[row][col];
    //printf("\e[?25l"); //hide cursor



    while (!stop) {  // Exit loop if SIGINT has been received
        
        usleep(30000);
        //You can change the speed of the letter cascade changing the value in the command above
        struct winsize w;
        ioctl(0, TIOCGWINSZ, &w);
        if ( row != w.ws_row || col != w.ws_col ) {
            for (x=0;x<row;x++)	{
                for (y=0;y<col;y++)	{
                    DART[x][y]=32;
                }
            }
            
            //fill matrix with white spaces
            system("clear");
            usleep(50000);
            row=w.ws_row;
            col=w.ws_col;
            int DART[row][col];

            //draw first linew.ws_col
            for (y=0;y<col;y++) {	
                    DART[0][y]=rand() % 90 + 97;
                    //the higher the 1st value gets, more spaces appear
                    if (DART[0][y]>122) {DART[0][y]=32;}
            }
        }


        for (x=row-1;x>0;x--) {
            for (y=0;y<w.ws_col;y++) {
                if ( DART[x][y] == 32 && DART[x-1][y] != 32 && DART[x-2][y] == 32 ) {
                    DART[x-1][y]=32;
                }//get rid of single letters

                if ( DART[x][y] == 32 && DART[x-1][y] != 32) {
                    DART[x][y]=rand() % 26 + 97;
                }

                else { 
                    if ( DART[x][y] != 32 && DART[x-1][y] == 32) {
                        DART[x][y]=32;
                    }
                }
            }
        }


    //rewrite first line
    for (y=0;y<w.ws_col;y++) {
        
        if ( DART[0][y] == 32 ) {
            DART[0][y]=rand() % 550 + 97;
        }
        
        else {DART[0][y]=rand() % 30 + 97;
        }

        if (DART[0][y]>122) {
            DART[0][y]=32;
        }
    }

    system("clear");



    //print matrix

// default code (no flags applied) 
    if (flag == 0){ 
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
    } 



// bold flag (-B)
    else if (flag == 1){ // makes the text BOLD
        for (x=0;x+1<w.ws_row;x++) {   
            for (y=0;y<w.ws_col;y++) {   
                if (DART[x][y] != 32 && DART[x+1][y] ==32 ) {
                    printf("\033[1m\033[33m%c\033[0m",DART[x][y]);
                    }
                else {
                    printf("\033[1m\033[32m%c\033[0m",DART[x][y]);
                }
            }
        printf("\n");
        }
    }



    // rainbow flag (-r)
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


// donut flag (-d)
    else if (flag == 3){
        donut();
        exit(1);
        }




    // donut flag (-s)
    else if (flag == 4){
        rocket();
        exit(1);
        }
    } 
    stop = 0; // reset the stop flag
}





// function that displays a donut

void donut() {

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
            //printf("[32m"); // set text color to green
            printf("\033[37m"); // set text color to white
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
    }
    stop = 0; // reset the stop flag
}






//Giving some delay
void delay( unsigned int value) {
    unsigned int count1 =0;
    unsigned int count2 = 0;

    for(count1 = 0; count1 < value ; count1++ )
    {
        for(count2 = 0; count2 < count1 ; count2++ )
        {

        }
    }
}


// string to display rocket_v
const char rocket_v[] =
    "\033[32m            ^ \n\
          /^\\\n\
          |-|\n\
          | |\n\
          |I|\n\
          |S|\n\
          |R|\n\
          |O|\n\
         /| |\\\n\
        / | | \\\n\
       |  | |  |\n\
        `-\"\"\"-`\n\
";

int rocket() {

    int jumpControlAtBottom = 0;
    const int someDelay = 6000;
    int shifControl = 0;


    //jump to bottom of console

    for (jumpControlAtBottom = 0; jumpControlAtBottom < 30; ++jumpControlAtBottom)
    {
        printf("\n");
    }

    //Print rocket_v
    fputs(rocket_v,stdout);

    for (shifControl = 0; shifControl < 30; ++shifControl)
    {
        // rocket_v move on the basis of delay
        delay(someDelay);
        
        // move rocket_v a line upward
        printf("\n");
    }
    return 0;
}