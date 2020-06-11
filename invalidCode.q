def uint16 InValidTest(){
   # ========= signed and unsigned ========== # 
     uint32 var0 = -9     #def & constant num

     #sint32 var1 = 5      #def & int var
     #uint32 var0 = var1

     #uint64 var1          #assign & constant num
     #var1 = -6

     #sint32 var2 = 6      #assign & int var
     #uint64 var3 = var2

     #uint8 var3
     #uint32 var4 = var3 + (-4)   #binary

     
   # ========= width ======================== #
     #sint8 var4 = 128   #define & constant num
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