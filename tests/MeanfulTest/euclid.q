
def sint64 euclid(sint64 a,sint64 b) {
  if (a==0 | b==0) then return 0
  else if (a<b) then return euclid(a,b-a)
  else if (a>b) then return euclid(a-b,b)
  else return a
}
