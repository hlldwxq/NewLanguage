#include <stdio.h>
int divu128_1();
int divu128_2();

int divs128_2();
int divs128_3();
int divs128_4();
int divs128_5();
int divs128_6();
int divs128_7();
int divs128_8();

int divu64_1();
int divu64_2();

int divs64_2();
int divs64_3();
int divs64_4();
int divs64_5();
int divs64_6();
int divs64_7();
int divs64_8();

int divu32_1();
int divu32_2();

int divs32_2();
int divs32_3();
int divs32_4();
int divs32_5();
int divs32_6();
int divs32_7();
int divs32_8();

int divu16_1();
int divu16_2();

int divs16_2();
int divs16_3();
int divs16_4();
int divs16_5();
int divs16_6();
int divs16_7();
int divs16_8();

int divu8_1();
int divu8_2();

int divs8_2();
int divs8_3();
int divs8_4();
int divs8_5();
int divs8_6();
int divs8_7();
int divs8_8();
int fail = 0;
void check(int v,char const *msg) {
  if (!v) {printf("ERROR: Test %s failed\n",msg); fail=1;}
}

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {

    check(divu128_1(),"divu128_1");
    check(divu128_2(),"divu128_2");

    check(divs128_2(),"divu128_2");
    check(divs128_3(),"divu128_3");
    check(divs128_4(),"divu128_4");
    check(divs128_5(),"divu128_5");
    check(divs128_6(),"divu128_6");
    check(divs128_7(),"divu128_7");
    check(divs128_8(),"divu128_8");

    check(divu64_1(),"divu64_1");
    check(divu64_2(),"divu64_2");

    check(divs64_2(),"divu64_2");
    check(divs64_3(),"divu64_3");
    check(divs64_4(),"divu64_4");
    check(divs64_5(),"divu64_5");
    check(divs64_6(),"divu64_6");
    check(divs64_7(),"divu64_7");
    check(divs64_8(),"divu64_8");

    check(divu32_1(),"divu32_1");
    check(divu32_2(),"divu32_2");

    check(divs32_2(),"divu32_2");
    check(divs32_3(),"divu32_3");
    check(divs32_4(),"divu32_4");
    check(divs32_5(),"divu32_5");
    check(divs32_6(),"divu32_6");
    check(divs32_7(),"divu32_7");
    check(divs32_8(),"divu32_8");

    check(divu16_1(),"divu16_1");
    check(divu16_2(),"divu16_2");

    check(divs16_2(),"divu16_2");
    check(divs16_3(),"divu16_3");
    check(divs16_4(),"divu16_4");
    check(divs16_5(),"divu16_5");
    check(divs16_6(),"divu16_6");
    check(divs16_7(),"divu16_7");
    check(divs16_8(),"divu16_8");

    check(divu8_1(),"divu8_1");
    check(divu8_2(),"divu8_2");

    check(divs8_2(),"divu8_2");
    check(divs8_3(),"divu8_3");
    check(divs8_4(),"divu8_4");
    check(divs8_5(),"divu8_5");
    check(divs8_6(),"divu8_6");
    check(divs8_7(),"divu8_7");
    check(divs8_8(),"divu8_8");

    if(fail == 0){
        printf("Success!\n");
    }
    return 0;
}

