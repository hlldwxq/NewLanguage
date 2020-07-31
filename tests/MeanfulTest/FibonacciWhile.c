#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
uint64_t Fibonacci(uint64_t);

void check(uint64_t a, uint64_t c) {
  if (Fibonacci(a) != c) {
    printf("ERROR: Fibonacci(%ld) != %ld\n",a,c);
    exit(1);
  }
}

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {
   
    check(1,1);
    check(4,3);
    check(6,8);
    check(8,21);
    check(10,55);
    check(15,610);

    printf("Success!");
    return 0;
}


