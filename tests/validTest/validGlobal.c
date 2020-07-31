#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

char* strTest0();
char* strTest1(char* str);
char* strTest2();
uint32_t utestPtrArg(uint32_t n);
long long stest0();
long long stest100();
long long stest(long long arg);
long long stest100arg(long long arg);
uint64_t utest0();
uint64_t utest100();
uint64_t utest(uint64_t arg);
uint64_t utest100arg(uint64_t arg);

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {
    int fail = 0;
    
    char* empty = "";
    if( strTest0()!=empty){ printf("strTest0 does not pass the test\n"); fail++; }
    
    char* str = "edgrthyeht";
    if( strTest1(str)!=str){ printf("strTest1 does not pass the test\n"); fail++; }

    char* str2 = "abc";
    if( strTest2()!=str2){ printf("strTest2 does not pass the test\n"); fail++; }


    uint32_t n1 = 100;
    if (utestPtrArg(n1)!=n1){ printf("utestPtrArg does not pass the test\n");fail++;}
    if (stest0()!=0){ printf("stest0 does not pass the test\n");fail++;}
    if (stest100()!=100){ printf("stest100 does not pass the test\n");fail++;}

    long long arg = 500;
    if (stest(arg)!=arg){ printf("stest does not pass the test\n");fail++;}
    if ( stest100arg(arg)!=arg+100){ printf("stest100arg does not pass the test\n");fail++;}
    if ( utest0()!=0){ printf("utest0 does not pass the test\n");fail++;}
    if ( utest100()!=100){ printf("utest100 does not pass the test\n");fail++;}

    uint64_t arg1 = 20;
    if (utest(arg1)!=arg1){ printf("utest does not pass the test\n");fail++;}
    if (utest100arg(arg)!=arg+100){ printf("utest100arg does not pass the test\n");fail++;}

    if(fail == 0){
        printf("Success!");
    }
    return 0;
}