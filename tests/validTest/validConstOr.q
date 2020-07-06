# constant folding

def uint1 testConstOru128(uint128 a, uint128 b, uint128 c) {
  return (a|b) == c
}

def uint1 testConstOrs128(sint128 a, sint128 b, sint128 c) {
  return (a|b) == c
}

def uint1 testConstOru64(uint64 a, uint64 b, uint64 c) {
  return (a|b) == c
}

def uint1 testConstOrs64(sint64 a, sint64 b, sint64 c) {
  return (a|b) == c
}

def uint1 testConstOru32(uint32 a, uint32 b, uint32 c) {
  return (a|b) == c
}

def uint1 testConstOrs32(sint32 a, sint32 b, sint32 c) {
  return (a|b) == c
}

def uint1 testConstOru16(uint16 a, uint16 b, uint16 c) {
  return (a|b) == c
}

def uint1 testConstOrs16(sint16 a, sint16 b, sint16 c) {
  return (a|b) == c
}

def uint1 testConstOru8(uint8 a, uint8 b, uint8 c) {
  return (a|b) == c
}

def uint1 testConstOrs8(sint8 a, sint8 b, sint8 c) {
  return (a|b) == c
}



#==============  unsigned 128 ==================#
#= mid pos | mid pos
def uint1 oru128_1() {
  return testConstOru128(170141183460469231731687303715884105727,170141183460469231731687303715884105728,170141183460469231731687303715884105727|170141183460469231731687303715884105728)
}
#= max pos | 0
def uint1 oru128_2() {
  return testConstOru128(340282366920938463463374607431768211455,0,340282366920938463463374607431768211455|0)
}
def uint1 oru128_3() {
  return testConstOru128(0,0,0)
}

#==============  signed ==================#
#= min pos | min pos
def uint1 ors128_2() {
  return testConstOrs128(9223372036854775808,9223372036854775808,9223372036854775808 | 9223372036854775808)
}
#= max neg | max neg
def uint1 ors128_3() {
  return testConstOrs128(-9223372036854775809,-9223372036854775809,-9223372036854775809 | (-9223372036854775809))
}
#= max pos | 0
def uint1 ors128_4() {
  return testConstOrs128(170141183460469231731687303715884105727,0,170141183460469231731687303715884105727 | 0)
}
#= max pos | min neg
def uint1 ors128_5() {
  return testConstOrs128(170141183460469231731687303715884105727,-170141183460469231731687303715884105728,170141183460469231731687303715884105727| (-170141183460469231731687303715884105728))
}
#= min neg | 0
def uint1 ors128_6() {
  return testConstOrs128(0,-170141183460469231731687303715884105728,0 | -170141183460469231731687303715884105728)
}
#= mid neg | mid neg
def uint1 ors128_7(){
  return testConstOrs128(-170141183460469231731687303715884105728/2,-170141183460469231731687303715884105728/2,-170141183460469231731687303715884105728/2 | -170141183460469231731687303715884105728/2)
}
#= mid pos | mis pos
def uint1 ors128_8() {
  return testConstOrs128(170141183460469231731687303715884105728/2,170141183460469231731687303715884105726/2,170141183460469231731687303715884105728/2|170141183460469231731687303715884105726/2)
}

#==============  unsigned 64 ==================#
#= mid pos | mid pos
def uint1 oru64_1() {
  return testConstOru64(9223372036854775807,9223372036854775807,9223372036854775807 | 9223372036854775807)
}
#= max pos | 0
def uint1 oru64_2() {
  return testConstOru64(18446744073709551615,0,18446744073709551615 | 0)
}
def uint1 oru64_3() {
  return testConstOru64(0,0,0)
}

