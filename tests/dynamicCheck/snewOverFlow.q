# EXPECT overflow
# Only overflows on platform with size_t <= 64 bits
def void snewOverFlow() {
  sint64* a = new sint64[9223372036854775807]             
}
