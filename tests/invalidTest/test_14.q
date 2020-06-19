# EXPECT Error:type cannot be converted
def uint16 ValidTest1(sint8 a, uint8 b) { return 0 }

def uint16 test(){

  ValidTest1(132, 32)
  return 0
}

