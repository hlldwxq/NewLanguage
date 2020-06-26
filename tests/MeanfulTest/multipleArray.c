#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
/// give an array A, make a new array B, which B[i]=A[0]*A[1]***A[i-1]*A[i+1]***A[n]
long long* multiply(long long* A, long long length);
int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {
    
    long long A[] = {1,2,3,4,5,6,7,8,9,10};
    long long *ptr = A;
    long long *ptrB = multiply(ptr,10);

    for(int i=0;i<10;i++){
        long long b = ptrB[i];
        long long expected = 1;
        for(int j=0;j<10;j++){
            if(j!=i){
                expected *= A[j];
            }
        }
        if(b!=expected){
            printf("ERROR: multiply %d %lld %lld\n",i,b,expected);
            exit(1);
        }
    }

    return 0;
}