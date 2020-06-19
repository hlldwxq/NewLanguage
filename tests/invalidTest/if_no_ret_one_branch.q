# Missing return in one branch of if
# EXPECT non-void function needs return
def uint64 prod(uint64 *a, uint64 i, uint64 res) {
  if i==0 then return res
  else prod(a,i-1, res * a[i-1])
}
