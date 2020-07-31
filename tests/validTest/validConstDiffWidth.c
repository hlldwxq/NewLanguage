#include <stdio.h>
int Sinted8_16_0();
int Sinted8_16_1();
int Sinted8_16_2();
int Sinted8_16_3();
int Sinted16_32_0();
int Sinted16_32_1();
int Sinted16_32_2();
int Sinted16_32_3();
int Uinted8_16_1(); 
int Uinted16_32_2();

int fail = 0;
void check(int v,char const *msg) {
  if (!v) {printf("ERROR: Test %s failed\n",msg); fail=1;}
}

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {

    check(Sinted8_16_0(),"Sinted8_16_0");
    check(Sinted8_16_1(),"Sinted8_16_1");
    check(Sinted8_16_2(),"Sinted8_16_2");
    check(Sinted8_16_3(),"Sinted8_16_3");
    check(Sinted16_32_0(),"Sinted16_32_0");
    check(Sinted16_32_1(),"Sinted16_32_1");
    check(Sinted16_32_2(),"Sinted16_32_2");
    check(Sinted16_32_3(),"Sinted16_32_3");
    check(Uinted8_16_1(),"Uinted8_16_1"); 
    check(Uinted16_32_2(),"Uinted16_32_2");

    if(fail == 0){
        printf("Success!\n");
    }
    return 0;
}

