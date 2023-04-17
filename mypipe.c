#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv){
    int pid;
    int fd[2];
    if(pipe(fd)==-1){
        fprintf(stderr,"Could not create pipe.\n");
        return 1;
    }
    pid=fork();
    

    if(pid==0){//child
        fprintf(stderr,"CHILD\n");
        if(close(fd[0])==-1){
            fprintf(stderr,"Failed to close read end of pipe of child.\n");
            return 2;
        }
        printf("closed\n");
        if(write(fd[1],"hello",6)<1){
            fprintf(stderr,"Failed to write to pipe.\n");
            return 3;
        }
        printf("wrote\n");
        if(close(fd[1])==-1){
            fprintf(stderr,"Failed to close write end of pipe of child.\n");
            return 2;
        }
        // if(close(fd[0])==-1){
        //     fprintf(stderr,"Failed to close read end of pipe of child.\n");
        //     return 2;
        // }
        // printf("closed\n");
        printf("child sent message\n");
        return 0;
    }else{
        printf("PARENT\n");
        char output[7];
        //int bytesRead=0;

        if(read(fd[0],output,7)<0){
                fprintf(stderr,"Failed to read from pipe.\n");
                return 3;
        }

        // while(bytesRead<5){
        //     printf("Reading\n");
            
        //     bytesRead++;
        // }
        
        printf("read\n");
        printf("Parent got message:%s\n",output);
        if(close(fd[0])==-1){
            fprintf(stderr,"Failed to close read end of pipe of parent.\n");
            return 2;
        }
        if(close(fd[1])==-1){
            fprintf(stderr,"Failed to close write end of pipe of parent.\n");
            return 2;
        }
        return 0;
    }
}