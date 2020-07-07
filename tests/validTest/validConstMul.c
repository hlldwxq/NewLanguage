#include <stdio.h>

int fail = 0;
void check(int v,char const *msg) {
  if (!v) {printf("ERROR: Test %s failed\n",msg); fail=1;}
}

int mulu128_1();
int mulu128_2();
int mulu128_3();
int muls128_2();
int muls128_3();
int muls128_4();
int muls128_5();
int muls128_6();
int muls128_7();

int mulu64_1();
int mulu64_2();
int mulu64_3();
int muls64_2();
int muls64_3();
int muls64_4();
int muls64_5();
int muls64_6();
int muls64_7();

int mulu32_1();
int mulu32_2();
int mulu32_3();
int muls32_2();
int muls32_3();
int muls32_4();
int muls32_5();
int muls32_6();
int muls32_7();

int mulu16_1();
int mulu16_2();
int mulu16_3();
int muls16_2();
int muls16_3();
int muls16_4();
int muls16_5();
int muls16_6();
int muls16_7();

int mulu8_1();
int mulu8_2();
int mulu8_3();
int muls8_2();
int muls8_3();
int muls8_4();
int muls8_5();
int muls8_6();
int muls8_7();

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {

    check(mulu128_1(),"mulu128_1");
    check(mulu128_2(),"mulu128_2");
    check(mulu128_3(),"mulu128_3");
    check(muls128_2(),"muls128_2");
    check(muls128_3(),"muls128_3");
    check(muls128_4(),"muls128_4");
    check(muls128_5(),"muls128_5");
    check(muls128_6(),"muls128_6");
    check(muls128_7(),"muls128_7");

    check(mulu64_1(),"mulu64_1");
    check(mulu64_2(),"mulu64_2");
    check(mulu64_3(),"mulu64_3");
    check(muls64_2(),"muls64_2");
    check(muls64_3(),"muls64_3");
    check(muls64_4(),"muls64_4");
    check(muls64_5(),"muls64_5");
    check(muls64_6(),"muls64_6");
    check(muls64_7(),"muls64_7");

    check(mulu32_1(),"mulu32_1");
    check(mulu32_2(),"mulu32_2");
    check(mulu32_3(),"mulu32_3");
    check(muls32_2(),"muls32_2");
    check(muls32_3(),"muls32_3");
    check(muls32_4(),"muls32_4");
    check(muls32_5(),"muls32_5");
    check(muls32_6(),"muls32_6");
    check(muls32_7(),"muls32_7");

    check(mulu16_1(),"mulu16_1");
    check(mulu16_2(),"mulu16_2");
    check(mulu16_3(),"mulu16_3");
    check(muls16_2(),"muls16_2");
    check(muls16_3(),"muls16_3");
    check(muls16_4(),"muls16_4");
    check(muls16_5(),"muls16_5");
    check(muls16_6(),"muls16_6");
    check(muls16_7(),"muls16_7");

    check(mulu8_1(),"mulu8_1");
    check(mulu8_2(),"mulu8_2");
    check(mulu8_3(),"mulu8_3");
    check(muls8_2(),"muls8_2");
    check(muls8_3(),"muls8_3");
    check(muls8_4(),"muls8_4");
    check(muls8_5(),"muls8_5");
    check(muls8_6(),"muls8_6");
    check(muls8_7(),"muls8_7");

    if(fail == 0){
        printf("Success!\n");
    }
    return 0;
}

