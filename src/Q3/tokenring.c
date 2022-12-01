#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


int main(int argc, char *argv[]){
    int n, t, val = 0, fd, mp, op;
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
        
        mp = mkfifo(string,0666);
        
        if(mp == -1){
            printf("error creating pipe\n");
            return EXIT_FAILURE;
        }
    }

    while(1){
        fd = fork();
        if(fd < 0){
            printf("error creating process\n");
            return EXIT_FAILURE;
        }

        //child process
        if(fd == 0){
            op = open(string, O_WRONLY);
            if(op == -1){
                printf("error opening pipe\n");
                return EXIT_FAILURE;
            }
            write(op, &val, sizeof(int));
            close(op);
            printf("child %d wrote %d\n", getpid(), val);
            
            sleep(t);
            exit(0);
        }
        else{
            //parent process
            for(int i = 1; i <= n; i++){
                string[4] = '0' + i;
                string[7] = '0' + i + 1;

                if(i == n){
                    string[4] = '0' + n;
                    string[7] = '0' + 1;
                }
                op = open(string, O_RDONLY);
                if(op == -1){
                    printf("error opening pipe\n");
                    return EXIT_FAILURE;
                }
                read(op, &val, sizeof(int));
                close(op);
                printf("process %d: %d\n", i, val);
                val++;
            }
        }
    }


    op = open(string, O_WRONLY);
    if(op == -1){
        printf("error occurred opening pipe\n");
        return EXIT_FAILURE;
    }

    //write
    if(write(op, &val, sizeof(int)) == -1){
        printf("error writing in the pipe\n");
        return EXIT_FAILURE;
    } else printf("sender sent the data.\n");
    

    op = open(string, O_RDONLY);
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

    return 0;
}