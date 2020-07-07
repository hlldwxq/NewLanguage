#include <stdio.h>
#include <stdint.h>
void test();

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {
    test();
    printf("Success!\n");
    return 0;
}