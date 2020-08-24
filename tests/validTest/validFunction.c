#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

void protoType1();
void protoType2(uint32_t a);
uint32_t protoType3();
uint32_t protoType4(uint32_t a);

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {
    

    protoType1();
    protoType2(0);
    protoType3();
    protoType4(0);
    printf("Success!");
    return 0;
}