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
    fptr = fopen("finder.txt","w");
    fclose(fptr);


    int fd;
    char text[BUFF];
    mkfifo(FIFO_FINDER_PATH, 0666);
    fd = open(FIFO_FINDER_PATH, O_RDONLY);
    read(fd, text, sizeof(text));
    struct pos p[BUFF];
    int count;

    close(fd);


    mkfifo(FIFO_FINDER_PATH_MP, 0666);
    fd = open(FIFO_FINDER_PATH_MP, O_RDONLY);
    read(fd,&count, sizeof(int));

    read(fd, p , sizeof(p));
    char words[count][BUFF];
    for (int i = 0; i < count; ++i) {
        memcpy( words[i], &text[p[i].start], p[i].end);
        words[i][p[i].start + p[i].end] = '\0';
        writeToFile(words[i]);
    }


    mkfifo(FIFO_PLACER_PATH, 0666);
    fd = open(FIFO_PLACER_PATH, O_WRONLY);

    write(fd,&count, sizeof(int));

    for (int i = 0; i < count; ++i) {
        write(fd , words[i],sizeof(words[i]));
    }


    return 0;


}