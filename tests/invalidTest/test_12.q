# EXPECT Error:type cannot be converted
def uint16 test(){

  sint64* p5 = new sint64*[5]
  sint64** p5 = new sint64[5]
  return 0
}

