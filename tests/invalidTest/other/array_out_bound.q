# EXPECT the index of array cannot be negative

def void errorType(){
    sint64* a = new sint64[2]
    a[2] = 0
}