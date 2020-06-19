#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
uint64_t mytest(uint64_t n);
int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {
    uint64_t n;

    if (argc!=2) n=1000;
    else n = atol(argv[1]);

    printf("Product %lu\n", mytest(n));
    return 0;
}


