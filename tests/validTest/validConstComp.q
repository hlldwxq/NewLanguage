# constant folding

def uint1 testConstPlus(uint128 a, uint128 b, uint128 c) {
  return a+b == c
}

def uint1 testConstMinus(uint128 a, uint128 b, uint128 c) {
  return a-b == c
}

# TODO: add other operations

# Tests against overflows in compiler constant folding
def uint1 test1() {return testConstPlus(9223372036854775808,9223372036854775808,9223372036854775808+9223372036854775808)}
def uint1 test2() {return testConstPlus(9223372036854775809,9223372036854775809,9223372036854775809+9223372036854775809)}

# TODO: add more tests. Get corner-cases!

