# constant folding

def uint1 testConstAndu128(uint128 a, uint128 b, uint128 c) {
  return (a&b) <= c
}

def uint1 testConstAnds128(sint128 a, sint128 b, sint128 c) {
  return (a&b) <= c
}

def uint1 testConstAndu64(uint64 a, uint64 b, uint64 c) {
  return (a&b) <= c
}

def uint1 testConstAnds64(sint64 a, sint64 b, sint64 c) {
  return (a&b) <= c
}

def uint1 testConstAndu32(uint32 a, uint32 b, uint32 c) {
  return (a&b) <= c
}

def uint1 testConstAnds32(sint32 a, sint32 b, sint32 c) {
  return (a&b) <= c
}

def uint1 testConstAndu16(uint16 a, uint16 b, uint16 c) {
  return (a&b) <= c
}

def uint1 testConstAnds16(sint16 a, sint16 b, sint16 c) {
  return (a&b) <= c
}

def uint1 testConstAndu8(uint8 a, uint8 b, uint8 c) {
  return (a&b) <= c
}

def uint1 testConstAnds8(sint8 a, sint8 b, sint8 c) {
  return (a&b) <= c
}



#<=<=<=<=<=<=<=  unsigned 128 <=<=<=<=<=<=<=<=<=#
#= mid pos & mid pos
def uint1 andu128_1() {
  return testConstAndu128(170141183460469231731687303715884105727,170141183460469231731687303715884105728,170141183460469231731687303715884105727&170141183460469231731687303715884105728)
}
#= max pos & 0
def uint1 andu128_2() {
  return testConstAndu128(340282366920938463463374607431768211455,0,340282366920938463463374607431768211455&0)
}
def uint1 andu128_3() {
  return testConstAndu128(0,0,0)
}

#<=<=<=<=<=<=<=  signed <=<=<=<=<=<=<=<=<=#
#= min pos & min pos
def uint1 ands128_2() {
  return testConstAnds128(9223372036854775808,9223372036854775808,9223372036854775808 & 9223372036854775808)
}
#= max neg & max neg
def uint1 ands128_3() {
  return testConstAnds128(-9223372036854775809,-9223372036854775809,-9223372036854775809 & (-9223372036854775809))
}
#= max pos & 0
def uint1 ands128_4() {
  return testConstAnds128(170141183460469231731687303715884105727,0,170141183460469231731687303715884105727 & 0)
}
#= max pos & min neg
def uint1 ands128_5() {
  return testConstAnds128(170141183460469231731687303715884105727,-170141183460469231731687303715884105728,170141183460469231731687303715884105727& (-170141183460469231731687303715884105728))
}
#= min neg & 0
def uint1 ands128_6() {
  return testConstAnds128(0,-170141183460469231731687303715884105728,0 & -170141183460469231731687303715884105728)
}
#= mid neg & mid neg
def uint1 ands128_7(){
  return testConstAnds128(-170141183460469231731687303715884105728/2,-170141183460469231731687303715884105728/2,-170141183460469231731687303715884105728/2 & -170141183460469231731687303715884105728/2)
}
#= mid pos & mis pos
def uint1 ands128_8() {
  return testConstAnds128(170141183460469231731687303715884105728/2,170141183460469231731687303715884105726/2,170141183460469231731687303715884105728/2&170141183460469231731687303715884105726/2)
}

#<=<=<=<=<=<=<=  unsigned 64 <=<=<=<=<=<=<=<=<=#
#= mid pos & mid pos
def uint1 andu64_1() {
  return testConstAndu64(9223372036854775807,9223372036854775807,9223372036854775807 & 9223372036854775807)
}
#= max pos & 0
def uint1 andu64_2() {
  return testConstAndu64(18446744073709551615,0,18446744073709551615 & 0)
}
def uint1 andu64_3() {
  return testConstAndu64(0,0,0)
}

#<=<=<=<=<=<=<=  signed <=<=<=<=<=<=<=<=<=#
#= min pos & min pos
def uint1 ands64_2() {
  return testConstAnds64(2147483648,2147483648,2147483648 & 2147483648)
}
#= max neg & max neg
def uint1 ands64_3() {
  return testConstAnds64(-2147483649,-2147483649,-2147483649&(-2147483649))
}
#= max pos & 0
def uint1 ands64_4() {
  return testConstAnds64(9223372036854775807,0,9223372036854775807 & 0)
}
#= max pos & min neg
def uint1 ands64_5() {
  return testConstAnds64(9223372036854775807,-9223372036854775808,9223372036854775807&(-9223372036854775808))
}
#= min neg & 0
def uint1 ands64_6() {
  return testConstAnds64(0,-9223372036854775808,0&(-9223372036854775808))
}
#= mid neg & mid neg
def uint1 ands64_7() {
  return testConstAnds64(-9223372036854775808/2,-9223372036854775808/2,-9223372036854775808/2&-9223372036854775808/2)
}
#= mid pos & mis pos
def uint1 ands64_8() {
  return testConstAnds64(9223372036854775808/2,9223372036854775806/2,9223372036854775808/2&9223372036854775806/2)
}

