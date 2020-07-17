# EXPECT the array index must be unsigned number

def void errorType(){
    sint64* a = new sint64[2]
    a[-1] = 0
}