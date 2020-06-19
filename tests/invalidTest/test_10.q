# EXPECT type cannot be converted
def uint16 test(){

  sint64* p1 = new sint32[5]
  sint64* p2 = new sint128[5]
  return 0
}

