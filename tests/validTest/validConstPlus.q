# constant folding

def uint1 testConstPlusu128(uint128 a, uint128 b, uint128 c) {
  return a+b == c
}

def uint1 testConstPluss128(sint128 a, sint128 b, sint128 c) {
  return a+b == c
}

def uint1 testConstPlusu64(uint64 a, uint64 b, uint64 c) {
  return a+b == c
}

def uint1 testConstPluss64(sint64 a, sint64 b, sint64 c) {
  return a+b == c
}

def uint1 testConstPlusu32(uint32 a, uint32 b, uint32 c) {
  return a+b == c
}

def uint1 testConstPluss32(sint32 a, sint32 b, sint32 c) {
  return a+b == c
}

def uint1 testConstPlusu16(uint16 a, uint16 b, uint16 c) {
  return a+b == c
}

def uint1 testConstPluss16(sint16 a, sint16 b, sint16 c) {
  return a+b == c
}

def uint1 testConstPlusu8(uint8 a, uint8 b, uint8 c) {
  return a+b == c
}

def uint1 testConstPluss8(sint8 a, sint8 b, sint8 c) {
  return a+b == c
}



#==============  unsigned 128 ==================#
#= mid pos + mid pos
def uint1 plusu128_1() {
  return testConstPlusu128(170141183460469231731687303715884105727,170141183460469231731687303715884105728,170141183460469231731687303715884105727+170141183460469231731687303715884105728)
}
#= max pos + 0
def uint1 plusu128_2() {
  return testConstPlusu128(340282366920938463463374607431768211455,0,340282366920938463463374607431768211455)
}
def uint1 plusu128_3() {
  return testConstPlusu128(0,0,0)
}

#==============  signed ==================#
#= min pos + min pos
def uint1 pluss128_2() {
  return testConstPluss128(9223372036854775808,9223372036854775808,9223372036854775808+9223372036854775808)
}
#= max neg + max neg
def uint1 pluss128_3() {
  return testConstPluss128(-9223372036854775809,-9223372036854775809,-9223372036854775809+(-9223372036854775809))
}
#= max pos + 0
def uint1 pluss128_4() {
  return testConstPluss128(170141183460469231731687303715884105727,0,170141183460469231731687303715884105727)
}
#= max pos + min neg
def uint1 pluss128_5() {
  return testConstPluss128(170141183460469231731687303715884105727,-170141183460469231731687303715884105728,170141183460469231731687303715884105727-170141183460469231731687303715884105728)
}
#= min neg + 0
def uint1 pluss128_6() {
  return testConstPluss128(0,-170141183460469231731687303715884105728,0-170141183460469231731687303715884105728)
}
#= mid neg + mid neg
def uint1 pluss128_7(){
  return testConstPluss128(-170141183460469231731687303715884105728/2,-170141183460469231731687303715884105728/2,-170141183460469231731687303715884105728)
}
#= mid pos + mis pos
def uint1 pluss128_8() {
  return testConstPluss128(170141183460469231731687303715884105728/2,170141183460469231731687303715884105726/2,170141183460469231731687303715884105727)
}

#==============  unsigned 64 ==================#
#= mid pos + mid pos
def uint1 plusu64_1() {
  return testConstPlusu64(9223372036854775807,9223372036854775807,9223372036854775807+9223372036854775807)
}
#= max pos + 0
def uint1 plusu64_2() {
  return testConstPlusu64(18446744073709551615,0,18446744073709551615)
}
def uint1 plusu64_3() {
  return testConstPlusu64(0,0,0)
}

#==============  signed ==================#
#= min pos + min pos
def uint1 pluss64_2() {
  return testConstPluss64(2147483648,2147483648,2147483648+2147483648)
}
#= max neg + max neg
def uint1 pluss64_3() {
  return testConstPluss64(-2147483649,-2147483649,-2147483649+(-2147483649))
}
#= max pos + 0
def uint1 pluss64_4() {
  return testConstPluss64(9223372036854775807,0,9223372036854775807)
}
#= max pos + min neg
def uint1 pluss64_5() {
  return testConstPluss64(9223372036854775807,-9223372036854775808,9223372036854775807-9223372036854775808)
}
#= min neg + 0
def uint1 pluss64_6() {
  return testConstPluss64(0,-9223372036854775808,0-9223372036854775808)
}
#= mid neg + mid neg
def uint1 pluss64_7() {
  return testConstPluss64(-9223372036854775808/2,-9223372036854775808/2,-9223372036854775808)
}
#= mid pos + mis pos
def uint1 pluss64_8() {
  return testConstPluss64(9223372036854775808/2,9223372036854775806/2,9223372036854775807)
}

