#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
void retVoid1();
void retVoid2(int a); // abtract value
void retVoid3(int a); // neg value
void retVoid4(uint32_t b);
void retVoid5(uint32_t b);
void retVoid6(uint32_t sum);

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {
    
    long long fail =0 ;
    retVoid1();

    retVoid2(0);
    retVoid2(10);
    retVoid2(100);
    retVoid2(1000);
    retVoid2(10000);
    retVoid2(-10);
    retVoid2(-100);
    retVoid2(-1000);
    retVoid2(-10000);

    retVoid3(0);
    retVoid3(10);
    retVoid3(100);
    retVoid3(1000);
    retVoid3(10000);
    retVoid3(-10);
    retVoid3(-100);
    retVoid3(-1000);
    retVoid3(-10000);

    retVoid4(0);
    retVoid4(10);
    retVoid4(100);
    retVoid4(1000);
    retVoid4(10000);

    retVoid5(0);
    retVoid5(10);
    retVoid5(100);
    retVoid5(1000);
    retVoid5(10000);

    retVoid6(10);
    retVoid6(100);
    retVoid6(1000);
    retVoid6(10000);

    if(fail==0){
        printf("Success!\n");
    }
    return 0;
}