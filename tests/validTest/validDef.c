#include <stdio.h>
// ================= bool ================= #
long long boolDefVar(long long a);
long long boolDefTrue(long long a);
long long boolDefFalse(long long a);
long long boolDefBin(long long a);
long long boolDefUnary(long long a);
long long boolDefArrIndex(long long a);
long long boolDefCall(long long a);
// ================== ptr ================= #
long long PtrDefVar(long long a);
long long PtrDefNew(long long a);
long long PtrDefArrIndex(long long a);
long long PtrDefCall(long long a);
// =================== long long =================== #
long long intDefVar(long long a);
long long intDefNum();
long long intDefArrIndex(long long a);
long long intDefCall(long long a);
long long intDefBinary(long long a);

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {
    long long fail = 0;
    long long a = 10;
  //  printf("positive:\n");
    // ================= bool ================= #
    if(a != boolDefVar(a)){ printf("boolDefVar does not pass the test\n"); fail++;}
    if(a != boolDefTrue(a)){ printf("boolDefTrue does not pass the test\n");  fail++;}
    if(a != boolDefFalse(a)){ printf("boolDefFalse does not pass the test\n");  fail++;}
    if(a != boolDefBin(a)){ printf("boolDefBin does not pass the test\n");  fail++;}
    if(a != boolDefUnary(a)){ printf("boolDefUnary does not pass the test\n");  fail++;}
    if(a != boolDefArrIndex(a)){ printf("boolDefArrIndex does not pass the test\n");  fail++;}
    if(a != boolDefCall(a)){ printf("boolDefCall does not pass the test\n");  fail++;}
    // ================== ptr ================= #
    if(a != PtrDefVar(a)){ printf("PtrDefVar does not pass the test\n");  fail++;}
    if(a != PtrDefNew(a)){ printf("PtrDefNew does not pass the test\n"); fail++;}
    if(a != PtrDefArrIndex(a)){ printf("PtrDefArrIndex does not pass the test\n"); fail++;}
    if(a != PtrDefCall(a)){ printf("PtrDefCall does not pass the test\n"); fail++;}
    // =================== long long =================== #
    if(a != intDefVar(a)){ printf("intDefVar does not pass the test\n"); fail++;}
    if(0 != intDefNum()){ printf("intDefNum does not pass the test\n"); fail++;}
    if(a != intDefArrIndex(a)){ printf("intDefArrIndex does not pass the test\n"); fail++;}
    if(a != intDefCall(a)){ printf("intDefCall does not pass the test\n"); fail++;}
    if(a != intDefBinary(a)){ printf("intDefBinary does not pass the test\n"); fail++;}


    //fail = 0;
    a = -10;
   // printf("negative:\n");
    // ================= bool ================= #
    if(a != boolDefVar(a)){ printf("boolDefVar does not pass the test\n"); fail++;}
    if(a != boolDefTrue(a)){ printf("boolDefTrue does not pass the test\n"); fail++;}
    if(a != boolDefFalse(a)){ printf("boolDefFalse does not pass the test\n"); fail++;}
    if(a != boolDefBin(a)){ printf("boolDefBin does not pass the test\n"); fail++;}
    if(a != boolDefUnary(a)){ printf("boolDefUnary does not pass the test\n"); fail++;}
    if(a != boolDefArrIndex(a)){ printf("boolDefArrIndex does not pass the test\n"); fail++;}
    if(a != boolDefCall(a)){ printf("boolDefCall does not pass the test\n"); fail++;}
    // ================== ptr ================= #
    if(a != PtrDefVar(a)){ printf("PtrDefVar does not pass the test\n"); fail++;}
    if(a != PtrDefNew(a)){ printf("PtrDefNew does not pass the test\n"); fail++;}
    if(a != PtrDefArrIndex(a)){ printf("PtrDefArrIndex does not pass the test\n"); fail++;}
    if(a != PtrDefCall(a)){ printf("PtrDefCall does not pass the test\n");fail++; }
    // =================== long long =================== #
    if(a != intDefVar(a)){ printf("intDefVar does not pass the test\n");fail++; }
    if(0 != intDefNum()){ printf("intDefNum does not pass the test\n"); fail++;}
    if(a != intDefArrIndex(a)){ printf("intDefArrIndex does not pass the test\n"); fail++;}
    if(a != intDefCall(a)){ printf("intDefCall does not pass the test\n"); fail++;}
    if(a != intDefBinary(a)){ printf("intDefBinary does not pass the test\n"); fail++;}
    if(fail == 0){
        printf("Success!\n");
    }
    return 0;
}