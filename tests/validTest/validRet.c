#include <stdio.h>
//return a
// ================= bool ================= #
long long retBoolVar(long long a);
long long retBoolTrue(long long a);
long long retBoolFalse(long long a);
long long retBoolBin(long long a);
long long retBoolUnary(long long a);
long long retBoolArrIndex(long long a);
long long retBoolCall(long long a);
// ================== ptr ================= #
long long retPtrVar(long long a);
long long retPtrNew(long long a);
long long retPtrArrIndex(long long a);
long long retPtrCall(long long a);
// =================== long long =================== #
long long retIntVar(long long a);
long long retIntCall(long long a);
long long retIntArrayIndex(long long a);
long long retIntNum();//# ret 0
long long retIntBinary(long long a,long long b);//# ret a+b

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {
    long long fail =0 ;
    long long a = 10;
    long long b = 5;
    //return a
// ================= bool ================= #
 //   printf("positive: \n");
    if(a != retBoolVar(a)){ printf("retBoolVar does not pass the test\n"); fail++;}
    if(a != retBoolTrue(a)){ printf("retBoolTrue does not pass the test\n");  fail++;}
    if(a != retBoolFalse(a)){ printf("retBoolFalse does not pass the test\n");  fail++;}
    if(a != retBoolBin(a)){ printf("retBoolBin does not pass the test\n");  fail++;}
    if(a != retBoolUnary(a)){ printf("retBoolUnary does not pass the test\n");  fail++;}
    if(a != retBoolArrIndex(a)){ printf("retBoolArrIndex does not pass the test\n");  fail++;}
    if(a != retBoolCall(a)){ printf("retBoolCall does not pass the test\n");  fail++;}
// ================== ptr ================= #
    if(a != retPtrVar(a)){ printf("retPtrVar does not pass the test\n");  fail++;}
    if(a != retPtrNew(a)){ printf("retPtrNew does not pass the test\n");  fail++;}
    if(a != retPtrArrIndex(a)){ printf("retPtrArrIndex does not pass the test\n");  fail++;}
    if(a != retPtrCall(a)){ printf("retPtrCall does not pass the test\n");  fail++;}
// =================== long long =================== #
    if(a != retIntVar(a)){ printf("retIntVar does not pass the test\n");  fail++;}
    if(a != retIntCall(a)){ printf("retIntCall does not pass the test\n");  fail++;}
    if(a != retIntArrayIndex(a)){ printf("retIntArrayIndex does not pass the test\n");  fail++;}
    if(0 != retIntNum()){ printf("retIntNum does not pass the test\n");  fail++;}//# ret 0
    if(a+b != retIntBinary(a,b)){ printf("retIntBinary does not pass the test\n");  fail++;}//# ret a+b


   // fail = 0 ;
    a = -10;
    b = -5;
    //return a
// ================= bool ================= #
  //  printf("negative: \n");
    if(a != retBoolVar(a)){ printf("retBoolVar does not pass the test\n"); fail++;}
    if(a != retBoolTrue(a)){ printf("retBoolTrue does not pass the test\n");  fail++;}
    if(a != retBoolFalse(a)){ printf("retBoolFalse does not pass the test\n");  fail++;}
    if(a != retBoolBin(a)){ printf("retBoolBin does not pass the test\n");  fail++;}
    if(a != retBoolUnary(a)){ printf("retBoolUnary does not pass the test\n");  fail++;}
    if(a != retBoolArrIndex(a)){ printf("retBoolArrIndex does not pass the test\n");  fail++;}
    if(a != retBoolCall(a)){ printf("retBoolCall does not pass the test\n");  fail++;}
// ================== ptr ================= #
    if(a != retPtrVar(a)){ printf("retPtrVar does not pass the test\n");  fail++;}
    if(a != retPtrNew(a)){ printf("retPtrNew does not pass the test\n");  fail++;}
    if(a != retPtrArrIndex(a)){ printf("retPtrArrIndex does not pass the test\n");  fail++;}
    if(a != retPtrCall(a)){ printf("retPtrCall does not pass the test\n");  fail++;}
// =================== long long =================== #
    if(a != retIntVar(a)){ printf("retIntVar does not pass the test\n");  fail++;}
    if(a != retIntCall(a)){ printf("retIntCall does not pass the test\n");  fail++;}
    if(a != retIntArrayIndex(a)){ printf("retIntArrayIndex does not pass the test\n");  fail++;}
    if(0 != retIntNum()){ printf("retIntNum does not pass the test\n");  fail++;}//# ret 0
    if(a+b != retIntBinary(a,b)){ printf("retIntBinary does not pass the test\n");  fail++;}//# ret a+b
    if(fail==0){
        printf("Success!\n");
    }
    return 0;
}