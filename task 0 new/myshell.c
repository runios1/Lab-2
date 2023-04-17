#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <linux/limits.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include "LineParser.h"

void execute(cmdLine *pCmdLine){
    if(pCmdLine->argCount==0)
        fprintf(stderr,"Got no arguments.\n");
    
    else{
        if(execvp(pCmdLine->arguments[0],pCmdLine->arguments)==-1){
            perror("Error accured in execute: ");
            _exit(4);
        }
    }
}

int main(int argc, char **argv){
    char curPath[PATH_MAX];
    char input[2048];
    cmdLine* parsedInput=NULL;

    if(getcwd(curPath,PATH_MAX)==NULL){
        fprintf(stderr,"Failed to get current directory.\n");
        return 1;
    }

    while(1){
        printf("#>");
        if(fgets(input,2048,stdin)==NULL){
            fprintf(stderr,"Failed to read input.\n");
            return 2;
        }

        if(strncmp(input,"quit",4)==0){
            return 0;
        }
        

        parsedInput=parseCmdLines(input);

        if(parsedInput==NULL){
            fprintf(stderr,"Failed to parse input.\n");
            return 3;
        }
        
        execute(parsedInput);
        
        freeCmdLines(parsedInput);
    }
}