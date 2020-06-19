# EXPECT type cannot be converted

def void ValidTest1(sint8 a, sint8 b){

}

def uint16 test(){

  uint8 arg2 = 3
  ValidTest1(arg2, 52)
  return 0
}

