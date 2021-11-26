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


char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }
    count += last_comma < (a_str + strlen(a_str) - 1);
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {

            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        *(result + idx) = 0;
    }

    return result;
}

int main(){





    char text[BUFF];

    printf("Enter encoded text : ");
    scanf("%s", text);



    int status,fd;


    mkfifo(FIFO_DECODER_PATH,0777);
    fd = open(FIFO_DECODER_PATH,O_RDWR);
    close(fd);
    mkfifo(FIFO_FINDER_PATH_MP,0777);
    fd = open(FIFO_DECODER_PATH,O_RDWR);
    close(fd);
    mkfifo(FIFO_PLACER_PATH,0777);
    fd = open(FIFO_DECODER_PATH,O_RDWR);
    close(fd);


    pid_t pid = fork();
    pid_t pid2 , pid3;

    if (pid < 0)
        return -1;
    else if (pid > 0) {
        pid2 = fork();
        if (pid2 < 0)
            return -1;
        else if (pid2 > 0) {
            pid3 = fork();
            if (pid3 < 0)
                return -1;
        }
    }

    if(pid == 0) {
        execvp(EXEC_DECODER,NULL);
    } else if (pid2 == 0){

        execvp(EXEC_FINDER,NULL);

    }else if (pid3 == 0){
        execvp(EXEC_PLACER,NULL);

    } else{ //Parent



        printf("Decoder ...\n");
        mkfifo(FIFO_DECODER_PATH, 0777);
        fd = open(FIFO_DECODER_PATH, O_WRONLY);
        write(fd, text, strlen(text) + 1);
        close(fd);
        waitpid(pid,&status,0);
        printf("Finder ...\n");
        printf("Enter positions : ");
        scanf(" %[^\n]%*c",text);
        struct pos p[BUFF];


        char ** token = str_split(text,'$');
        int count = 0;
        if(token){
            for (int i = 0; *(token + i); ++i) {
                char** numbers = str_split(*(token + i),' ');
                struct pos temp;
                if(numbers){
                    for (int j = 0; j < 2; ++j) {
                        if(!j)
                            temp.start = (int) strtol(*(numbers + j), (char **)NULL, 10);
                        else
                            temp.end = (int) strtol(*(numbers + j), (char **)NULL, 10);
                    }
                    p[i] = temp;
                }

                count = i + 1;
            }

        }


        mkfifo(FIFO_FINDER_PATH_MP, 0777);
        fd = open(FIFO_FINDER_PATH_MP, O_RDWR);

        printf("Counter main is : %d\n",count);
        write(fd,&count, sizeof(int));
        write(fd, p, sizeof (p));



        waitpid(pid2,&status,0);

        char sentence[BUFF];

        printf("Placer ...\n");
        printf("enter your text : ( press ';' to end input)\n");

        scanf("%[^;]s", sentence);
        mkfifo(FIFO_PLACER_PATH_MP,0777);
        fd = open(FIFO_PLACER_PATH_MP, O_RDWR);
        write(fd,sentence,sizeof (sentence));

        close(fd);


        mkfifo(FIFO_PLACER_PATH_MP,0777);

        fd = open(FIFO_PLACER_PATH_MP, O_RDONLY);


        read(fd,sentence, sizeof(sentence));


        printf("Final Result %s",sentence);
    }

}