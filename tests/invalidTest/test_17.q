# EXPECT Error:the index of array cannot be negative
def uint16 test(){

  sint64* arr1 = new sint64[3]
  sint64 var1 = arr1[-1]
  return 0
}

