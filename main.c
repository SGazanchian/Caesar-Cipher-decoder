#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/wait.h>
#include "Constraint.c"

int main(){


    pid_t pid = fork();
    int status;
    if(pid == 0){

     execvp("./OS_Finder" , NULL);
    } else if(pid == -1){

        printf("Error.\n");
    } else{

        wait(&status);
        printf("Child finished");
    }


}