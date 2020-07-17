
def uint64 ReverseFibonacci(sint64 a) {
  if a==1
  then return 1

  sint64* array
  array = null
  array = new sint64[2]
  uint64 i = 1
  while true {
    if i<=2
    then array[i-1] = 1
    else { 
      sint64 n = array[0]+array[1]
      if n>a
      then break
      else {
        array[0] = array[1]
        array[1] = n
      }
    }
    i = i+1
  }
  return i-1
}
