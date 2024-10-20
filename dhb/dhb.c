#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// print binary
void printb(unsigned int n) {
  if (n >> 1) {
    printb(n >> 1);
  }
  if (n & 1) {
    printf("1");
  } else {
    printf("0");
  }
}

// takes binary number as string formatted with or without a "0b"
// prefix
unsigned int bin2int(char* bin) {
  int d = 0;
  int i = strlen(bin) - 1; // string index (indexing in reverse)
  int bi = 0; // binary index
  while (i >= 0) {
    if (bin[i] == 'b') {
      return d;
    }
    if (bin[i] == '1') {
      d += pow(2, bi);
    }
    i--;
    bi++;
  }
  return d;
}

int main(int argc, char** argv) {
  if (argc < 2) {
    printf("USAGE: dhb [d/h/b] VALUE\n");
    return -1;
  }

  char input = argv[1][0];

  // decimal input
  if (input == 'd') {
    unsigned int n = atoi(argv[2]);
    // print hex
    printf("0x%x\n", n);
    // print binary
    printf("0b");
    printb(n);
  } else if (input == 'h') { // hex input
    unsigned int n = strtol(argv[2], NULL, 16);
    //print decimal
    printf("%d\n", n);
    // print binary
    printf("0b");
    printb(n);
  } else if (input == 'b') { // binary input
    // print decimal
    unsigned int d = bin2int(argv[2]);
    printf("%d\n", d);
    // print hex
    printf("0x%x", d);
  }
  printf("\n");
}