#==============  unsigned 32 ==================#
#= mid pos + mid pos
def uint1 plusu32_1() {
  return testConstPlusu32(2147483647,2147483647,2147483647+2147483647)
}
#= max pos + 0
def uint1 plusu32_2() {
  return testConstPlusu32(4294967295,0,4294967295)
}
def uint1 plusu32_3() {
  return testConstPlusu32(0,0,0)
}

#==============  signed ==================#
#= min pos + min pos
def uint1 pluss32_2() {
  return testConstPluss32(32768,32768,32768+32768)
}
#= max neg + max neg
def uint1 pluss32_3() {
  return testConstPluss32(-32769,-32769,-32769+(-32769))
}
#= max pos + 0
def uint1 pluss32_4() {
  return testConstPluss32(2147483647,0,2147483647)
}
#= max pos + min neg
def uint1 pluss32_5() {
  return testConstPluss32(2147483647,-2147483648,2147483647-2147483648)
}
#= min neg + 0
def uint1 pluss32_6() {
  return testConstPluss32(0,-2147483648,0-2147483648)
}
#= mid neg + mid neg
def uint1 pluss32_7() {
  return testConstPluss32(-2147483648/2,-2147483648/2,-2147483648)
}
#= mid pos + mis pos
def uint1 pluss32_8() {
  return testConstPluss32(2147483648/2,2147483646/2,2147483647)
}

#==============  unsigned 32 ==================#
#= mid pos + mid pos
def uint1 plusu16_1() {
  return testConstPlusu16(32767,32767,32767+32767)
}
#= max pos + 0
def uint1 plusu16_2() {
  return testConstPlusu16(65535,0,65535)
}
def uint1 plusu16_3() {
  return testConstPlusu16(0,0,0)
}

#==============  signed ==================#
#= min pos + min pos
def uint1 pluss16_2() {
  return testConstPluss16(128,128,128+128)
}
#= max neg + max neg
def uint1 pluss16_3() {
  return testConstPluss16(-129,-129,-129+(-129))
}
#= max pos + 0
def uint1 pluss16_4() {
  return testConstPluss16(32767,0,32767)
}
#= max pos + min neg
def uint1 pluss16_5() {
  return testConstPluss16(32767,-32768,32767-32768)
}
#= min neg + 0
def uint1 pluss16_6() {
  return testConstPluss16(0,-32768,0-32768)
}
#= mid neg + mid neg
def uint1 pluss16_7() {
  return testConstPluss16(-32768/2,-32768/2,-32768)
}
#= mid pos + mis pos
def uint1 pluss16_8() {
  return testConstPluss16(32768/2,32766/2,32767)
}

#==============  unsigned 8 ==================#
#= mid pos + mid pos
def uint1 plusu8_1() {
  return testConstPlusu8(127,127,127+127)
}
#= max pos + 0
def uint1 plusu8_2() {
  return testConstPlusu8(255,0,255)
}
def uint1 plusu8_3() {
  return testConstPlusu8(0,0,0)
}

#==============  signed ==================#
#= max pos + 0
def uint1 pluss8_4() {
  return testConstPluss8(127,0,127)
}
#= max pos + min neg
def uint1 pluss8_5() {
  return testConstPluss8(127,-128,127-128)
}
#= min neg + 0
def uint1 pluss8_6() {
  return testConstPluss8(0,-128,0-128)
}
#= mid neg + mid neg
def uint1 pluss8_7() {
  return testConstPluss8(-64,-64,-128)
}
#= mid pos + mis pos
def uint1 pluss8_8() {
  return testConstPluss8(64,63,127)
}