#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
uint64_t ReverseFibonacci(uint64_t); //Calculate which Fibonacci number is less or equal than the given value

void check(uint64_t a, uint64_t c) {
  if (ReverseFibonacci(a) != c) {
    printf("ERROR: ReverseFibonacci(%ld) != %ld\n",a,c);
    exit(1);
  }
}

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {
   
    check(1,1);
    check(3,4);
    check(9,6);
    check(22,8);
    check(60,10);
    check(610,15);

    printf("Success!");
    return 0;
}


