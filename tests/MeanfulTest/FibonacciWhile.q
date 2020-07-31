
def sint64 Fibonacci(uint64 a) {
  sint64* array = new sint64[a+1]
  uint64 i = 0
  while i<=a {
    if i<=2
    then array[i] = 1
    else array[i] = array[i-1]+array[i-2]

    i = i+1
  }
  return array[a]
}
