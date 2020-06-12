def uint64 f(uint64 x) {
  if x==0 then return 1
  if x==1 then return 1
  return f(x-1) + f(x-2)
}
