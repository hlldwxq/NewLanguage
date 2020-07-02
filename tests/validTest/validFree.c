#include <stdio.h>
#include <stdint.h>

long long testNewSint(long long size,long long value);
long long testNewUint(long long size,long long value);
void testNewOvf(uint64_t n);

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {
    long long fail = 0;
    long long s = 4;
    long long a = 10;
    long long b = -10;
    
    long long i=testNewUint(s,a);printf("%d\n",i);
    long long i1=testNewSint(s,b);printf("%d\n",i1);
    
    if(fail == 0){
        printf("Success!\n");
    }
    return 0;
}
