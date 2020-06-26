 # give an array A, make a new array B, which B[i]=A[0]*A[1]***A[i-1]*A[i+1]***A[n]

 def sint64* multiply(sint64* A, uint64 length) {
    sint64* B = new sint64[length]
    
    B[0] = 1
    for uint64 i=1,i<length,1{
        B[i] = B[i-1] * A[i-1]
    }
    
    sint64 temp = 1
    uint64 i = length
    while i>0{
        i = i-1
        B[i] = B[i] * temp
        temp = A[i] * temp
    }
    return B
}