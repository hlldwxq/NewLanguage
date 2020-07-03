#include <stdio.h>

long long for_binary_pos(long long a);
long long for_binary_neg(long long a);
long long for_break1(long long a);
long long for_break2(long long a);
long long for_scope(long long a);

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {
    int fail = 0;
    long long a = -10;

    if(for_binary_pos(a)!=a){ printf("postive for_binary_pos does not pass the test\n"); fail++;}
    if(for_binary_neg(a)!=a){ printf("postive for_binary_neg does not pass the test\n"); fail++;}
    if(for_break1(a)!=a){ printf("postive for_break1 does not pass the test\n"); fail++;}
    if(for_break2(a)!=a){ printf("postive for_break2 does not pass the test\n"); fail++;}
    if(for_scope(a)!=a){ printf("postive for_scope does not pass the test\n"); fail++;}


    a = -10;

    if(for_binary_pos(a)!=a){ printf("negative for_binary_pos does not pass the test\n"); fail++;}
    if(for_binary_neg(a)!=a){ printf("negative for_binary_neg does not pass the test\n"); fail++;}
    if(for_break1(a)!=a){ printf("negative for_break1 does not pass the test\n"); fail++;}
    if(for_break2(a)!=a){ printf("negative for_break2 does not pass the test\n"); fail++;}
    if(for_scope(a)!=a){ printf("negative for_scope does not pass the test\n"); fail++;}
    if(fail == 0){
        printf("Success!");
    }
    return 0;
}