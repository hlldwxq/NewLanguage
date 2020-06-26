#include <stdio.h>

long long testNewSint(long long size,long long value);
long long testNewUint(long long size,long long value);

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {
    long long fail = 0;
    long long s = 4;
    long long a = 10;
    long long b = -10;
    
    if(a!=testNewUint(s,a)){printf("testNewUint does not pass the test\n");fail++;}
    if(b!=testNewSint(s,b)){printf("testNewSint does not pass the test\n");fail++;}
    if(fail == 0){
        printf("Success!\n");
    }
    return 0;
}