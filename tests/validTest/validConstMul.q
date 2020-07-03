# constant folding

def uint1 testConstMulu128(uint128 a, uint128 b, uint128 c) {
  return a*b == c
}

def uint1 testConstMuls128(sint128 a, sint128 b, sint128 c) {
  return a*b == c
}

def uint1 testConstMulu64(uint64 a, uint64 b, uint64 c) {
  return a*b == c
}

def uint1 testConstMuls64(sint64 a, sint64 b, sint64 c) {
  return a*b == c
}

def uint1 testConstMulu32(uint32 a, uint32 b, uint32 c) {
  return a*b == c
}

def uint1 testConstMuls32(sint32 a, sint32 b, sint32 c) {
  return a*b == c
}

def uint1 testConstMulu16(uint16 a, uint16 b, uint16 c) {
  return a*b == c
}

def uint1 testConstMuls16(sint16 a, sint16 b, sint16 c) {
  return a*b == c
}

def uint1 testConstMulu8(uint8 a, uint8 b, uint8 c) {
  return a*b == c
}

def uint1 testConstMuls8(sint8 a, sint8 b, sint8 c) {
  return a*b == c
}
#==============  unsigned 128 ==================#
def uint1 mulu128_1() {
  return testConstMulu128(340282366920938463463374607431768211455,1,340282366920938463463374607431768211455)
}
def uint1 mulu128_2() {
  return testConstMulu128(1,1,1)
}
def uint1 mulu128_3() {
  return testConstMulu128(0,0,0)
}

#==============  signed ==================#

def uint1 muls128_2() {
  return testConstMuls128(170141183460469231731687303715884105727,1,170141183460469231731687303715884105727)
}

def uint1 muls128_3() {
  return testConstMuls128(-170141183460469231731687303715884105727,-1,170141183460469231731687303715884105727)
}

def uint1 muls128_4() {
  return testConstMuls128(170141183460469231731687303715884105727,-1,-170141183460469231731687303715884105727)
}

def uint1 muls128_5() {
  return testConstMuls128(-170141183460469231731687303715884105728,1,-170141183460469231731687303715884105728)
}

def uint1 muls128_6() {
  return testConstMuls128(170141183460469231731687303715884105727,0,0)
}

def uint1 muls128_7() {
  return testConstMuls128(-170141183460469231731687303715884105728,0,0)
}

#==============  unsigned 64 ==================#
def uint1 mulu64_1() {
  return testConstMulu64(18446744073709551615,1,18446744073709551615)
}
def uint1 mulu64_2() {
  return testConstMulu64(1,1,1)
}
def uint1 mulu64_3() {
  return testConstMulu64(0,0,0)
}

#==============  signed ==================#

def uint1 muls64_2() {
  return testConstMuls64(9223372036854775807,1,9223372036854775807)
}

def uint1 muls64_3() {
  return testConstMuls64(-9223372036854775807,-1,9223372036854775807)
}

def uint1 muls64_4() {
  return testConstMuls64(9223372036854775807,-1,-9223372036854775807)
}

def uint1 muls64_5() {
  return testConstMuls64(-9223372036854775808,1,-9223372036854775808)
}

def uint1 muls64_6() {
  return testConstMuls64(9223372036854775807,0,0)
}

def uint1 muls64_7() {
  return testConstMuls64(-9223372036854775808,0,0)
}

#==============  unsigned 32 ==================#
def uint1 mulu32_1() {
  return testConstMulu32(4294967295,1,4294967295)
}
def uint1 mulu32_2() {
  return testConstMulu32(1,1,1)
}
def uint1 mulu32_3() {
  return testConstMulu32(0,0,0)
}

#==============  signed ==================#

def uint1 muls32_2() {
  return testConstMuls32(2147483647,1,2147483647)
}

def uint1 muls32_3() {
  return testConstMuls32(-2147483647,-1,2147483647)
}

def uint1 muls32_4() {
  return testConstMuls32(2147483647,-1,-2147483647)
}

def uint1 muls32_5() {
  return testConstMuls32(-2147483648,1,-2147483648)
}

def uint1 muls32_6() {
  return testConstMuls32(2147483647,0,0)
}

def uint1 muls32_7() {
  return testConstMuls32(-2147483648,0,0)
}

#==============  unsigned 16 ==================#
def uint1 mulu16_1() {
  return testConstMulu16(65535,1,65535)
}
def uint1 mulu16_2() {
  return testConstMulu16(1,1,1)
}
def uint1 mulu16_3() {
  return testConstMulu16(0,0,0)
}

#==============  signed ==================#

def uint1 muls16_2() {
  return testConstMuls16(32767,1,32767)
}

def uint1 muls16_3() {
  return testConstMuls16(-32767,-1,32767)
}

def uint1 muls16_4() {
  return testConstMuls16(32767,-1,-32767)
}

def uint1 muls16_5() {
  return testConstMuls16(-32768,1,-32768)
}

def uint1 muls16_6() {
  return testConstMuls16(32767,0,0)
}

def uint1 muls16_7() {
  return testConstMuls16(-32768,0,0)
}

#==============  unsigned 8 ==================#
def uint1 mulu8_1() {
  return testConstMulu8(255,1,255)
}
def uint1 mulu8_2() {
  return testConstMulu8(1,1,1)
}
def uint1 mulu8_3() {
  return testConstMulu8(0,0,0)
}

#==============  signed ==================#

def uint1 muls8_2() {
  return testConstMuls8(127,1,127)
}

def uint1 muls8_3() {
  return testConstMuls8(-127,-1,127)
}

def uint1 muls8_4() {
  return testConstMuls8(127,-1,-127)
}

def uint1 muls8_5() {
  return testConstMuls8(-128,1,-128)
}

def uint1 muls8_6() {
  return testConstMuls8(127,0,0)
}

def uint1 muls8_7() {
  return testConstMuls8(-128,0,0)
}