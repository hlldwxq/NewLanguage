#include <stdio.h>
unsigned long long f(unsigned long long);
int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {
    printf("result: %llu",f(5));
    return 0;
}