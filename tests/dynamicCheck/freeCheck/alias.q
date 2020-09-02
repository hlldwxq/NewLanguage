# EXPECT the array has been free

sint8 *c

def void f2(uint1 b) {
  sint8* a = new sint8[5]
  c=a
  if b then free(a)
}

def void errorDef(){
  f2(false)
  c[0]=1
}
