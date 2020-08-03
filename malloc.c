#include "malloc.h"

void memdump(char *adr) {
  printf("%.4hhd ", *adr);
}

int main(int c, char **v) {
  char *test, *test3;
  unsigned int *test2;
  int i;
  i = -4;


  long u;
  u = getpagesize();
  printf("%ld     ", u);
  test = malloc(1);
  //test2 = malloc(1);
  *test = 'c';
  *(test + 1) = 'h';

  while (i < 10) {
    memdump(test + i);
    i++;
  }
  //test3 = (int *)malloc(1024);
  //test2 = test3 - 1;
  //printf("%p   %p    %p\n", test, test2, test3);
  //printf("%X\n", *test2);
}
