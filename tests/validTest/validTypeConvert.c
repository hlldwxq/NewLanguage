#include <stdio.h>


// ============ binary expression ============= //
long long snumAndVarPara(int); // 127 ~ -128
long long svarAndNumPara(int); // 127 ~ -128
long long snumAndNum128();
long long svarAndVarPara(long long); //9223372036854775807 ~ -9223372036854775808
long long snumAndNumAndVarPara(int); //32767 ~ -32768
long long sCallFunBinPara(int); //32767 ~ -32768

unsigned long unumAndVarPara(unsigned int); //255 ~ 0
unsigned long uvarAndNumPara(unsigned int); //255 ~ 0
unsigned long unumAndNum65535();
unsigned long uvarAndVarPara(unsigned long); //4294967295 ~ 0
unsigned long unumAndNumAndVarPara(unsigned int); //65535 ~ 0
unsigned long uCallFunBinPara(unsigned int); //65535 ~ 0 

// ============ int var define and init ================ // 
// signed
long long sintVarDef127();
long long sintVarDefBinaryPara(int); //32767 ~ -32768
int sCallFunVarDefPara(int); //32767 ~ -32768

// unsigned   
unsigned long uintVarDef127();
unsigned long uintVarDefBinaryPara(unsigned int); //65535 ~ 0
unsigned long uCallFunVarDefPara(unsigned int); //65535 ~ 0  

// ============= int var assign ======================== //
// signed
long sAssignNum32767(long); //2147483647 ~ -2147483648
long sAssignVarPara(int); //32767 ~ -32768
long sAssignBinaryPara(long); //2147483647 ~ -2147483648
long sCallFunAssignPara(int); //32767 ~ -32768 

// unsigned
unsigned long uAssignNum65537(unsigned long); //4294967295 ~ 0
unsigned long uAssignVarPara(unsigned int); //65535 ~ 0
unsigned long uAssignBinaryPara(unsigned long); //4294967295 ~ 0
unsigned long uCallFunAssignPara(unsigned int); //65535 ~ 0 

// ==================== function args =================== //
long sFunctionArg(int arg1, int arg2); //arg1 32767 ~ -32768   arg2 127 ~ -128
unsigned long uFunctionArg(unsigned int arg1, unsigned int arg2); //arg1 65535 ~ 0   arg2 255~0

//=========================== return type =============================#
//signed
long sReturnNum127();
long sReturnVarPara(int);//arg1 32767 ~ -32768
int sReturnBinPara(int);//arg1 32767 ~ -32768
long sReturnCallPara(int);//arg1 32767 ~ -32768
//unsigned
unsigned long uReturnNum127();
unsigned long uReturnVarPara(unsigned int var);//arg1 65535 ~ 0
unsigned long uReturnBinPara(unsigned int var);//arg1 65535 ~ 0
unsigned long uReturnCallPara(unsigned int var);//arg1 65535 ~ 0


int failed = 0;

void check(char const *msg, int res) {
 // printf("%s: %d\n",msg, res);
  if (res != 1) ++failed;
}



