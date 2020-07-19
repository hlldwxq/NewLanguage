#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
uint64_t euclid(uint64_t,uint64_t);

void check(uint64_t a, uint64_t b, uint64_t c) {
  if (euclid(a,b) != c) {
    printf("ERROR: euclid(%lu, %lu) != %lu\n",a,b,c);
    exit(1);
  }
}


int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {
    uint64_t a, b;

    if (argc!=3) {
      check(0,2,0);
      check(2,0,0);
      check(1,2,1);
      check(5,20,5);
      check(15,20,5);
      check(101,100,1);
      check(100,100,100);
      printf("Success!");
      return 0;
    }
    else {
      a = atol(argv[1]);
      b = atol(argv[2]);
    }

    
    return 0;
}