#<=<=<=<=<=<=<=  unsigned 32 <=<=<=<=<=<=<=<=<=#
#= mid pos & mid pos
def uint1 andu32_1() {
  return testConstAndu32(2147483647,2147483647,2147483647&2147483647)
}
#= max pos & 0
def uint1 andu32_2() {
  return testConstAndu32(4294967295,0,0&4294967295)
}
def uint1 andu32_3() {
  return testConstAndu32(0,0,0)
}

#<=<=<=<=<=<=<=  signed <=<=<=<=<=<=<=<=<=#
#= min pos & min pos
def uint1 ands32_2() {
  return testConstAnds32(32768,32768,32768 & 32768)
}
#= max neg & max neg
def uint1 ands32_3() {
  return testConstAnds32(-32769,-32769,-32769 & (-32769))
}
#= max pos & 0
def uint1 ands32_4() {
  return testConstAnds32(2147483647,0,2147483647&0)
}
#= max pos & min neg
def uint1 ands32_5() {
  return testConstAnds32(2147483647,-2147483648,2147483647&-2147483648)
}
#= min neg & 0
def uint1 ands32_6() {
  return testConstAnds32(0,-2147483648,0 & -2147483648)
}
#= mid neg & mid neg
def uint1 ands32_7() {
  return testConstAnds32(-2147483648/2,-2147483648/2,-2147483648/2 & (-2147483648/2))
}
#= mid pos & mis pos
def uint1 ands32_8() {
  return testConstAnds32(2147483648/2,2147483646/2,2147483648/2 & 2147483646/2)
}

#<=<=<=<=<=<=<=  unsigned 32 <=<=<=<=<=<=<=<=<=#
#= mid pos & mid pos
def uint1 andu16_1() {
  return testConstAndu16(32767,32767,32767&32767)
}
#= max pos & 0
def uint1 andu16_2() {
  return testConstAndu16(65535,0,65535&0)
}
def uint1 andu16_3() {
  return testConstAndu16(0,0,0)
}

#<=<=<=<=<=<=<=  signed <=<=<=<=<=<=<=<=<=#
#= min pos & min pos
def uint1 ands16_2() {
  return testConstAnds16(128,128,128&128)
}
#= max neg & max neg
def uint1 ands16_3() {
  return testConstAnds16(-129,-129,-129&(-129))
}
#= max pos & 0
def uint1 ands16_4() {
  return testConstAnds16(32767,0,32767&0)
}
#= max pos & min neg
def uint1 ands16_5() {
  return testConstAnds16(32767,-32768,32767&(-32768))
}
#= min neg & 0
def uint1 ands16_6() {
  return testConstAnds16(0,-32768,0& -32768)
}
#= mid neg & mid neg
def uint1 ands16_7() {
  return testConstAnds16(-32768/2,-32768/2,-32768/2 & -32768/2)
}
#= mid pos & mis pos
def uint1 ands16_8() {
  return testConstAnds16(32768/2,32766/2,32768/2&32766/2)
}

#<=<=<=<=<=<=<=  unsigned 8 <=<=<=<=<=<=<=<=<=#
#= mid pos & mid pos
def uint1 andu8_1() {
  return testConstAndu8(127,127,127&127)
}
#= max pos & 0
def uint1 andu8_2() {
  return testConstAndu8(255,0,255&0)
}
def uint1 andu8_3() {
  return testConstAndu8(0,0,0)
}

#<=<=<=<=<=<=<=  signed <=<=<=<=<=<=<=<=<=#
#= max pos & 0
def uint1 ands8_4() {
  return testConstAnds8(127,0,127&0)
}
#= max pos & min neg
def uint1 ands8_5() {
  return testConstAnds8(127,-128,127&-128)
}
#= min neg & 0
def uint1 ands8_6() {
  return testConstAnds8(0,-128,0&-128)
}
#= mid neg & mid neg
def uint1 ands8_7() {
  return testConstAnds8(-64,-64,-64 & -64)
}
#= mid pos & mis pos
def uint1 ands8_8() {
  return testConstAnds8(64,63,64 & 63)
}