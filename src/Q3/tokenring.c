#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


int main(int argc, char *argv[]){
    printf("1 passei aqui");
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
        printf("passei aqui 2");
        if(i == n){
            string[4] = '0' + n;
            string[7] = '0' + 1;
        }
        printf("passei aqui");
        mp = mkfifo(string,0666);
        if(mp > 0){
            printf("error creating pipe\n");
            return EXIT_FAILURE;
        }
        printf("passei aqui tambem");
        op = open(string, O_WRONLY);
        if(op == -1){
            printf("error occurred opening pipe\n");
            return EXIT_FAILURE;
        }
        printf("tambem passei aqui");
        int id = fork();
        if(id == -1){
            printf("error occurred creating child process\n");
            return EXIT_FAILURE;
        }
        else if (id == 0){
            close(fd[0]);
            if(write(fd[1], &val, sizeof(int)) == -1){
                printf("error writing in the pipe\n");
                return EXIT_FAILURE;
            }
            close(fd[1]);
        }
        else if (id == 1){
            close(fd[1]);
            if(read(fd[0], &val, sizeof(int)) == -1){
                printf("error reading from pipe\n");
                return EXIT_FAILURE;
            }
            close(fd[0]);
            val++;
            printf("Got %d from child process.\n", val);
        }
        printf("aqui tambem");
    }

    return 0;
}