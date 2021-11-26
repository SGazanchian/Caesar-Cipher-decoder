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
    fptr = fopen("placer.txt","w");
    if(fptr == NULL)
    {
        printf("Error!");
        exit(1);
    }
    fprintf(fptr,"%s",text);
    fclose(fptr);
}
char *str_replace(char *orig, char *rep, char *with) {
    char *result;
    char *ins;
    char *tmp;
    int len_rep;
    int len_with;
    int len_front;
    int count;

    if (!orig || !rep)
        return NULL;
    len_rep = strlen(rep);
    if (len_rep == 0)
        return NULL;
    if (!with)
        with = "";
    len_with = strlen(with);


    ins = orig;
    for (count = 0; tmp = strstr(ins, rep); ++count) {
        ins = tmp + len_rep;
    }

    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;
    ins = strstr(orig, rep);
    len_front = ins - orig;
    tmp = strncpy(tmp, orig, len_front) + len_front;
    tmp = strcpy(tmp, with) + len_with;
    orig += len_front + len_rep;
    strcpy(tmp, orig);
    return result;
}
int main(){

    FILE *fptr;
    fptr = fopen("placer.txt","w");
    fclose(fptr);

    int fd;
    char text[BUFF];

    int wordCount , sentenceCount;
    mkfifo(FIFO_PLACER_PATH, 0666);
    fd = open(FIFO_PLACER_PATH, O_RDONLY);
    read(fd, &wordCount, sizeof(int));
    char words[wordCount][BUFF];
    for (int i = 0; i < wordCount; ++i) {
        read(fd,words[i], sizeof(words[i]));

    }
    mkfifo(FIFO_PLACER_PATH_MP, 0666);
    fd = open(FIFO_PLACER_PATH_MP, O_RDONLY);

    char sentence[BUFF];

    read(fd,sentence, sizeof(sentence));
    char * str;
    int j = 0;
    while ((str = strstr(sentence,"$"))){
        memcpy(sentence,str_replace(sentence,"$",words[j++]) , sizeof(sentence));
    }
    writeToFile(sentence);
    close(fd);
    mkfifo(FIFO_PLACER_PATH_MP, 0666);
    fd = open(FIFO_PLACER_PATH_MP, O_WRONLY);

    write(fd,sentence,sizeof(sentence));

    return 0;
}