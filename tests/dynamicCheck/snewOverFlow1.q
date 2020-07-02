# EXPECT overflow

def void snewOverFlow() {
  uint128 n = 922337203685477580799
              #9223372036854775808
  sint64* a = new sint64[n]
}
