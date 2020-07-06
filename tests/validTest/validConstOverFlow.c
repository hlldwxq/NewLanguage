#include <stdio.h>
int Sinted8_0();
int Sinted8_1();
int Sinted16_1();
int Sinted8_16_2();
int Sinted32_0();
int Sinted32_1();
int Sinted64_0();
int Sinted64_1();
int Sinted128();

int fail = 0;
void check(int v,char const *msg) {
  if (!v) {printf("ERROR: Test %s failed\n",msg); fail=1;}
}

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {

    check(Sinted8_0(),"Sinted8_0");
    check(Sinted8_1(),"Sinted8_1");
    check(Sinted16_1(),"Sinted16_1");
    check(Sinted16_2(),"Sinted16_2");
    check(Sinted32_0(),"Sinted32_0");
    check(Sinted32_1(),"Sinted32_1");
    check(Sinted64_0(),"Sinted64_0");
    check(Sinted64_1(),"Sinted64_1");
    check(Sinted128(),"Sinted128");

    if(fail == 0){
        printf("Success!\n");
    }
    return 0;
}

