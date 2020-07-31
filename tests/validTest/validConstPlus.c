#include <stdio.h>

int fail = 0;
void check(int v,char const *msg) {
  if (!v) {printf("ERROR: Test %s failed\n",msg); fail=1;}
}

int plusu128_1();
int plusu128_2();
int plusu128_3();

int pluss128_2();
int pluss128_3();
int pluss128_4();
int pluss128_5();
int pluss128_6();
int pluss128_7(); 
int pluss128_8();

int plusu64_1();
int plusu64_2();
int plusu64_3();

int pluss64_2();
int pluss64_3();
int pluss64_4();
int pluss64_5();
int pluss64_6();
int pluss64_7(); 
int pluss64_8();

int plusu32_1();
int plusu32_2();
int plusu32_3();

int pluss32_2();
int pluss32_3();
int pluss32_4();
int pluss32_5();
int pluss32_6();
int pluss32_7(); 
int pluss32_8();

int plusu16_1();
int plusu16_2();
int plusu16_3();

int pluss16_2();
int pluss16_3();
int pluss16_4();
int pluss16_5();
int pluss16_6();
int pluss16_7(); 
int pluss16_8();

int plusu8_1();
int plusu8_2();
int plusu8_3();

int pluss8_4();
int pluss8_5();
int pluss8_6();
int pluss8_7(); 
int pluss8_8();


int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {

    check(plusu128_1(),"plusu128_1");
    check(plusu128_2(),"plusu128_2");
    check(plusu128_3(),"plusu128_3");

    check(pluss128_2(),"pluss128_2");
    check(pluss128_3(),"pluss128_3");
    check(pluss128_4(),"pluss128_4");
    check(pluss128_5(),"pluss128_5");
    check(pluss128_6(),"pluss128_6");
    check(pluss128_7(),"pluss128_7"); 
    check(pluss128_8(),"pluss128_8");

    check(plusu64_1(),"plusu64_1");
    check(plusu64_2(),"plusu64_2");
    check(plusu64_3(),"plusu64_3");

    check(pluss64_2(),"pluss64_2");
    check(pluss64_3(),"pluss64_3");
    check(pluss64_4(),"pluss64_4");
    check(pluss64_5(),"pluss64_5");
    check(pluss64_6(),"pluss64_6");
    check(pluss64_7(),"pluss64_7"); 
    check(pluss64_8(),"pluss64_8");

    check(plusu32_1(),"plusu32_1");
    check(plusu32_2(),"plusu32_2");
    check(plusu32_3(),"plusu32_3");

    check(pluss32_2(),"pluss32_2");
    check(pluss32_3(),"pluss32_3");
    check(pluss32_4(),"pluss32_4");
    check(pluss32_5(),"pluss32_5");
    check(pluss32_6(),"pluss32_6");
    check(pluss32_7(),"pluss32_7"); 
    check(pluss32_8(),"pluss32_8");

    check(plusu16_1(),"plusu16_1");
    check(plusu16_2(),"plusu16_2");
    check(plusu16_3(),"plusu16_3");

    check(pluss16_2(),"pluss16_2");
    check(pluss16_3(),"pluss16_3");
    check(pluss16_4(),"pluss16_4");
    check(pluss16_5(),"pluss16_5");
    check(pluss16_6(),"pluss16_6");
    check(pluss16_7(),"pluss16_7"); 
    check(pluss16_8(),"pluss16_8");

    check(plusu8_1(),"plusu8_1");
    check(plusu8_2(),"plusu8_2");
    check(plusu8_3(),"plusu8_3");

    check(pluss8_4(),"pluss8_4");
    check(pluss8_5(),"pluss8_5");
    check(pluss8_6(),"pluss8_6");
    check(pluss8_7(),"pluss8_7"); 
    check(pluss8_8(),"pluss8_8");

    if(fail == 0){
        printf("Success!\n");
    }
    return 0;
}

