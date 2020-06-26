# EXPECT overflow
# Only overflows on platform with size_t <= 64 bits
def void snewOverFlow() {
  uint128 n = 1152921504606846976 * 16
  sint8* a = new sint8[n]
}
