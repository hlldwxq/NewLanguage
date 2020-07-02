#include <stdio.h>

long long if_2ret(long long  a,long long  b);
long long if_then_ret(long long a,long long b);
long long if_else_ret(long long a,long long b);
long long if_0ret(long long a,long long b);



long long max(long long a,long long b){
    if(a>b)
    return a;
    else
    return b;
}

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {
    int fail = 0;
    long long a = 10;
    long long b = 1;
 
    if(if_2ret(a,b)!=max(a,b)){ printf("if_2ret does not pass the test %lld %lld\n",max(a,b),if_2ret(a,b)); }
    if(if_then_ret(a,b)!=max(a,b)){ printf("if_then_ret does not pass the test %lld %lld\n",max(a,b),if_then_ret(a,b)); }
    if(if_else_ret(a,b)!=max(a,b)){ printf("if_else_ret does not pass the test %lld %lld\n",max(a,b),if_else_ret(a,b)); }
    if(if_0ret(a,b)!=max(a,b)){ printf("i_0ret does not pass the test %lld %lld\n",max(a,b),if_0ret(a,b)); }


    a = -10;
    b = -1;

    if(if_2ret(a,b)!=max(a,b)){ printf("if_2ret does not pass the test %lld %lld\n",max(a,b),if_2ret(a,b)); }
    if(if_then_ret(a,b)!=max(a,b)){ printf("if_then_ret does not pass the test %lld %lld\n",max(a,b),if_then_ret(a,b)); }
    if(if_else_ret(a,b)!=max(a,b)){ printf("if_else_ret does not pass the test %lld %lld\n",max(a,b),if_else_ret(a,b)); }
    if(if_0ret(a,b)!=max(a,b)){ printf("i_0ret does not pass the test %lld %lld\n",max(a,b),if_0ret(a,b)); }

  
    a = 10;
    b = -10;

    if(if_2ret(a,b)!=max(a,b)){ printf("if_2ret does not pass the test %lld %lld\n",max(a,b),if_2ret(a,b)); }
    if(if_then_ret(a,b)!=max(a,b)){ printf("if_then_ret does not pass the test %lld %lld\n",max(a,b),if_then_ret(a,b)); }
    if(if_else_ret(a,b)!=max(a,b)){ printf("if_else_ret does not pass the test %lld %lld\n",max(a,b),if_else_ret(a,b)); }
    if(if_0ret(a,b)!=max(a,b)){ printf("i_0ret does not pass the test %lld %lld\n",max(a,b),if_0ret(a,b)); }

    if(fail == 0){
        printf("Success!\n");
    }
    return 0;
}