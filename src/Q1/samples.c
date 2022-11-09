#include <stdio.h>
#include <stdlib.h>
#include <libc.h>

int main(int argc, char *argv[]) {
  FILE *fp = fopen(argv[1], "r");
  int n = argv[2], m = argv[3];
  int charCount = 0;
  setbuf(stdout, NULL); //buffer

  if (fp == NULL) printf("could not open file");

  //count the number of chars in the file
  for (char c = getc(fp); c != EOF; c = getc(fp)) charCount++;
  
}
