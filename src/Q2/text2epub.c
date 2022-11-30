#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    int size = argc;
    if(size <2){
        printf("Number of arguments is smaller than expected.\n");
        return EXIT_FAILURE;
    }
    
    char* listOfZips[argc+2];
    listOfZips[0] = (char *)malloc((strlen("zip")+1)*sizeof(char));
    strcpy(listOfZips[0], "zip");
    listOfZips[1] = (char *)malloc((strlen("ebooks.zip") + 1)*sizeof(char));
    strcpy(listOfZips[1], "ebooks.zip");
    listOfZips[argc+1] = NULL;

    for(int i = 1; i < argc ; i++){
        pid_t pid = fork();
        if(pid == 0){
            char* oldName = (char*)malloc((strlen(argv[i])+2));
            strcpy(oldName, argv[i]);
            char* newName = (char*)malloc((strlen(argv[i])+2));
            oldName[strlen(oldName)-4] = '\0';
            strcpy(newName, oldName);
            strcat(newName, ".epub");

            if(execlp("pandoc", "pandoc", argv[i], "-o", newName, NULL) == -1){
                perror("execlp():");
                return EXIT_FAILURE; //ver
            }
            free(oldName);
            free(newName);
            return EXIT_FAILURE;
        }
        else if(pid == -1){
            perror("fork():");
            return EXIT_FAILURE;
        }
    }
    
    for(int i = 1; i < argc ; i++){
        wait(NULL);
    }

    if(execvp("zip", listOfZips) == -1){
        perror("execvp():");
        return EXIT_FAILURE;
    }
    
    for(int i = 0; i < argc+2 ; i++){
        print("%s\n", listOfZips[i]);
        free(listOfZips[i]);
    }
}