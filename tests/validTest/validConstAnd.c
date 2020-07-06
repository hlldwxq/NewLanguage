#include <stdio.h>

int fail = 0;
void check(int v,char const *msg) {
  if (!v) {printf("ERROR: Test %s failed\n",msg); fail=1;}
}


int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {

    check(andu128_1(),"andu128_1");
    check(andu128_2(),"andu128_2");
    check(andu128_3(),"andu128_3");

    check(ands128_2(),"ands128_2");
    check(ands128_3(),"ands128_3");
    check(ands128_4(),"ands128_4");
    check(ands128_5(),"ands128_5");
    check(ands128_6(),"ands128_6");
    check(ands128_7(),"ands128_7"); 
    check(ands128_8(),"ands128_8");

    check(andu64_1(),"andu64_1");
    check(andu64_2(),"andu64_2");
    check(andu64_3(),"andu64_3");

    check(ands64_2(),"ands64_2");
    check(ands64_3(),"ands64_3");
    check(ands64_4(),"ands64_4");
    check(ands64_5(),"ands64_5");
    check(ands64_6(),"ands64_6");
    check(ands64_7(),"ands64_7"); 
    check(ands64_8(),"ands64_8");

    check(andu32_1(),"andu32_1");
    check(andu32_2(),"andu32_2");
    check(andu32_3(),"andu32_3");

    check(ands32_2(),"ands32_2");
    check(ands32_3(),"ands32_3");
    check(ands32_4(),"ands32_4");
    check(ands32_5(),"ands32_5");
    check(ands32_6(),"ands32_6");
    check(ands32_7(),"ands32_7"); 
    check(ands32_8(),"ands32_8");

    check(andu16_1(),"andu16_1");
    check(andu16_2(),"andu16_2");
    check(andu16_3(),"andu16_3");

    check(ands16_2(),"ands16_2");
    check(ands16_3(),"ands16_3");
    check(ands16_4(),"ands16_4");
    check(ands16_5(),"ands16_5");
    check(ands16_6(),"ands16_6");
    check(ands16_7(),"ands16_7"); 
    check(ands16_8(),"ands16_8");

    check(andu8_1(),"andu8_1");
    check(andu8_2(),"andu8_2");
    check(andu8_3(),"andu8_3");

    check(ands8_4(),"ands8_4");
    check(ands8_5(),"ands8_5");
    check(ands8_6(),"ands8_6");
    check(ands8_7(),"ands8_7"); 
    check(ands8_8(),"ands8_8");

    if(fail == 0){
        printf("Success!\n");
    }
    return 0;
}

