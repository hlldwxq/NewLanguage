# constant folding

def uint1 testConstMinusu128(uint128 a, uint128 b, uint128 c) {
  return a-b == c
}

def uint1 testConstMinuss128(sint128 a, sint128 b, sint128 c) {
  return a-b == c
}

def uint1 testConstMinusu64(uint64 a, uint64 b, uint64 c) {
  return a-b == c
}

def uint1 testConstMinuss64(sint64 a, sint64 b, sint64 c) {
  return a-b == c
}

def uint1 testConstMinusu32(uint32 a, uint32 b, uint32 c) {
  return a-b == c
}

def uint1 testConstMinuss32(sint32 a, sint32 b, sint32 c) {
  return a-b == c
}

def uint1 testConstMinusu16(uint16 a, uint16 b, uint16 c) {
  return a-b == c
}

def uint1 testConstMinuss16(sint16 a, sint16 b, sint16 c) {
  return a-b == c
}

def uint1 testConstMinusu8(uint8 a, uint8 b, uint8 c) {
  return a-b == c
}

def uint1 testConstMinuss8(sint8 a, sint8 b, sint8 c) {
  return a-b == c
}



#==============  unsigned 128 ==================#
#= max pos - max pos
def uint1 minusu128_1() {
  return testConstMinusu128(340282366920938463463374607431768211455,340282366920938463463374607431768211455,0)
}
#= max pos + 0
def uint1 minusu128_2() {
  return testConstMinusu128(340282366920938463463374607431768211455,0,340282366920938463463374607431768211455)
}
def uint1 minusu128_3() {
  return testConstMinusu128(0,0,0)
}

#==============  signed ==================#
#= max pos - max pos
def uint1 minuss128_2() {
  return testConstMinuss128(170141183460469231731687303715884105727,170141183460469231731687303715884105727,0)
}
#= min neg - min neg
def uint1 minuss128_3() {
  return testConstMinuss128(-170141183460469231731687303715884105728,-170141183460469231731687303715884105728,0)
}
#= max pos -0
def uint1 minuss128_4() {
  return testConstMinuss128(170141183460469231731687303715884105727,0,170141183460469231731687303715884105727)
}
#= min neg -0
def uint1 minuss128_5() {
  return testConstMinuss128(-170141183460469231731687303715884105728,0,-170141183460469231731687303715884105728)
}
#==============  unsigned 64 ==================#
#= max pos - max pos
def uint1 minusu64_1() {
  return testConstMinusu64(18446744073709551615,18446744073709551615,0)
}
#= max pos - 0
def uint1 minusu64_2() {
  return testConstMinusu64(18446744073709551615,0,18446744073709551615)
}
def uint1 minusu64_3() {
  return testConstMinusu64(0,0,0)
}

#==============  signed ==================#
#= max pos - max pos
def uint1 minuss64_2() {
  return testConstMinuss64(9223372036854775807,9223372036854775807,0)
}
#= min neg - min neg
def uint1 minuss64_3() {
  return testConstMinuss64(-9223372036854775808,-9223372036854775808,0)
}
#= max pos + min neg
def uint1 minuss64_4() {
  return testConstMinuss64(9223372036854775807,0,9223372036854775807)
}
#= max pos + min neg
def uint1 minuss64_5() {
  return testConstMinuss64(-9223372036854775808,0,-9223372036854775808)
}
#==============  unsigned 32 ==================#
#= max pos - max pos
def uint1 minusu32_1() {
  return testConstMinusu32(4294967295,4294967295,0)
}
#= max pos - 0
def uint1 minusu32_2() {
  return testConstMinusu32(4294967295,0,4294967295)
}
def uint1 minusu32_3() {
  return testConstMinusu32(0,0,0)
}

#==============  signed ==================#
#= max pos - max pos
def uint1 minuss32_2() {
  return testConstMinuss32(2147483647,2147483647,0)
}
#= min neg - min neg
def uint1 minuss32_3() {
  return testConstMinuss32(-2147483648,-2147483648,0)
}
#= max pos + min neg
def uint1 minuss32_4() {
  return testConstMinuss32(2147483647,0,2147483647)
}
#= max pos + min neg
def uint1 minuss32_5() {
  return testConstMinuss32(-2147483648,0,-2147483648)
}

#==============  unsigned 16 ==================#
#= max pos - max pos
def uint1 minusu16_1() {
  return testConstMinusu16(65535,65535,0)
}
#= max pos - 0
def uint1 minusu16_2() {
  return testConstMinusu16(65535,0,65535)
}
def uint1 minusu16_3() {
  return testConstMinusu16(0,0,0)
}

#==============  signed ==================#
#= max pos - max pos
def uint1 minuss16_2() {
  return testConstMinuss16(32767,32767,0)
}
#= min neg - min neg
def uint1 minuss16_3() {
  return testConstMinuss16(-32768,-32768,0)
}
#= max pos + min neg
def uint1 minuss16_4() {
  return testConstMinuss16(32767,0,32767)
}
#= max pos + min neg
def uint1 minuss16_5() {
  return testConstMinuss16(-32768,0,-32768)
}

#==============  unsigned 8 ==================#
#= max pos - max pos
def uint1 minusu8_1() {
  return testConstMinusu8(255,255,0)
}
#= max pos - 0
def uint1 minusu8_2() {
  return testConstMinusu8(255,0,255)
}
def uint1 minusu8_3() {
  return testConstMinusu8(0,0,0)
}

#==============  signed ==================#
#= max pos - max pos
def uint1 minuss8_2() {
  return testConstMinuss8(127,127,0)
}
#= min neg - min neg
def uint1 minuss8_3() {
  return testConstMinuss8(-128,-128,0)
}
#= max pos + min neg
def uint1 minuss8_4() {
  return testConstMinuss8(127,0,127)
}
#= max pos + min neg
def uint1 minuss8_5() {
  return testConstMinuss8(-128,0,-128)
}
