#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> //isspace()


void wc_results(int lines, int words, int chars, int bytes,int lineFlag,int wordFlag,int charFlag,int byteFlag, char* filename) {
    
    if (lineFlag==1) {printf("Number of lines in the file is: %d\t",lines);};
    if (wordFlag==1) {printf("Number of words in the file is: %d\t",words);};
    if (charFlag==1) {printf("Number of characters in the file is: %d\t",chars);};
    if (byteFlag==1) {printf("Number of lines in the file is: c%d\t ",bytes);};

    if(lineFlag == 0 && wordFlag == 0 && charFlag == 0 && byteFlag == 0){
      printf("%d\t%d\t%d\t",lines,words,chars);
    }

    printf("%s\n",filename);
  }

int my_wc(int argc, char** argv) {

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


    for (i = 1; i < argc; i++) {
        int j;
        
        if (startingFile != 0) {
          break;
        }

        for (j = 0; j<strlen(argv[i])-1;j++) {
            if (argv[i][0] == '-') {
                char current = argv[i][j+1];

                switch (current) {

                    case 'c':
                        byteFlag = 1;
                        if (charFlag == 1) {
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
                        printf("Unknown argument\n");
                        printf("Available arguments: \n-l: Display the number of lines\n-w: Display the number of words\n-c: Display the number of characters\n-m: Display the number of bytes\n"); 
                        return 1;
                        break;
                }
            }

            else {
                startingFile = i;
                break;
            }
        }
    }


    int arg;
    for(arg = startingFile; arg < argc; arg++) {

        byteCount = 0;
        lineCount = 0;
        charCount = 0;
        wordCount = 0;

        file = fopen(argv[arg], "r");
        if (file) {
            const size_t line_size = 400;
            char* readLine = malloc(line_size);

            while (fgets(readLine, line_size, file) != NULL) {

                for (i = 0; i < strlen(readLine); i++) {
                    if (!isspace(readLine[i])) {
                        isInsideWord = 1;
                    }
                    else {
                        if (isInsideWord){
                            isInsideWord = 0;
                            wordCount++;
                        }
                    }
                    charCount++;
                    byteCount++;
                }

                if (readLine[i]=='\0' && i != 399) {
                    lineCount++;
                }
            }

            if (wordCount==0 && isInsideWord==1) {
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

        else {
            fprintf(stderr,"%s: %s\n",argv[arg],strerror(errno));
            returnValue = 1;
        }

        if (arg == argc-1 && startingFile != arg) {
          wc_results(totalLineCount,totalWordCount,totalCharCount,totalByteCount,lineFlag,wordFlag,charFlag,byteFlag,"total");
        }
    }
    return returnValue;
}
