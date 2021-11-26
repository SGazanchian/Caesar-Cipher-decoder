#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "Constraint.c"
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>




void writeToFile(char text[]){


    FILE *fptr;
    fptr = fopen("decoder.txt","w");
    if(fptr == NULL)
    {
        printf("Error!");
        exit(1);
    }
    fprintf(fptr,"%s",text);
    fclose(fptr);
}
int main(){




    mkfifo(FIFO_DECODER_PATH, 0777);
    int fd;
    char text[BUFF];
    char ch;
    //Read Encrypted Text From Parent process
    fd = open(FIFO_DECODER_PATH, O_RDONLY);
    read(fd, text, sizeof(text));



    for(int i = 0; text[i] != '\0'; ++i){
        ch = text[i];
        if(ch >= 'a' && ch <= 'z'){
            ch = ch - 3;
            if(ch < 'a'){
                ch = ch + 'z' - 'a' + 1;
            }
            text[i] = ch;
        }
        else if(ch >= 'A' && ch <= 'Z'){
            ch = ch - 3;
            if(ch < 'A'){
                ch = ch + 'Z' - 'A' + 1;
            }
            text[i] = ch;
        }
    }

    writeToFile(text);
    fd = open(FIFO_FINDER_PATH, O_WRONLY);
    write(fd, text, sizeof(text));
    close(fd);


    return 0;
}