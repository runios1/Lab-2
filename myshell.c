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
    int pid=-2;
    int fdIn;   
    int fdOut; 
    if(pCmdLine->argCount==0)
        fprintf(stderr,"Got no arguments.\n");
    
    else{
        if(pCmdLine->inputRedirect){
            fdIn=open(pCmdLine->inputRedirect,O_RDONLY,0777);
            dup2(fdIn,STDIN_FILENO);
        }
        if(pCmdLine->outputRedirect){
            fdOut=open(pCmdLine->outputRedirect,O_WRONLY,0777);
            dup2(fdOut,STDOUT_FILENO);
        }
        if(strcmp(pCmdLine->arguments[0],"cd")==0){ //CHECK THIS
            if(pid==0){
                if(pCmdLine->argCount<2)
                    fprintf(stderr,"cd did not get a directody to change to.\n");
            
                else if(chdir(pCmdLine->arguments[1])!=0)
                    fprintf(stderr,"chdir failed.\n");
            }
        }else{

                if(strcmp(pCmdLine->arguments[0],"suspend")==0){
                    if(pCmdLine->argCount<2)
                        fprintf(stderr,"suspend did not get a process to suspend.\n");
                    kill(atoi(pCmdLine->arguments[1]),SIGTSTP);
                }else if(strcmp(pCmdLine->arguments[0],"wake")==0){
                    if(pCmdLine->argCount<2)
                        fprintf(stderr,"wake did not get a process to wake.\n");
                    kill(atoi(pCmdLine->arguments[1]),SIGCONT);
                }else if(strcmp(pCmdLine->arguments[0],"kill")==0){
                    if(pCmdLine->argCount<2)
                        fprintf(stderr,"kill did not get a process to kill.\n");
                    kill(atoi(pCmdLine->arguments[1]),SIGKILL);
                }else{
                    pid=fork();
                    if(pCmdLine->blocking==1){
                        waitpid(pid,NULL,0);
                    }
                    if(pid==0&&execvp(pCmdLine->arguments[0],pCmdLine->arguments)==-1){
                        perror("Error accured in execute: ");
                        _exit(4);
                    }
                } 
            
        } 

        if(pCmdLine->inputRedirect){
            close(fdIn);
        }
        if(pCmdLine->outputRedirect){
            close(fdOut);
        }
    }
}

int main(int argc, char **argv){
    char curPath[PATH_MAX];
    char input[2048];
    cmdLine* parsedInput=NULL;
    int debugFlag=0; //1 for debug mode, 0 otherwise.

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
            exit(0);
        }
        


        for(int i=0;input[i]!='\0';i++){
            if(input[i]=='-'&&input[i+1]=='d')
                debugFlag=1;
        }

        parsedInput=parseCmdLines(input);

        if(parsedInput==NULL){
            fprintf(stderr,"Failed to parse input.\n");
            return 3;
        }

        

        if(debugFlag==1){
            fprintf(stderr,"PID:%d\nExecuting command:%s\n",getpid(),input);
        }

        
        execute(parsedInput);
        
        freeCmdLines(parsedInput);
    }

    return 0;
}