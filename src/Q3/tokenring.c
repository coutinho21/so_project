#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


int main(int argc, char *argv[]){
    int n, t, val = 0, fd[2], mp, op;
    float p;
    char string[] = "pipeitoj";
    n = atoi(argv[1]);
    p = atof(argv[2]);
    t = atoi(argv[3]);

    //creating the pipes
    for(int i = 1; i <= n; i++){
        string[4] = '0' + i;
        string[7] = '0' + i + 1;

        if(i == n){
            string[4] = '0' + n;
            string[7] = '0' + 1;
        }

        int flag = 0;

        mp = mkfifo(string,0666);
        if(mp == -1){
            printf("error creating pipe\n");
            flag = 1;
        }

        if(!flag) op = open(string, O_WRONLY);
        if(op == -1){
            printf("error occurred opening pipe\n");
            return EXIT_FAILURE;
        }

        //write
        if(write(op, &val, sizeof(int)) == -1){
            printf("error writing in the pipe\n");
            return EXIT_FAILURE;
        } else printf("sender sent the data.\n");
        

        if(!flag) op = open(string, O_RDONLY);
        if(op == -1){
            printf("error occurred opening pipe\n");
            return EXIT_FAILURE;
        }

        //read
        if(read(op, &val, sizeof(int)) == -1){
            printf("error reading from pipe\n");
            return EXIT_FAILURE;
        }else{
            printf("Data received from sender: %d\n", val);
            val++;
        }
    }

    return 0;
}