#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {void * p;} *array;

array newarray(uint64_t);
void setarray(array,uint64_t,uint64_t);
uint64_t getarray(array,uint64_t);

void quicksort_aux(array,uint64_t,uint64_t);

void prl() {printf ("\n");}
void pr(uint64_t x) {
  printf("%ld ",x);
}

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {
  srand(0);
  size_t n=2;
  array a = newarray(n);
  for (size_t i=0;i<n;++i) setarray(a,i,rand());

  quicksort_aux(a,0,n);

  for (size_t i=0;i<n-1;++i) {
    if (getarray(a,i) > getarray(a,i+1)) {
      printf("Not sorted\n");
      return 1;
    }
  }

//   for (size_t i=0;i<n;++i) printf("%ld ",getarray(a,i));
//   printf("\n");

  printf("Success!\n");
  return 0;
}


