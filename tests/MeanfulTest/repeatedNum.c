#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
long long duplicate(long long* numberList,long long length); //Calculate which Fibonacci number is less or equal than the given value

void check(long long* a, long long num,long long len) {

  if (duplicate(a,len) != num) {
    printf("ERROR: duplicate(%lld) != %lld\n",num,num);
    exit(1);
  }

}

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {
    long long *p;
   
    long long listL[6] = {1,2,3,4,5,1};
    p = listL;
    check(p,1,6);

    long long listL1[6] = {1,2,1,4,5,1};
    p = listL1;
    check(p,1,6);

    long long listL2[5] = {1,2,7,4,5};
    p = listL2;
    check(p,0,5);

    return 0;
}