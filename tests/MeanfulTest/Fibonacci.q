
def sint64 Fibonacci(sint64 a) {
  sint64* array = new sint64[a+1]
  for sint64 i = 0, i<=a ,1
    if i<=2
    then array[i] = 1
    else array[i] = array[i-1]+array[i-2]

  return array[a]
}