#include <stdio.h>

bool test1();
bool test2();

bool fail=false;

void check(bool v,char const *msg) {
  if (!v) {printf("ERROR: Test %s failed"); fail=1;}
}


int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {
    check(test1(),"test1");
    check(test2(),"test2");

    if(fail == 0){
        printf("Success!\n");
    }
    return 0;
}

