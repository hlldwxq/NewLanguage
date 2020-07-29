#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {void * p;} *array;

array newarray(uint64_t);
void setarray(array,uint64_t,uint64_t);
uint64_t getarray(array,uint64_t);

void sort(array, uint64_t, uint64_t);
void quicksort_aux(array, uint64_t, uint64_t);
void final_insertion_sort(array, uint64_t, uint64_t);

void prl() {printf ("\n");}
void pr(uint64_t x) {
  printf("%ld ",x);
}
void prs(char* x) {
  printf("%s ",x);
}

void check(int n){
  srand(0);
  array a = newarray(n);
  for (size_t i=0;i<n;++i) 
    setarray(a,i,rand()%n);

  sort(a,0,n);

  for (size_t i=0;i+1<n;++i) {
    if (getarray(a,i) > getarray(a,i+1)) {
      printf("introsort failed\n");
      exit(1);
//       return 0;
    }
  }
//   return 1;
}

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {

  
  for (size_t i=0; i<10000000; i= i?i*2:1 ) check(i);


//   check(0);
//   check(1);
//   check(2);
//   check(20);
//   check(20);
//   check(50);
//   check(50);
   check(100);
//   check(100);

  printf("Success!\n");
  return 0;
}
