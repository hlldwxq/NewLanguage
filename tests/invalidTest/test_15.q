# EXPECT Error:type cannot be converted

def void ValidTest1(uint8 a, sint8 b){

}

def uint16 test(){

  sint16 arg1 = 2
  ValidTest1(arg1, 42)
  return 0
}

