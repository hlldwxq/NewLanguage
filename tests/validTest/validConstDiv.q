# constant folding

def uint1 testConstDivu128(uint128 a, uint128 b, uint128 c) {
  return a/b == c
}

def uint1 testConstDivs128(sint128 a, sint128 b, sint128 c) {
  return a/b == c
}

def uint1 testConstDivu64(uint64 a, uint64 b, uint64 c) {
  return a/b == c
}

def uint1 testConstDivs64(sint64 a, sint64 b, sint64 c) {
  return a/b == c
}

def uint1 testConstDivu32(uint32 a, uint32 b, uint32 c) {
  return a/b == c
}

def uint1 testConstDivs32(sint32 a, sint32 b, sint32 c) {
  return a/b == c
}

def uint1 testConstDivu16(uint16 a, uint16 b, uint16 c) {
  return a/b == c
}

def uint1 testConstDivs16(sint16 a, sint16 b, sint16 c) {
  return a/b == c
}

def uint1 testConstDivu8(uint8 a, uint8 b, uint8 c) {
  return a/b == c
}

def uint1 testConstDivs8(sint8 a, sint8 b, sint8 c) {
  return a/b == c
}


#==============  unsigned 128 ==================#

def uint1 divu128_1() {
  return testConstDivu128(340282366920938463463374607431768211455,340282366920938463463374607431768211455,1)
}

def uint1 divu128_2() {
  return testConstDivu128(340282366920938463463374607431768211455,1,340282366920938463463374607431768211455)
}

#==============  signed ==================#

def uint1 divs128_2() {
  return testConstDivs128(170141183460469231731687303715884105727,170141183460469231731687303715884105727,1)
}

def uint1 divs128_3() {
  return testConstDivs128(170141183460469231731687303715884105727,1,170141183460469231731687303715884105727)
}

def uint1 divs128_4() {
  return testConstDivs128(170141183460469231731687303715884105727,-1,-170141183460469231731687303715884105727)
}

def uint1 divs128_5() {
  return testConstDivs128(-170141183460469231731687303715884105728,1,-170141183460469231731687303715884105728)
}
#= min neg + 0
def uint1 divs128_6() {
  return testConstDivs128(-170141183460469231731687303715884105728,-170141183460469231731687303715884105728,1)
}
#= mid neg + mid neg
def uint1 divs128_7() {
  return testConstDivs128(-170141183460469231731687303715884105727,-1,170141183460469231731687303715884105727)
}
#= mid pos + mis pos
def uint1 divs128_8() {
  return testConstDivs128(170141183460469231731687303715884105727,-1,-170141183460469231731687303715884105727)
}

#==============  unsigned 64 ==================#

def uint1 divu64_1() {
  return testConstDivu64(18446744073709551615,18446744073709551615,1)
}

def uint1 divu64_2() {
  return testConstDivu64(18446744073709551615,1,18446744073709551615)
}

#==============  signed ==================#

def uint1 divs64_2() {
  return testConstDivs64(9223372036854775807,9223372036854775807,1)
}

def uint1 divs64_3() {
  return testConstDivs64(9223372036854775807,1,9223372036854775807)
}

def uint1 divs64_4() {
  return testConstDivs64(9223372036854775807,-1,-9223372036854775807)
}

def uint1 divs64_5() {
  return testConstDivs64(-9223372036854775808,1,-9223372036854775808)
}
#= min neg + 0
def uint1 divs64_6() {
  return testConstDivs64(-9223372036854775808,-9223372036854775808,1)
}
#= mid neg + mid neg
def uint1 divs64_7() {
  return testConstDivs64(-9223372036854775807,-1,9223372036854775807)
}
#= mid pos + mis pos
def uint1 divs64_8() {
  return testConstDivs64(9223372036854775807,-1,-9223372036854775807)
}

#==============  unsigned 32 ==================#

def uint1 divu32_1() {
  return testConstDivu32(4294967295,4294967295,1)
}

def uint1 divu32_2() {
  return testConstDivu32(4294967295,1,4294967295)
}

#==============  signed ==================#

def uint1 divs32_2() {
  return testConstDivs32(2147483647,2147483647,1)
}

def uint1 divs32_3() {
  return testConstDivs32(2147483647,1,2147483647)
}

def uint1 divs32_4() {
  return testConstDivs32(2147483647,-1,-2147483647)
}

def uint1 divs32_5() {
  return testConstDivs32(-2147483648,1,-2147483648)
}
#= min neg + 0
def uint1 divs32_6() {
  return testConstDivs32(-2147483648,-2147483648,1)
}
#= mid neg + mid neg
def uint1 divs32_7() {
  return testConstDivs32(-2147483647,-1,2147483647)
}
#= mid pos + mis pos
def uint1 divs32_8() {
  return testConstDivs32(2147483647,-1,-2147483647)
}

#==============  unsigned 16 ==================#

def uint1 divu16_1() {
  return testConstDivu16(65535,65535,1)
}

def uint1 divu16_2() {
  return testConstDivu16(65535,1,65535)
}

#==============  signed ==================#

def uint1 divs16_2() {
  return testConstDivs16(32767,32767,1)
}

def uint1 divs16_3() {
  return testConstDivs16(32767,1,32767)
}

def uint1 divs16_4() {
  return testConstDivs16(32767,-1,-32767)
}

def uint1 divs16_5() {
  return testConstDivs16(-32768,1,-32768)
}
#= min neg + 0
def uint1 divs16_6() {
  return testConstDivs16(-32768,-32768,1)
}
#= mid neg + mid neg
def uint1 divs16_7() {
  return testConstDivs16(-32767,-1,32767)
}
#= mid pos + mis pos
def uint1 divs16_8() {
  return testConstDivs16(32767,-1,-32767)
}

#==============  unsigned 8 ==================#

def uint1 divu8_1() {
  return testConstDivu8(255,255,1)
}

def uint1 divu8_2() {
  return testConstDivu8(255,1,255)
}

#==============  signed ==================#

def uint1 divs8_2() {
  return testConstDivs8(127,127,1)
}

def uint1 divs8_3() {
  return testConstDivs8(127,1,127)
}

def uint1 divs8_4() {
  return testConstDivs8(127,-1,-127)
}

def uint1 divs8_5() {
  return testConstDivs8(-128,1,-128)
}
#= min neg + 0
def uint1 divs8_6() {
  return testConstDivs8(-128,-128,1)
}
#= mid neg + mid neg
def uint1 divs8_7() {
  return testConstDivs8(-127,-1,127)
}
#= mid pos + mis pos
def uint1 divs8_8() {
  return testConstDivs8(127,-1,-127)
}