#==============  signed ==================#
#= min pos | min pos
def uint1 ors64_2() {
  return testConstOrs64(2147483648,2147483648,2147483648 | 2147483648)
}
#= max neg | max neg
def uint1 ors64_3() {
  return testConstOrs64(-2147483649,-2147483649,-2147483649|(-2147483649))
}
#= max pos | 0
def uint1 ors64_4() {
  return testConstOrs64(9223372036854775807,0,9223372036854775807 | 0)
}
#= max pos | min neg
def uint1 ors64_5() {
  return testConstOrs64(9223372036854775807,-9223372036854775808,9223372036854775807|(-9223372036854775808))
}
#= min neg | 0
def uint1 ors64_6() {
  return testConstOrs64(0,-9223372036854775808,0|(-9223372036854775808))
}
#= mid neg | mid neg
def uint1 ors64_7() {
  return testConstOrs64(-9223372036854775808/2,-9223372036854775808/2,-9223372036854775808/2|-9223372036854775808/2)
}
#= mid pos | mis pos
def uint1 ors64_8() {
  return testConstOrs64(9223372036854775808/2,9223372036854775806/2,9223372036854775808/2|9223372036854775806/2)
}

#==============  unsigned 32 ==================#
#= mid pos | mid pos
def uint1 oru32_1() {
  return testConstOru32(2147483647,2147483647,2147483647|2147483647)
}
#= max pos | 0
def uint1 oru32_2() {
  return testConstOru32(4294967295,0,0|4294967295)
}
def uint1 oru32_3() {
  return testConstOru32(0,0,0)
}

#==============  signed ==================#
#= min pos | min pos
def uint1 ors32_2() {
  return testConstOrs32(32768,32768,32768 | 32768)
}
#= max neg | max neg
def uint1 ors32_3() {
  return testConstOrs32(-32769,-32769,-32769 | (-32769))
}
#= max pos | 0
def uint1 ors32_4() {
  return testConstOrs32(2147483647,0,2147483647|0)
}
#= max pos | min neg
def uint1 ors32_5() {
  return testConstOrs32(2147483647,-2147483648,2147483647|-2147483648)
}
#= min neg | 0
def uint1 ors32_6() {
  return testConstOrs32(0,-2147483648,0 | -2147483648)
}
#= mid neg | mid neg
def uint1 ors32_7() {
  return testConstOrs32(-2147483648/2,-2147483648/2,-2147483648/2 | (-2147483648/2))
}
#= mid pos | mis pos
def uint1 ors32_8() {
  return testConstOrs32(2147483648/2,2147483646/2,2147483648/2 | 2147483646/2)
}

#==============  unsigned 32 ==================#
#= mid pos | mid pos
def uint1 oru16_1() {
  return testConstOru16(32767,32767,32767|32767)
}
#= max pos | 0
def uint1 oru16_2() {
  return testConstOru16(65535,0,65535|0)
}
def uint1 oru16_3() {
  return testConstOru16(0,0,0)
}

#==============  signed ==================#
#= min pos | min pos
def uint1 ors16_2() {
  return testConstOrs16(128,128,128|128)
}
#= max neg | max neg
def uint1 ors16_3() {
  return testConstOrs16(-129,-129,-129|(-129))
}
#= max pos | 0
def uint1 ors16_4() {
  return testConstOrs16(32767,0,32767|0)
}
#= max pos | min neg
def uint1 ors16_5() {
  return testConstOrs16(32767,-32768,32767|(-32768))
}
#= min neg | 0
def uint1 ors16_6() {
  return testConstOrs16(0,-32768,0| -32768)
}
#= mid neg | mid neg
def uint1 ors16_7() {
  return testConstOrs16(-32768/2,-32768/2,-32768 | -32768)
}
#= mid pos | mis pos
def uint1 ors16_8() {
  return testConstOrs16(32768/2,32766/2,32768/2|32766/2)
}

#==============  unsigned 8 ==================#
#= mid pos | mid pos
def uint1 oru8_1() {
  return testConstOru8(127,127,127|127)
}
#= max pos | 0
def uint1 oru8_2() {
  return testConstOru8(255,0,255|0)
}
def uint1 oru8_3() {
  return testConstOru8(0,0,0)
}

#==============  signed ==================#
#= max pos | 0
def uint1 ors8_4() {
  return testConstOrs8(127,0,127|0)
}
#= max pos | min neg
def uint1 ors8_5() {
  return testConstOrs8(127,-128,127|-128)
}
#= min neg | 0
def uint1 ors8_6() {
  return testConstOrs8(0,-128,0|-128)
}
#= mid neg | mid neg
def uint1 ors8_7() {
  return testConstOrs8(-64,-64,-64 | -64)
}
#= mid pos | mis pos
def uint1 ors8_8() {
  return testConstOrs8(64,63,64 | 63)
}