#sint16* t = new sint16 [3]
# ============ binary expression ============= #
#signed
def sint64 snumAndVarPara(sint8 var1){
     sint8* ptr = new sint8 [3]
     sint64 var2
     var2 = 128 + var1                # the type of var1 will be changed
     var2 = -128 + var2               # the type of constant num will be changed
     return var2
}

def sint64 svarAndNumPara(sint8 var1){
     sint64 var2
     var2 = var1 - 128                # the type of var1 will be changed
     var2 = var2 - (-128)             # the type of constant num will be changed
     return var2
}

def sint64 snumAndNum128(){
     return 128 * 1                   # number + number
}

def sint64 svarAndVarPara(sint64 var2){
     sint16 var1 = -32768
     var2 = var2 - var1               # int var + int var
     var2 = var1 + var2               # change the position of left and right
     return var2
}

def sint64 snumAndNumAndVarPara(sint16 var1){
     sint64 var2 = 32768 + (-32768) +var1
     return var2
}

def sint16 sHelperFunc(sint16 var){
     return var
}

def sint64 sCallFunBinPara(sint16 var1){
     sint64 var2 = sHelperFunc(var1) - var1 + sHelperFunc(var1)
     return var2
}

# unsigned
def uint32 unumAndVarPara(uint8 var1){
     uint32 var2
     var2 = 256 + var1                # the type of var1 will be changed
     var2 = var2 - 256                # the type of constant num will be changed
     return var2
}

def uint32 uvarAndNumPara(uint8 var1){
     uint32 var2
     var2 = var1 + 256                # the type of var1 will be changed
     var2 = var2 - 256                # the type of constant num will be changed
     return var2
}

def uint16 unumAndNum65535(){
     return 65535 * 1                 # number * number
}

def uint32 uvarAndVarPara(uint32 var2){
     uint16 var1 = 255
     var2 = var1 + var2               # int var + int var
     var2 = var2 - var1               # change the position of left and right
     return var2
}

def uint32 unumAndNumAndVarPara(uint16 var1){
     uint32 var2 = 65535 - 65535 +var1
     return var2
}

def uint16 uHelperFunc(uint16 var){
     return var
}

def uint32 uCallFunBinPara(uint16 var1){
     uint32 var2 = uHelperFunc(var1) - var1 + uHelperFunc(var1)
     return var2
}
# ============ int var define and init ================ # 
# signed
def sint64 sintVarDef127(){
     sint16 var1 = 127               # constant number type convert
     sint64 var2 = var1              # var type convert
     return var2
}
  
def sint32 sintVarDefBinaryPara(sint16 var2){
     sint16 var1 = 32768 - 1         # constant number
     sint32 var3 = var2 - var1      # var bop var var2 + 32767
     sint32 var4 = var3 + 32767      # var bop num var2 + 32767 - 32767
     return var4
}

def sint32 sCallFunVarDefPara(sint16 var1){
     sint32 var2 = sHelperFunc(var1)
     return var2
}

#unsigned   
def uint32 uintVarDef127(){
     uint16 var1 = 127               # constant number type convert
     uint32 var2 = var1              # var type convert
     return var2
}
  
def uint32 uintVarDefBinaryPara(uint16 var2){
     uint16 var1 = 65536 - 1
     uint32 var3 = var2 + var1 - 65535
     return var3
}

def uint32 uCallFunVarDefPara(uint16 var1){
     uint32 var2 = uHelperFunc(var1)
     return var2
}
# ============= int var assign =============================#
# signed
def sint32 sAssignNum32767(sint32 var1){
     var1 = 32767
     return var1
}

def sint32 sAssignVarPara(sint16 var1){
     sint32 var2
     var2 = var1
     return var2
}

def sint32 sAssignBinaryPara(sint32 var1){ 
     sint32 var2 
     var2 = 32768 - 1             # constant number
     var1 = var2 - var2 + var1    # var bop var
     var1 = var1 + 0              # var bop num
     return var1
}

def sint32 sCallFunAssignPara(sint16 var1){
     sint32 var2 
     var2 = sHelperFunc(var1)
     return var2
}

# unsigned
def uint32 uAssignNum65537(uint32 var1){
     var1 = 65537
     return var1
}

def uint32 uAssignVarPara(uint16 var1){
     uint32 var2
     var2 = var1
     return var2
}

def uint32 uAssignBinaryPara(uint32 var1){ 
     uint32 var2 
     var2 = 65536 - 1             # constant number
     var1 = var1 - var2 + var2    # var bop var
     var1 = var1 + 0              # var bop num
     return var1
}

def uint32 uCallFunAssignPara(uint16 var1){
     uint32 var2 
     var2 = uHelperFunc(var1)
     return var2
}

#=============== call function parameter ======================== #
def sint32 sHelperFuncArg(sint32 arg1, sint16 arg2){
     return arg1 - arg2
}

def sint32 sFunctionArg(sint16 arg1, sint8 arg2){
     return sHelperFuncArg(arg1,arg2)
}

def uint32 uHelperFuncArg(uint32 arg1, uint16 arg2){
     return arg1 - arg2
}

def uint32 uFunctionArg(uint16 arg1, uint8 arg2){
     return uHelperFuncArg(arg1,arg2)
}

#=========================== return type =============================#
#signed
def sint32 sReturnNum127(){
     return 127
}

def sint32 sReturnVarPara(sint16 var){
     return var
}

def sint32 sReturnBinPara(sint16 var){
     return 127 - 127 + var
}

def sint32 sReturnCallPara(sint16 var){
     return sHelperFunc(var)
}
#unsigned
def uint32 uReturnNum127(){
     return 127
}

def uint32 uReturnVarPara(uint16 var){
     return var
}

def uint32 uReturnBinPara(uint16 var){
     return 127 - 127 + var
}

def uint32 uReturnCallPara(uint16 var){
     return uHelperFunc(var)
}