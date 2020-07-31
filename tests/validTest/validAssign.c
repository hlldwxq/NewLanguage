#include <stdio.h>
// ================= bool ================= #
long long boolAssignVar(long long a);
long long boolAssignTrue(long long a);
long long boolAssignFalse(long long a);
long long boolAssignBin(long long a);
long long boolAssignUnary(long long a);
long long boolAssignArrIndex(long long a);
long long boolAssignCall(long long a);
// ================== ptr ================= #
long long PtrAssignVar(long long a);
long long PtrAssignNew(long long a);
long long PtrAssignArrIndex(long long a);
long long PtrAssignCall(long long a);
// =================== long long =================== #
long long intAssignVar(long long a);
long long intAssignNum();
long long intAssignArrIndex(long long a);
long long intAssignCall(long long a);
long long intAssignBinary(long long a);

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {

    long long fail = 0;
    long long a = 10;
    //printf("positive: \n");
    // ================= bool ================= #
    if(a != boolAssignVar(a)){ printf("boolAssignVar does not pass the test\n"); fail++;}
    if(a != boolAssignTrue(a)){ printf("boolAssignTrue does not pass the test\n");  fail++;}
    if(a != boolAssignFalse(a)){ printf("boolAssignFalse does not pass the test\n");  fail++;}
    if(a != boolAssignBin(a)){ printf("boolAssignBin does not pass the test\n");  fail++;}
    if(a != boolAssignUnary(a)){ printf("boolAssignUnary does not pass the test\n");  fail++;}
    if(a != boolAssignArrIndex(a)){ printf("boolAssignArrIndex does not pass the test\n");  fail++;}
    if(a != boolAssignCall(a)){ printf("boolAssignCall does not pass the test\n");  fail++;}
    // ================== ptr ================= #
    if(a != PtrAssignVar(a)){ printf("PtrAssignVar does not pass the test\n");  fail++;}
    if(a != PtrAssignNew(a)){ printf("PtrAssignNew does not pass the test\n");  fail++;}
    if(a != PtrAssignArrIndex(a)){ printf("PtrAssignArrIndex does not pass the test\n");  fail++;}
    if(a != PtrAssignCall(a)){ printf("PtrAssignCall does not pass the test\n");  fail++;}
    // =================== long long =================== #
    if(a != intAssignVar(a)){ printf("intAssignVar does not pass the test\n");  fail++;}
    if(0 != intAssignNum()){ printf("intAssignNum does not pass the test\n");  fail++;}
    if(a != intAssignArrIndex(a)){ printf("intAssignArrIndex does not pass the test\n");  fail++;}
    if(a != intAssignCall(a)){ printf("intAssignCall does not pass the test\n");  fail++;}
    if(a != intAssignBinary(a)){ printf("intAssignBinary does not pass the test\n");  fail++;}


    a = -10;
    //printf("negative: \n");
    // ================= bool ================= #
    if(a != boolAssignVar(a)){ printf("boolAssignVar does not pass the test\n");  fail++;}
    if(a != boolAssignTrue(a)){ printf("boolAssignTrue does not pass the test\n");  fail++;}
    if(a != boolAssignFalse(a)){ printf("boolAssignFalse does not pass the test\n");  fail++;}
    if(a != boolAssignBin(a)){ printf("boolAssignBin does not pass the test\n");  fail++;}
    if(a != boolAssignUnary(a)){ printf("boolAssignUnary does not pass the test\n");  fail++;}
    if(a != boolAssignArrIndex(a)){ printf("boolAssignArrIndex does not pass the test\n");  fail++;}
    if(a != boolAssignCall(a)){ printf("boolAssignCall does not pass the test\n");  fail++;}
    // ================== ptr ================= #
    if(a != PtrAssignVar(a)){ printf("PtrAssignVar does not pass the test\n");  fail++;}
    if(a != PtrAssignNew(a)){ printf("PtrAssignNew does not pass the test\n");  fail++;}
    if(a != PtrAssignArrIndex(a)){ printf("PtrAssignArrIndex does not pass the test\n");  fail++;}
    if(a != PtrAssignCall(a)){ printf("PtrAssignCall does not pass the test\n");  fail++;}
    // =================== long long =================== #
    if(a != intAssignVar(a)){ printf("intAssignVar does not pass the test\n");  fail++;}
    if(0 != intAssignNum()){ printf("intAssignNum does not pass the test\n");  fail++;}
    if(a != intAssignArrIndex(a)){ printf("intAssignArrIndex does not pass the test\n");  fail++;}
    if(a != intAssignCall(a)){ printf("intAssignCall does not pass the test\n");  fail++;}
    if(a != intAssignBinary(a)){ printf("intAssignBinary does not pass the test\n");  fail++;}
    
    if(fail == 0){
        printf("Success!\n");
    }
    return 0;
}