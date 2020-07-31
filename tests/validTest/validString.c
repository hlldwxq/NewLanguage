#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
stringDef();
stringAssign();
stringDefault();
stringPara1(char* s);
stringPara2(char* s);
char* stringRet1();
char* stringRet2();
char* stringRet3();
stringHelp(char* s);
stringDiffSymbol();

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {
    
    long long fail =0 ;
    
    stringDef();
    stringAssign();
    stringDefault();
    stringDiffSymbol();

    char* s = "abcdefg";
    stringPara1(s);

    s = "1234567";
    stringPara2(s);
    
    s = "gaufyd678342hdfjk";
    stringHelp(s);

    s = "abcdefghigklmn";
    if(*(stringRet1(s)) != *s){    printf("stringRet1 does not pass the test");    fail++;  }
    s = "123456";
    if(*(stringRet1(s)) != *s){    printf("stringRet1 does not pass the test");    fail++;  }
    s = "abc35345lmn";
    if(*(stringRet1(s)) != *s){    printf("stringRet1 does not pass the test");    fail++;  }
    s = "!@#$^&*()-=+;,.<>?{}[]";
    if(*(stringRet1(s)) != *s){    printf("stringRet1 does not pass the test");    fail++;  }
    s = "  abc35  345lmn  ";
    if(*(stringRet1(s)) != *s){    printf("stringRet1 does not pass the test");    fail++;  }


    s = "abcdefghigklmn";
    if(*(stringRet2(s)) != *s){    printf("stringRet2 does not pass the test");    fail++;  }
    s = "123456";
    if(*(stringRet2(s)) != *s){    printf("stringRet2 does not pass the test");    fail++;  }
    s = "abc35345lmn";
    if(*(stringRet2(s)) != *s){    printf("stringRet2 does not pass the test");    fail++;  }
    s = "!@#$^&*()-=+;,.<>?{}[]";
    if(*(stringRet2(s)) != *s){    printf("stringRet2 does not pass the test");    fail++;  }
    s = "  abc35  345lmn  ";
    if(*(stringRet2(s)) != *s){    printf("stringRet2 does not pass the test");    fail++;  }


    s = "abcdefghigklmn";
    if(*(stringRet3(s)) != *s){    printf("stringRet3 does not pass the test");    fail++;  }
    s = "123456";
    if(*(stringRet3(s)) != *s){    printf("stringRet3 does not pass the test");    fail++;  }
    s = "abc35345lmn";
    if(*(stringRet3(s)) != *s){    printf("stringRet3 does not pass the test");    fail++;  }
    s = "!@#$^&*()-=+;,.<>?{}[]";
    if(*(stringRet3(s)) != *s){    printf("stringRet3 does not pass the test");    fail++;  }
    s = "  abc35  345lmn  ";
    if(*(stringRet3(s)) != *s){    printf("stringRet3 does not pass the test");    fail++;  }

    if(fail==0){
        printf("Success!\n");
    }
    return 0;
}