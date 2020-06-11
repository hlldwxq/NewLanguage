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

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {

   // ============ binary expression ============= //
   // signed
   int var1 = 127; //127~-128
   printf("num bop var test: %d\n",snumAndVarPara(var1) == var1); // 127 ~ -128
   
   var1 = -128;
   printf("var bop num test: %d\n",svarAndNumPara(var1) == var1); // 127 ~ -128
   
   printf("num bop num test: %d\n",snumAndNum128()==128);

   long long var2 = 9223372036854775807; //9223372036854775807 ~ -9223372036854775808
   printf("var bop var test: %d\n",svarAndVarPara(var2)==var2); //9223372036854775807 ~ -9223372036854775808

   int var3 = -32768;
   printf("num bop num bop var test: %d\n",snumAndNumAndVarPara(var3)==var3); //32767 ~ -32768

   var3 = 32767;
   printf("callFunc as binary operand test: %d\n",sCallFunBinPara(var3)==var3); //32767 ~ -32768 hhhhhhh

   // unsigned
   unsigned int var4 = 255;
   printf("unsigned num bop var test: %d\n",unumAndVarPara(var4) == var4); //255 ~ 0
   printf("unsigned var bop num test: %d\n",uvarAndNumPara(var4) == var4); //255 ~ 0

   printf("unsigned num bop var test: %d\n",unumAndNum65535()==65535);

   unsigned long var5 = 4294967295;
   printf("unsigned var bop var test: %d\n",uvarAndVarPara(var5) == var5); //4294967295 ~ 0

   unsigned int var6 = 0;
   printf("unsigned num bop num bop var test: %d\n",unumAndNumAndVarPara(var6)==var6); //65535 ~ 0

   var6 = 65535;
   printf("unsigned callFunc as binary operand test: %d\n",uCallFunBinPara(var6)==var6); //65535 ~ 0 

   // ============ int var define and init ================ // 
   // signed
   printf("int var def, init with constant number or other var: %d\n",sintVarDef127()==127);
   
   int var7 = 32767;
   printf("int var def, init with binary expression: %d\n",sintVarDefBinaryPara(var7)==var7); //32767 ~ -32768
   
   var7 = -32768; 
   printf("int var def, init with callFunction: %d\n",sCallFunVarDefPara(var7)==var7); //32767 ~ -32768 hhhhhh

   // unsigned   
   printf("unsigned int var def, init with constant number or other var: %d\n",uintVarDef127()==127);
   
   unsigned int var8 = 0;
   printf("unsigned int var def, init with binary expression: %d\n",uintVarDefBinaryPara(var8)==var8); //65535 ~ 0

   var8 = 65535;
   printf("unsigned int var def, init with callFunction: %d\n",uCallFunVarDefPara(var8)==var8); //65535 ~ 0  hhhhhh

   // ============= int var assign ======================== //
   // signed
   long var9 = 2147483647;
   printf("assign, right value is constant number: %d\n",sAssignNum32767(var9)==32767); //2147483647 ~ -2147483648
   
   int var10 = 32767;
   printf("assign, right value is other variable: %d\n",sAssignVarPara(var10) == var10); //32767 ~ -32768

   var9 = 2147483647;
   printf("assign, right value is binary expression: %d\n",sAssignBinaryPara(var9)==var9); //2147483647 ~ -2147483648
   
   var10 = 0;
   printf("assign, right value is call Function: %d\n",sCallFunAssignPara(var10)==var10); //65535 ~ 0 
   // unsigned
   unsigned long var11 = 4294967295;
   printf("unsigned assign, right value is constant number: %d\n",uAssignNum65537(var11)==65537); //4294967295 ~ 0

   unsigned int var12 = 0;
   printf("unsigned assign, right value is other variable: %d\n",uAssignVarPara(var12)==var12); //65535 ~ 0

   var11 = 0;
   printf("unsigned assign, right value is binary expression: %d\n",uAssignBinaryPara(var11)==var11); //4294967295 ~ 0

   var12 = 65535;
   printf("unsigned assign, right value is call Function: %d\n",uCallFunAssignPara(var12)==var12); //65535 ~ 0 
   
   // ==================== function args =================== //
   int var13 = 32767;
   int var14 = 127;
   printf("arguments test: %d\n",sFunctionArg(var13,var14)==var13 - var14); //arg1 32767 ~ -32768   arg2 127 ~ -128
   
   unsigned int var15 = 65535;
   unsigned int var16 = 255;
   printf("unsigned arguments test: %d\n",uFunctionArg(var15,var16)==var15-var16); //arg1 65535 ~ 0   arg2 255~0

   //=========================== return type =============================#
   //signed
   printf("constant num as return value: %d\n",sReturnNum127()==127);
   int var17 = 32767;
   printf("variable as return value: %d\n",sReturnVarPara(var17)==var17);//arg1 32767 ~ -32768
   var17 = -32768;  //
   printf("binary expression as return value: %d\n",sReturnBinPara(var17)==var17);//arg1 32767 ~ -32768
   var17 = 32767;
   printf("call function as return value: %d\n",sReturnCallPara(var17)==var17);//arg1 32767 ~ -32768
   
   //unsigned
   printf("unsigned constant num as return value: %d\n",uReturnNum127()==127);
   unsigned int var18 = 65535; 
   printf("unsigned variable as return value: %d\n",uReturnVarPara(var18)==var18);//arg1 32767 ~ -32768
   var18 = 0;
   printf("unsigned binary expression as return value: %d\n",uReturnBinPara(var18)==var18);//arg1 32767 ~ -32768
   var18 = 65535;
   printf("unsigned call function as return value: %d\n",uReturnCallPara(var18)==var18);//arg1 32767 ~ -32768

}





