# EXPECT The operands of a binary operator cannot be a pointer

def void errorTest(){
    sint64* arr = new sint64[2]
    sint64 a = arr + arr
}