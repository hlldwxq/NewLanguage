
def void set(uint64 *a, uint64 i) {
  if i!=0 then {
    i=i-1
    a[i]=i+1
    set(a,i)
  }
}

def uint64 prod(uint64 *a, uint64 i, uint64 res) {
  if i==0 then return res
  else return prod(a,i-1, res * a[i-1])
}

 
def uint64 mytest(uint64 n) {
  uint64* a = new uint64[n]
  set(a,n)
  return prod(a,n,1)
}
