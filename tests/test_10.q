# EXPECT Error:.* types.*need to be the same
def uint16 test(){

  sint64* p1 = new sint32[5]
  sint64* p2 = new sint128[5]
  return 0
}

