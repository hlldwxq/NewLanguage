#include <stdio.h>

long long while_break1(long long a);
long long while_break2(long long a);
long long while_scope(long long a);

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {
    long long fail = 0;
    long long a = 10;
  //  printf("positive:\n");
    // return a
    if(while_break1(a)!=a){ printf("postive: while_break1 does not pass the test\n"); fail++;}
    if(while_break2(a)!=a){ printf("postive: while_break2 does not pass the test\n"); fail++; }
    if(while_scope(a)!=a){ printf("postive: while_scope does not pass the test\n"); fail++; }
   
  //  fail = 0;
    a = -10;
   // printf("negative:\n");
    // return a
    if(while_break1(a)!=a){ printf("negative: while_break1 does not pass the test\n"); fail++; }
    if(while_break2(a)!=a){ printf("negative: while_break2 does not pass the test\n"); fail++; }
    if(while_scope(a)!=a){ printf("negative: while_scope does not pass the test\n"); fail++; }
    if(fail == 0)
        printf("Success!\n");

    return 0;
}