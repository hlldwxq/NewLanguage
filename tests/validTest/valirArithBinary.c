#include <stdio.h>

long long plusNum200();
long long plusVar(long long a,long long b);
long long plusCall(long long a,long long b);
long long plusArrIndex(long long a,long long b);
long long minusNum0();
long long minusVar(long long a,long long b);
long long minusCall(long long a,long long b);
long long minusArrIndex(long long a,long long b);
long long starNum100();
long long starVar(long long a,long long b);
long long starCall(long long a,long long b);
long long starArrIndex(long long a,long long b);
long long divNum1();
long long divVar(long long a,long long b);
long long divCall(long long a,long long b);
long long divArrIndex(long long a,long long b);
long long andNum4();
long long andVar(long long a,long long b);
long long andCall(long long a,long long b);
long long andArrIndex(long long a,long long b);
long long orNum4();
long long orVar(long long a,long long b);
long long orCall(long long a,long long b);
long long orArrIndex(long long a,long long b);

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {
    long long fail = 0;
    long long a = 101;
    long long b = 10;

    if( 200 != plusNum200()){printf("plusNum200 does not pass the test\n");fail++;}
    if( (a+b) !=  plusVar(a,b)){printf("plusVar does not pass the test\n");fail++;}
    if( (a+b) !=  plusCall(a,b)){printf("plusCall does not pass the test\n");fail++;}
    if( (a+b) !=  plusArrIndex(a,b)){printf("plusArrIndex does not pass the test\n");fail++;}

    if( 0 !=  minusNum0()){printf("minusNum0 does not pass the test\n");fail++;}
    if( (a-b) !=  minusVar(a,b)){printf("minusVar does not pass the test\n");fail++;}
    if( (a-b) !=  minusCall(a,b)){printf("minusCall does not pass the test\n");fail++;}
    if( (a-b) !=  minusArrIndex(a,b)){printf("minusArrIndex does not pass the test\n");fail++;}

    if( 100 !=  starNum100()){printf("starNum100 does not pass the test\n");fail++;}
    if( (a*b) !=  starVar(a,b)){printf("starVar does not pass the test\n");fail++;}
    if( (a*b) !=  starCall(a,b)){printf("starCall does not pass the test\n");fail++;}
    if( (a*b) !=  starArrIndex(a,b)){printf("starArrIndex does not pass the test\n");fail++;}

    if( 1 !=  divNum1()){printf("divNum1 does not pass the test\n");fail++;}
    if( (a/b) !=  divVar(a,b)){printf("divVar does not pass the test\n");fail++;}
    if( (a/b) !=  divCall(a,b)){printf("divCall does not pass the test\n");fail++;}
    if( (a/b) !=  divArrIndex(a,b)){printf("divArrIndex does not pass the test\n");fail++;}

    if( 4 !=  andNum4()){printf("andNum4 does not pass the test\n");fail++;}
    if( (a&b) !=  andVar(a,b)){printf("andVar does not pass the test\n");fail++;}
    if( (a&b) !=  andCall(a,b)){printf("andCall does not pass the test\n");fail++;}
    if( (a&b) !=  andArrIndex(a,b)){printf("andArrIndex does not pass the test\n");fail++;}
    
    if( 4 !=  orNum4()){printf("orNum4 does not pass the test\n");fail++;}
    if( (a|b) !=  orVar(a,b)){printf("orVar does not pass the test\n");fail++;}
    if( (a|b) !=  orCall(a,b)){printf("orCall does not pass the test\n");fail++;}
    if( (a|b) !=  orArrIndex(a,b)){printf("orArrIndex does not pass the test\n");fail++;}

    if(fail == 0){
        printf("Success!\n");
    }
    return 0;
}
