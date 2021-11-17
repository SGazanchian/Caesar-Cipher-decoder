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

int main(){



    printf("//////////////////////////////////////////////////\n");
    printf("Decoding ...\n");

    //Decoding
    char text[BUFF];

    printf("Enter encoded text : ");
    scanf("%s", text);


    mkfifo(FIFO_DECODER_PATH, 0666);
    int status,fd;
    pid_t pid = fork();

    if(pid == -1){
        printf("Error -> closing\n");
        return -1;
    }
    if(pid == 0) { // Child
        execvp(EXEC_DECODER,NULL);
    } else if(pid > 0){ //Parent
        //send encrypted text through NamedPipe
        fd = open(FIFO_DECODER_PATH, O_WRONLY);
        write(fd, text, strlen(text) + 1);
        close(fd);

        //Read Decrypted Text From child process
        fd = open(FIFO_DECODER_PATH, O_RDONLY);
        read(fd, text, sizeof(text));
        printf("Decrypted Text is : %s\n" , text);
    }
    printf("//////////////////////////////////////////////////\n");

}