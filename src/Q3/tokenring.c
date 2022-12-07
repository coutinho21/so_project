#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>

int main (int argc, char *argv[]) {
    setbuf(stdout, NULL);
    /* check for failure */
    if(argc != 4) {
        perror("Wrong number of arguments");
        return EXIT_FAILURE;
    }
    
    if(atoi(argv[1]) < 2) {
        perror("Wrong number of processes");
        return EXIT_FAILURE;
    }

    if(atof(argv[2]) > 1 && atof(argv[2]) < 0) {
        perror("Wrong probability");
        return EXIT_FAILURE;
    }

    if(atoi(argv[3]) < 0) {
        perror("Wrong number of seconds");
        return EXIT_FAILURE;
    }

    char *pn = (char*)malloc(50 * sizeof(char));
    for(int i = 1; i <= atoi(argv[1]); i++) {
        if(i == atoi(argv[1]))
            sprintf(pn, "pipe%dto1", i);
        else
            sprintf(pn, "pipe%dto%d", i, i+1);

        if((mkfifo(pn, 0666)) < 0) {
            perror("mkfifo():");
            exit(EXIT_FAILURE);
        }
    }
    free(pn);

    int tok = 0;

    pid_t pid[atoi(argv[1])];
    char *pipeWrite = (char*)malloc(50 * sizeof(char));
    char *pipeRead = (char*)malloc(50 * sizeof(char));
    for(int i = 1; i <= atoi(argv[1]); i++) {
        if((pid[i-1] = fork()) < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        } 
        else if(pid[i-1] == 0) {

            if(i == 1) {
                sprintf(pipeWrite, "pipe%dto%d", i, i+1);
                sprintf(pipeRead, "pipe%dto1", atoi(argv[1]));
            } 
            else if(i == atoi(argv[1])) {
                sprintf(pipeWrite, "pipe%dto1", i);
                sprintf(pipeRead, "pipe%dto%d", i-1, i);
            } 
            else {
                sprintf(pipeWrite, "pipe%dto%d", i, i+1);
                sprintf(pipeRead, "pipe%dto%d", i-1, i);
            }

            srandom(getpid());

            int fd[2];

            if(i == 1) {

                if((fd[1] = open(pipeWrite, O_WRONLY)) < 0) {
                    perror("open():");
                    exit(EXIT_FAILURE);
                }

                if(tok == INT_MAX)
                    exit(EXIT_SUCCESS);
                else
                    tok++; 

                if(write(fd[1], &tok, sizeof(int)) < 0) {
                    perror("write():");
                    exit(EXIT_FAILURE);
                }

                close(fd[1]);
            }

            while(1) {

                if((fd[0] = open(pipeRead, O_RDONLY)) < 0) {
                    perror("open():");
                    exit(EXIT_FAILURE);
                }

                if(read(fd[0], &tok, sizeof(int)) < 0) {
                    perror("read():");
                    exit(EXIT_FAILURE);
                }

                close(fd[0]);
                tok++; 
                
                int rand = random() % 100;
                if(rand < 100 * atof(argv[2])) {
                    printf("[p%d] lock on token (val = %d)\n", i, tok);
                    sleep(atoi(argv[3]));
                    printf("[p%d] unlock token\n", i);
                }

                if((fd[1] = open(pipeWrite, O_WRONLY)) < 0) {
                    perror("open():");
                    exit(EXIT_FAILURE);
                }
                
                if(write(fd[1], &tok, sizeof(int)) < 0) {
                    perror("write():");
                    exit(EXIT_FAILURE);
                }

                close(fd[1]);
            }

            exit(EXIT_SUCCESS);
        }
    }

    /* wait for child processes to end */
    for(int i = 0; i < atoi(argv[1]); i++) {
        if(waitpid(pid[i], NULL, 0) < 0) {
            perror("waitpid():");
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}