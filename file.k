sint64* t = new sint64[3]
#sint64* t1 = new uint64[3]

sint32 i1 = 32
#uint16 i2 = -9

def sint64 ValidTest1 (sint8 var1, sint64 var2){

  # ============ binary expression and int var assign ============= #
      

     # number + int variable
     var2 = 129 + var1                # the type of var1 will be changed
     var2 = 2 + var1                  # the type of constant num will be changed


     # int variable + number
     var2 = var1 - 128                # the type of var1 will be changed
     var2 = var1 - 2                  # the type of constant num will be changed


     # number + number
     var2 = 128 * 9


     # int var + int var
     var2 = var2 + var1


  # ============ int var define and init ================ # 
    
     sint32 var3 = -45
     sint32 var4 = var1

  # ============ return ================================= #
     return var3             # int var type convert
}

def sint64 ValidTest2 (){ 
   
  #=============== int var def ========================== #
     sint32 var1 = 78          # constant number type convert
     sint64 var2 = var1        # int var type convert

  #=============== call function ======================== #
     sint32 arg = 20
     ValidTest1( 6 , arg )   # constant num type convert & int var type convert
     
  #=============== return constant num ================== #
     return 6                # constant num type convert    
}

def uint16 InValidTest(){
   # ========= signed and unsigned ========== # 
     #uint32 var0 = -9     #def & constant num

     #sint32 var1 = 5      #def & int var
     #uint32 var0 = var1

     #uint64 var1          #assign & constant num
     #var1 = -6

     #sint32 var2 = 6      #assign & int var
     #uint64 var3 = var2

     #uint8 var3
     #uint32 var4 = var3 + (-4)   #binary

     
   # ========= width ======================== #
     #sint8 var4 = 127        #define & constant num
     #sint8 var4 = -129
 
     #sint16 var4 = 12        #define & int var
     #sint8 var5 = var4

     #sint64 var5 = 1         #assign & constant num
     #sint32 var6
     #var6 = var5

     #sint8 var7              #assign & int var
     #var7 = 130


   # ========= pointer ====================== #
     #sint64* p1 = new sint32[5]
     #sint64* p2 = new sint128[5]

     #uint64* p3 = new sint64[5]
     #sint64* p4 = new uint64[5]

     #sint64* p5 = new sint64*[5]
     #sint64** p5 = new sint64[5]

     #sint64** p6 = new sint64*[5]
     #p6[0] = new sint32[5]  
     #p6[0] = new sint64*[5]
     #p6[0] = new uint64[5]

  
   # ========= call ========================= #
     #ValidTest1(132, 32)
     
     #sint16 arg1 = 2
     #ValidTest1(arg1, 42)

     #uint8 arg2 = 3
     #ValidTest1(arg2, 52)

   
   # ========= arr index ==================== #
     #sint64* arr1 = new sint64[3]
     #sint64 var1 = arr1[-1]

   
   # ========= return ======================= #
     #return 65536
     
     #sint16 ret1 = 4
     #return ret1

     #uint32 ret2 = 5
     #return ret2

  return 0
}
