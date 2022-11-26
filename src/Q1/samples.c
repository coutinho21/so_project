#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[]) {
    time_t t;
    int fragmentNumber, sizeOfFragment;
    FILE *fp = fopen(argv[1], "r");

    fragmentNumber = atoi(argv[2]);
    sizeOfFragment = atoi(argv[3]);
    srand((unsigned) time(&t));

    if (fp == NULL){
        perror("fopen():");
        return EXIT_FAILURE;
    }
    if(argc < 3){
        printf("Number of arguments is smaller than expected.\n");
        return EXIT_FAILURE;
    }

    if(fseek(fp, 0, SEEK_END) == -1){
        perror("fseek():");
        return EXIT_FAILURE;
    }

    int fileSize = ftell(fp);

    if(fileSize == -1){
        perror("ftell():");
        return EXIT_FAILURE;
    }   

    for(int i = 0; i < fragmentNumber ; i++){
        int random = rand() % fileSize-sizeOfFragment;
        char *buf = (char *) malloc((sizeOfFragment+1)*(sizeof(char)));
        fseek(fp, random, SEEK_SET);
        fread(buf, 1, sizeOfFragment, fp);
        buf[sizeOfFragment] = '\0';
        printf(">%s<\n", buf);
        free(buf);
    }

    if(fclose(fp) == -1){
        perror("Error closing file.");
        return EXIT_FAILURE;
    }

    return 0;
}
