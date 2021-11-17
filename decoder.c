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

    printf("Writing to file ...\n");
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


    printf("**********************************\n");
    printf("Decoding process starts ...\n");
    mkfifo(FIFO_DECODER_PATH, 0666);
    int status,fd;
    char text[BUFF] , decryptedText[BUFF];


    char ch;
    //Read Encrypted Text From Parent process
    fd = open(FIFO_DECODER_PATH, O_RDONLY);
    read(fd, text, sizeof(text));
    printf("Received %s\n", text);


    for(int i = 0; text[i] != '\0'; ++i){
        ch = text[i];
        if(ch >= 'a' && ch <= 'z'){
            //replaces with three earlier letter
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
    printf("Decoded text is : %s\n" , text);

    writeToFile(text);
    fd = open(FIFO_DECODER_PATH, O_WRONLY);
    write(fd , text , sizeof(text) + 1);
    close(fd);
    printf("**********************************\n");
}