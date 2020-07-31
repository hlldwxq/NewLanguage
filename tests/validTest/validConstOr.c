#include <stdio.h>

int fail = 0;
void check(int v,char const *msg) {
  if (!v) {printf("ERROR: Test %s failed\n",msg); fail=1;}
}
int oru128_1();
int oru128_2();
int oru128_3();

int ors128_2();
int ors128_3();
int ors128_4();
int ors128_5();
int ors128_6();
int ors128_7(); 
int ors128_8();

int oru64_1();
int oru64_2();
int oru64_3();

int ors64_2();
int ors64_3();
int ors64_4();
int ors64_5();
int ors64_6();
int ors64_7(); 
int ors64_8();

int oru32_1();
int oru32_2();
int oru32_3();

int ors32_2();
int ors32_3();
int ors32_4();
int ors32_5();
int ors32_6();
int ors32_7(); 
int ors32_8();

int oru16_1();
int oru16_2();
int oru16_3();

int ors16_2();
int ors16_3();
int ors16_4();
int ors16_5();
int ors16_6();
int ors16_7(); 
int ors16_8();

int oru8_1();
int oru8_2();
int oru8_3();

int ors8_4();
int ors8_5();
int ors8_6();
int ors8_7(); 
int ors8_8();

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {

    check(oru128_1(),"oru128_1");
    check(oru128_2(),"oru128_2");
    check(oru128_3(),"oru128_3");

    check(ors128_2(),"ors128_2");
    check(ors128_3(),"ors128_3");
    check(ors128_4(),"ors128_4");
    check(ors128_5(),"ors128_5");
    check(ors128_6(),"ors128_6");
    check(ors128_7(),"ors128_7"); 
    check(ors128_8(),"ors128_8");

    check(oru64_1(),"oru64_1");
    check(oru64_2(),"oru64_2");
    check(oru64_3(),"oru64_3");

    check(ors64_2(),"ors64_2");
    check(ors64_3(),"ors64_3");
    check(ors64_4(),"ors64_4");
    check(ors64_5(),"ors64_5");
    check(ors64_6(),"ors64_6");
    check(ors64_7(),"ors64_7"); 
    check(ors64_8(),"ors64_8");

    check(oru32_1(),"oru32_1");
    check(oru32_2(),"oru32_2");
    check(oru32_3(),"oru32_3");

    check(ors32_2(),"ors32_2");
    check(ors32_3(),"ors32_3");
    check(ors32_4(),"ors32_4");
    check(ors32_5(),"ors32_5");
    check(ors32_6(),"ors32_6");
    check(ors32_7(),"ors32_7"); 
    check(ors32_8(),"ors32_8");

    check(oru16_1(),"oru16_1");
    check(oru16_2(),"oru16_2");
    check(oru16_3(),"oru16_3");

    check(ors16_2(),"ors16_2");
    check(ors16_3(),"ors16_3");
    check(ors16_4(),"ors16_4");
    check(ors16_5(),"ors16_5");
    check(ors16_6(),"ors16_6");
    check(ors16_7(),"ors16_7"); 
    check(ors16_8(),"ors16_8");

    check(oru8_1(),"oru8_1");
    check(oru8_2(),"oru8_2");
    check(oru8_3(),"oru8_3");

    check(ors8_4(),"ors8_4");
    check(ors8_5(),"ors8_5");
    check(ors8_6(),"ors8_6");
    check(ors8_7(),"ors8_7"); 
    check(ors8_8(),"ors8_8");

    if(fail == 0){
        printf("Success!\n");
    }
    return 0;
}