int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {

   // ============ binary expression ============= //
   // signed
   int var1 = 127; //127~-128
   check("num bop var test",snumAndVarPara(var1) == var1); // 127 ~ -128
   
   var1 = -128;
   check("var bop num test",svarAndNumPara(var1) == var1); // 127 ~ -128
   
   check("num bop num test",snumAndNum128()==128);

   long long var2 = 854775808; //9223372036854775807 ~ -9223372036854775808
   check("var bop var test",svarAndVarPara(var2)==var2); //9223372036854775807 ~ -9223372036854775808

   int var3 = -32768;
   check("num bop num bop var test",snumAndNumAndVarPara(var3)==var3); //32767 ~ -32768

   var3 = 32767;
   check("callFunc as binary operand test",sCallFunBinPara(var3)==var3); //32767 ~ -32768 hhhhhhh

   // unsigned
   unsigned int var4 = 255;
   check("unsigned num bop var test",unumAndVarPara(var4) == var4); //255 ~ 0
   check("unsigned var bop num test",uvarAndNumPara(var4) == var4); //255 ~ 0

   check("unsigned num bop var test",unumAndNum65535()==65535);

   unsigned long var5 = 429496;
   check("unsigned var bop var test",uvarAndVarPara(var5) == var5); //4294967295 ~ 0

   unsigned int var6 = 0;
   check("unsigned num bop num bop var test",unumAndNumAndVarPara(var6)==var6); //65535 ~ 0

   var6 = 655;
   check("unsigned callFunc as binary operand test",uCallFunBinPara(var6)==var6); //65535 ~ 0

   // ============ int var define and init ================ // 
   // signed
   check("int var def, init with constant number or other var",sintVarDef127()==127);
   
   int var7 = 327;
   check("int var def, init with binary expression",sintVarDefBinaryPara(var7)==var7); //32767 ~ -32768
   
   var7 = -327; 
   check("int var def, init with callFunction",sCallFunVarDefPara(var7)==var7); //32767 ~ -32768 hhhhhh

   // unsigned   
   check("unsigned int var def, init with constant number or other var",uintVarDef127()==127);
   
   unsigned int var8 = 0;
   check("unsigned int var def, init with binary expression",uintVarDefBinaryPara(var8)==var8); //65535 ~ 0

   var8 = 6553;
   check("unsigned int var def, init with callFunction",uCallFunVarDefPara(var8)==var8); //65535 ~ 0  hhhhhh

   // ============= int var assign ======================== //
   // signed
   long var9 = 21474836;
   check("assign, right value is constant number",sAssignNum32767(var9)==32767); //2147483647 ~ -2147483648
   
   int var10 = 327;
   check("assign, right value is other variable",sAssignVarPara(var10) == var10); //32767 ~ -32768

   var9 = 21474837;
   check("assign, right value is binary expression",sAssignBinaryPara(var9)==var9); //2147483647 ~ -2147483648
   
   var10 = 0;
   check("assign, right value is call Function",sCallFunAssignPara(var10)==var10); //65535 ~ 0
   // unsigned
   unsigned long var11 = 42947295;
   check("unsigned assign, right value is constant number",uAssignNum65537(var11)==65537); //4294967295 ~ 0

   unsigned int var12 = 0;
   check("unsigned assign, right value is other variable",uAssignVarPara(var12)==var12); //65535 ~ 0

   var11 = 0;
   check("unsigned assign, right value is binary expression",uAssignBinaryPara(var11)==var11); //4294967295 ~ 0

   var12 = 635;
   check("unsigned assign, right value is call Function",uCallFunAssignPara(var12)==var12); //65535 ~ 0
   
   // ==================== function args =================== //
   int var13 = 367;
   int var14 = 17;
   check("arguments test",sFunctionArg(var13,var14)==var13 - var14); //arg1 32767 ~ -32768   arg2 127 ~ -128
   
   unsigned int var15 = 655;
   unsigned int var16 = 25;
   check("unsigned arguments test",uFunctionArg(var15,var16)==var15-var16); //arg1 65535 ~ 0   arg2 255~0

   //=========================== return type =============================#
   //signed
   check("constant num as return value",sReturnNum127()==127);
   int var17 = 327;
   check("variable as return value",sReturnVarPara(var17)==var17);//arg1 32767 ~ -32768
   var17 = -328;  //
   check("binary expression as return value",sReturnBinPara(var17)==var17);//arg1 32767 ~ -32768
   var17 = 3277;
   check("call function as return value",sReturnCallPara(var17)==var17);//arg1 32767 ~ -32768
   
   //unsigned
   check("unsigned constant num as return value",uReturnNum127()==127);
   unsigned int var18 = 535; 
   check("unsigned variable as return value",uReturnVarPara(var18)==var18);//arg1 32767 ~ -32768
   var18 = 0;
   check("unsigned binary expression as return value",uReturnBinPara(var18)==var18);//arg1 32767 ~ -32768
   var18 = 655;
   check("unsigned call function as return value",uReturnCallPara(var18)==var18);//arg1 32767 ~ -32768


    if (failed) {
      printf("ERROR: %d tests failed\n",failed);
      return 1;
    } else {
      printf("Success!\n");
      return 0;
    }

}





