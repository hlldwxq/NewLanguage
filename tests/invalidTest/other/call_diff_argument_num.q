# EXPECT The number of participating arguments does not match
def void helper(sint64 a,sint64 b){}

def void ErrorTest(){
    sint64 a
    helper(a)
}