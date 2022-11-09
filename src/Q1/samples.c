#include <stdio.h>
#include <stdlib.h>
#include <libc.h>

int main(int argc, char *argv[]) {
  FILE *fp = fopen(argv[1], "r");
  int n = argv[2], m = argv[3];
  
  if (fp == NULL) printf("could not open file");

  
}
