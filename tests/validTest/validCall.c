#include <stdio.h>

long long callRetInt(long long a);
long long callRetPtr(long long a);
long long callParaInt(long long a);
long long callParaPtr(long long a);

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {
    long long fail = 0;
    long long a = 10;
    //printf("positive:\n");
    // return a
    if(callRetInt(a)!=a){ printf("callRetInt does not pass the test\n");  fail++;}
    if(callRetPtr(a)!=a){ printf("callRetPtr does not pass the test\n");  fail++;}
    if(callParaInt(a)!=a){ printf("callParaInt does not pass the test\n");  fail++;}
    if(callParaPtr(a)!=a){ printf("callParaPtr does not pass the test\n");  fail++;}


    a = -10;
   // printf("negative:\n");
    // return a
    if(callRetInt(a)!=a){ printf("callRetInt does not pass the test\n");  fail++;}
    if(callRetPtr(a)!=a){ printf("callRetPtr does not pass the test\n");  fail++;}
    if(callParaInt(a)!=a){ printf("callParaInt does not pass the test\n");  fail++;}
    if(callParaPtr(a)!=a){ printf("callParaPtr does not pass the test\n");  fail++;} 
    if(fail == 0)
        printf("Success!\n");
 return 0;
}