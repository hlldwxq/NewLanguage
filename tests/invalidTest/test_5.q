# EXPECT Error:unvalid binary calculation between signed number and unsigned number
def uint16 test(){

  uint8 var3
  uint32 var4 = var3 + (-4)   #binary
  return 0
}

