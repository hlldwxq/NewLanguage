#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
uint32_t findContinueNum(uint32_t sum);
uint32_t findMaxNumAccessWhile(uint32_t col, uint32_t row);

uint32_t findNumWithSum(uint32_t count, uint32_t sum);
uint32_t findUniqueNumAccess(uint32_t length);
uint32_t findMaxNumAccessFor(uint32_t col, uint32_t row);

uint32_t findContinueNumC(uint32_t sum){
    uint32_t a = 0;
    int l = 1;
    int r = 1;
    int s = 1;
    while(l<=sum/2){
        if(s<sum){
            r++;
            s+=r;
        }else if(s>sum){
            s-=l;
            l++;
        }else{
            a++;
            s-=l;
            l++;
        }
    }
    return a;
}
uint32_t findNumWithSumC(uint32_t count, uint32_t sum){
    uint32_t a = 0;
    for( uint32_t i = 1 ; i < count ;i++) {
        a = a + i;
        if (a > sum)
            return i - 1;
    }
    return 0;
}

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {
    int fail = 0;

    uint32_t sum = 10;
    if(findContinueNum(sum) != findContinueNumC(sum)){ printf("findContinueNum does not pass the test\n");printf("%d %d\n",findContinueNum(sum),findContinueNumC(sum));  fail++;}

    sum = 50;
    if(findContinueNum(sum) != findContinueNumC(sum)){ printf("findContinueNum does not pass the test\n");printf("%d %d\n",findContinueNum(sum),findContinueNumC(sum));  fail++;}
    
    sum = 100;
    if(findContinueNum(sum) != findContinueNumC(sum)){ printf("findContinueNum does not pass the test\n");printf("%d %d\n",findContinueNum(sum),findContinueNumC(sum));  fail++;}
    
    sum = 500;
    if(findContinueNum(sum) != findContinueNumC(sum)){ printf("findContinueNum does not pass the test\n");printf("%d %d\n",findContinueNum(sum),findContinueNumC(sum));  fail++;}
    
    sum = 1000;
    if(findContinueNum(sum) != findContinueNumC(sum)){ printf("findContinueNum does not pass the test\n");printf("%d %d\n",findContinueNum(sum),findContinueNumC(sum));  fail++;}
    

   uint32_t col = 5;
    uint32_t row = 5;
    if(findMaxNumAccessWhile(col,row) != (col-1)*(row-1)){ printf("findMaxNumAccessWhile does not pass the test\n");  fail++;}
    col = 10;
    row = 10;
    if(findMaxNumAccessWhile(col,row) != (col-1)*(row-1)){ printf("findMaxNumAccessWhile does not pass the test\n");  fail++;}
    col = 100;
    row = 100;
    if(findMaxNumAccessWhile(col,row) != (col-1)*(row-1)){ printf("findMaxNumAccessWhile does not pass the test\n");  fail++;}
    col = 500;
    row = 500;
    if(findMaxNumAccessWhile(col,row) != (col-1)*(row-1)){ printf("findMaxNumAccessWhile does not pass the test\n");  fail++;}
    col = 1000;
    row = 1000;
    if(findMaxNumAccessWhile(col,row) != (col-1)*(row-1)){ printf("findMaxNumAccessWhile does not pass the test\n");  fail++;}


    uint32_t count = 100; 
    sum = 100;
    if(findNumWithSum(count,sum)!=findNumWithSumC(count,sum)){ printf("findNumWithSum does not pass the test\n");  fail++;}
    count = 100; 
    sum = 1000;
    if(findNumWithSum(count,sum)!=findNumWithSumC(count,sum)){ printf("findNumWithSum does not pass the test\n");  fail++;}
    count = 1000; 
    sum = 5000;
    if(findNumWithSum(count,sum)!=findNumWithSumC(count,sum)){ printf("findNumWithSum does not pass the test\n");  fail++;}
    count = 1000; 
    sum = 10000;
    if(findNumWithSum(count,sum)!=findNumWithSumC(count,sum)){ printf("findNumWithSum does not pass the test\n");  fail++;}
    count = 2000; 
    sum = 50000;
    if(findNumWithSum(count,sum)!=findNumWithSumC(count,sum)){ printf("findNumWithSum does not pass the test\n");  fail++;}


    uint32_t length = 11;
    if(findUniqueNumAccess(length) != length*length){ printf("findUniqueNumAccess does not pass the test\n %d\n",findUniqueNumAccess(length));  fail++;}
    length = 101;
    if(findUniqueNumAccess(length) != length*length){ printf("findUniqueNumAccess does not pass the test\n %d\n",findUniqueNumAccess(length));  fail++;}
    length = 501;
    if(findUniqueNumAccess(length) != length*length){ printf("findUniqueNumAccess does not pass the test\n %d\n",findUniqueNumAccess(length));  fail++;}
    length = 1001;
    if(findUniqueNumAccess(length) != length*length){ printf("findUniqueNumAccess does not pass the test\n %d\n",findUniqueNumAccess(length));  fail++;}
    length = 5001;
    if(findUniqueNumAccess(length) != length*length){ printf("findUniqueNumAccess does not pass the test\n %d\n",findUniqueNumAccess(length));  fail++;}


    col = 5;
    row = 5;
    if(findMaxNumAccessFor(col,row) != (col-1)*(row-1)){ printf("findMaxNumAccessFor does not pass the test\n");  fail++;}
    col = 10;
    row = 10;
    if(findMaxNumAccessFor(col,row) != (col-1)*(row-1)){ printf("findMaxNumAccessFor does not pass the test\n");  fail++;}
    col = 100;
    row = 100;
    if(findMaxNumAccessFor(col,row) != (col-1)*(row-1)){ printf("findMaxNumAccessFor does not pass the test\n");  fail++;}
    col = 500;
    row = 500;
    if(findMaxNumAccessFor(col,row) != (col-1)*(row-1)){ printf("findMaxNumAccessFor does not pass the test\n");  fail++;}
    col = 1000;
    row = 1000;
    if(findMaxNumAccessFor(col,row) != (col-1)*(row-1)){ printf("findMaxNumAccessFor does not pass the test\n");  fail++;}
    

    if(fail == 0)
        printf("Success!\n");
    return 0;
}