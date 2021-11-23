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
    fptr = fopen("finder.txt","a");
    if(fptr == NULL)
    {
        printf("Error!");
        exit(1);
    }
    fprintf(fptr,"%s",text);
    fclose(fptr);
}
int main(){

    FILE *fptr;
    fptr = fopen("decoder.txt","w");
    fclose(fptr);
    printf("**********************************\n");
    printf("Finder process starts ...\n");

    int fd;
    char text[BUFF];
    mkfifo(FIFO_FINDER_PATH, 0666);
    fd = open(FIFO_FINDER_PATH, O_RDONLY);
    read(fd, text, sizeof(text));
    printf("Finder : Received %s\n", text);
    struct pos p[BUFF];
    int count;

    close(fd);


    mkfifo(FIFO_FINDER_PATH_MP, 0666);
    fd = open(FIFO_FINDER_PATH_MP, O_RDONLY);
    read(fd,&count, sizeof(int));
    printf("Counter is : %d\n",count);
    read(fd, p , sizeof(p));
    char words[count][BUFF];
    for (int i = 0; i < count; ++i) {

        printf("heyyy : %s %d %d\n",text, p[i].start , p[i].end);
        memcpy( words[i], &text[p[i].start], p[i].end);
        writeToFile(words[i]);
    }
    printf("**********************************\n");

    return 0;


}