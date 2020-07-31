#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {void * p;} *array;

array newarray(uint64_t);
void setarray(array,uint64_t,uint64_t);
uint64_t getarray(array,uint64_t);

void sort_heap(array, uint64_t, uint64_t);

void prl() {
  printf ("\n");
}

void pr(uint64_t x) {
  printf("%ld ",x);
}

void prs(char* x) {
  printf("%s ",x);
}


int check(int n){
  srand(0);
  array a = newarray(n);
  for (size_t i=0;i<n;++i) 
    setarray(a,i,rand()%n);
    
  make_heap(a,0,n);
  sort_heap(a,0,n);
  for (size_t i=0;i<n-1;++i) {
    if (getarray(a,i) > getarray(a,i+1)) {
      return 0;
    }
  }
  return 1;
}

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {
  int failed = 0;
  
  if(check(2)==0){ failed++; printf("heapsort failed\n"); }
  if(check(20)==0){ failed++; printf("heapsort failed\n"); }
  if(check(20)==0){ failed++; printf("heapsort failed\n"); }
  if(check(50)==0){ failed++; printf("heapsort failed\n"); }
  if(check(50)==0){ failed++; printf("heapsort failed\n"); }
  if(check(100)==0){ failed++; printf("heapsort failed\n"); }
  if(check(100)==0){ failed++; printf("heapsort failed\n"); }

  if(failed==0)
    printf("Success!\n");
  
  return 0;
}


