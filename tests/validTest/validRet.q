# what need to be tested in return
# return expr
#    int -> variable  number  binary  callFunction  arrayIndex
#    ptr -> variable  new  arrayIndex  callFunction
#    bool -> variable true|false binary arrayIndex callFunction unary

# ================= bool ================= #
def uint1 retBoolVar1(){
    uint1 b = true
    return b
}

def uint1 retBoolTrue1(){
    return true
}

def uint1 retBoolFalse1(){
    return false
}

def uint1 retBoolBin1(uint64 a){
    return a>0
}

def uint1 retBoolUnary1(uint64 a){
    return !(a<=0)
}

def uint1 retBoolArrIndex1(){
    uint1* array = new uint1[2]
    array[0] = true
    return array[0]
}

def uint1 retBoolCall1(){
    return retBoolVar1()
}
# ================== ptr ================= #
def uint64* retPtrVar1(uint64 a){
    uint64* arr = new uint64[1]
    return arr
}

def uint64* retPtrNew1(uint64 a){
    return new uint64[1]
}

def uint64* retPtrArrIndex1(uint64 a){
    uint64** arr = new uint64*[1]
    arr[0] = new uint64[1]
    return arr[0]
}

def uint64* retPtrCall1(uint64 a){
    return retPtrVar1(a)
}

# ================= bool ================= #
def uint64 retBoolVar(uint64 a){
    if retBoolVar1()
    then return a
    else return 0
}

def uint64 retBoolTrue(uint64 a){
    if retBoolTrue1()
    then return a
    else return 0
}

def uint64 retBoolFalse(uint64 a){
    if retBoolFalse1()
    then return 0
    else return a
}

def uint64 retBoolBin(uint64 a){
    if retBoolBin1(a)
    then return a
    else return 0
}

def uint64 retBoolUnary(uint64 a){
    if retBoolUnary1(a)
    then return a
    else return 0
}

def uint64 retBoolArrIndex(uint64 a){
    if retBoolArrIndex1()
    then return a
    else return 0
}

def uint64 retBoolCall(uint64 a){
    if retBoolCall1()
    then return a
    else return 0
}
# ================== ptr ================= #
def uint64 retPtrVar(uint64 a){
    uint64* arr = retPtrVar1(a)
    arr[0] = a
    return arr[0]
}

def uint64 retPtrNew(uint64 a){
    uint64* arr = retPtrNew1(a)
    arr[0] = a
    return arr[0]
}

def uint64 retPtrArrIndex(uint64 a){
    uint64* arr = retPtrArrIndex1(a)
    arr[0] = a
    return arr[0]
}

def uint64 retPtrCall(uint64 a){
    uint64* arr = retPtrCall1(a)
    arr[0] = a
    return arr[0]
}
# =================== int =================== #
#ret a
def sint64 retIntVar(sint64 a){
    return a
}

def sint64 retIntCall(sint64 a){
    return retIntVar(a)
}

def sint64 retIntArrayIndex(sint64 a){
    sint64* arr = new sint64[1]
    arr[0] = a
    return arr[0]
}

# ret 0
def sint64 retIntNum(){
    return 0
}

# ret a+b
def sint64 retIntBinary(sint64 a,sint64 b){
    return a+b
}

