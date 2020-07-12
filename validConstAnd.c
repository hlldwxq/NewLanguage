#include <stdio.h>

int fail = 0;
void check(int v,char const *msg) {
  if (!v) {printf("ERROR: Test %s failed\n",msg); fail=1;}
}

void andu128_2();

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {


    andu128_2();

    if(fail == 0){
        printf("Success!\n");
    }
    return 0;
}

