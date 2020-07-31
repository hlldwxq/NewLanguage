#include <stdio.h>

int fail = 0;
void check(int v,char const *msg) {
  if (!v) {printf("ERROR: Test %s failed\n",msg); fail=1;}
}

int minusu128_1();
int minusu128_2();
int minusu128_3();

int minuss128_2();
int minuss128_3();
int minuss128_4();
int minuss128_5();

int minusu64_1();
int minusu64_2();
int minusu64_3();

int minuss64_2();
int minuss64_3();
int minuss64_4();
int minuss64_5();

int minusu32_1();
int minusu32_2();
int minusu32_3();

int minuss32_2();
int minuss32_3();
int minuss32_4();
int minuss32_5();

int minusu16_1();
int minusu16_2();
int minusu16_3();

int minuss16_2();
int minuss16_3();
int minuss16_4();
int minuss16_5();

int minusu8_1();
int minusu8_2();
int minusu8_3();

int minuss8_2();
int minuss8_3();
int minuss8_4();
int minuss8_5();

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {

    check(minusu128_1(),"minusu128_1");
    check(minusu128_2(),"minusu128_2");
    check(minusu128_3(),"minusu128_3");

    check(minuss128_2(),"minuss128_2");
    check(minuss128_3(),"minuss128_3");
    check(minuss128_4(),"minuss128_4");
    check(minuss128_5(),"minuss128_5");

    check(minusu64_1(),"minusu64_1");
    check(minusu64_2(),"minusu64_2");
    check(minusu64_3(),"minusu64_3");

    check(minuss64_2(),"minuss64_2");
    check(minuss64_3(),"minuss64_3");
    check(minuss64_4(),"minuss64_4");
    check(minuss64_5(),"minuss64_5");

    check(minusu32_1(),"minusu32_1");
    check(minusu32_2(),"minusu32_2");
    check(minusu32_3(),"minusu32_3");

    check(minuss32_2(),"minuss32_2");
    check(minuss32_3(),"minuss32_3");
    check(minuss32_4(),"minuss32_4");
    check(minuss32_5(),"minuss32_5");

    check(minusu16_1(),"minusu16_1");
    check(minusu16_2(),"minusu16_2");
    check(minusu16_3(),"minusu16_3");

    check(minuss16_2(),"minuss16_2");
    check(minuss16_3(),"minuss16_3");
    check(minuss16_4(),"minuss16_4");
    check(minuss16_5(),"minuss16_5");

    check(minusu8_1(),"minusu8_1");
    check(minusu8_2(),"minusu8_2");
    check(minusu8_3(),"minusu8_3");

    check(minuss8_2(),"minuss8_2");
    check(minuss8_3(),"minuss8_3");
    check(minuss8_4(),"minuss8_4");
    check(minuss8_5(),"minuss8_5");

    if(fail == 0){
        printf("Success!\n");
    }
    return 0;
}

