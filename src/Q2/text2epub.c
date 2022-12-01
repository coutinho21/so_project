#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    if (argc < 2){
        printf("Number of arguments is smaller than expected.\n");
        return EXIT_FAILURE;
    }

    char *listOfZips[argc + 2];
    listOfZips[0] = (char *)malloc((strlen("zip") + 1) * sizeof(char));
    strcpy(listOfZips[0], "zip");
    listOfZips[1] = (char *)malloc((strlen("ebooks.zip") + 1) * sizeof(char));
    strcpy(listOfZips[1], "ebooks.zip");
    listOfZips[argc + 1] = NULL;

    for (int i = 1; i < argc; i++){
        char aux[strlen(argv[i]) + 2];
        strcpy(aux, argv[i]);
        char *oldName = (char *)malloc((strlen(aux)) * sizeof(char));
        strcpy(oldName, argv[i]);
        char *newName = (char *)malloc((strlen(aux)) * sizeof(char));
        oldName[strlen(oldName) - 4] = '\0';
        strcpy(newName, oldName);
        strcat(newName, ".epub");
        
        listOfZips[i + 1] = (char *)malloc((strlen(newName) + 1) * sizeof(char));
        strcpy(listOfZips[i + 1], newName);

        pid_t pid = fork();

        if (pid == 0){
            printf("%s <- list\n", listOfZips[i + 1]);

            if (execlp("pandoc", "pandoc", argv[i], "--quiet", "-o", newName, NULL) == -1) perror("execlp():");
            
            free(oldName);
            free(newName);
            return EXIT_FAILURE;
        }
        else if (pid == -1){
            perror("fork():");
            return EXIT_FAILURE;
        }
    }

    for (int i = 1; i < argc; i++){
        if (wait(NULL) == -1){
            perror("wait():");
            return EXIT_FAILURE;
        }
    }

    if (execvp("zip", listOfZips) == -1){
        perror("execvp():");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < argc + 2; i++){
        free(listOfZips[i]);
    }
}
