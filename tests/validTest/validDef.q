# what need be tested for Def
#   int -> variable   const number   array index   callFun   binary
#   ptr -> variable   new    array index    callFun   
#   bool -> variable   true/false    array index   callFun   binary

# ================= bool ================= #
def uint64 boolDefVar(uint64 a){
    uint1 a1 = true
    uint1 b = false
    b = a1

    if b
    then return a
    else return 0
}

def uint64 boolDefTrue(uint64 a){
    uint1 b = false
    b = true

    if b
    then return a
    else return 0
}

def uint64 boolDefFalse(uint64 a){
    uint1 b =true
    b = false
    
    if b
    then return 0
    else return a
}

def uint64 boolDefBin(uint64 a){
    uint1 b = false
    b = a>0

    if b
    then return a
    else return 0
}

def uint64 boolDefUnary(uint64 a){
    uint1 b =false
    b = !(a<=0)

    if b
    then return a
    else return 0
}

def uint64 boolDefArrIndex(uint64 a){
    uint1* array = new uint1[2]
    array[0] = true

    uint1 b = false
    b = array[0]

    if b
    then return a
    else return 0
}

def uint1 bool1(){
    return true
}
def uint64 boolDefCall(uint64 a){
    uint1 b = false
    b = bool1()

    if b
    then return a
    else return 0
}

# ================== ptr ================= #
def uint64 PtrDefVar(uint64 a){
    uint64* arr1 = new uint64[1]
    arr1[0] = a

    uint64* arr
    arr = arr1
    return arr[0]
}

def uint64 PtrDefNew(uint64 a){
    uint64* arr
    arr = new uint64[1]

    arr[0] = a
    return arr[0]
}

def uint64 PtrDefArrIndex(uint64 a){
    uint64** arr1 = new uint64*[1]
    arr1[0] = new uint64[1]
    arr1[0][0] = a

    uint64* arr
    arr = arr1[0]
    return arr[0]
}

def uint64* ptr(uint64 a){
    uint64* arr = new uint64[1]
    arr[0] = a
    return arr
}
def uint64 PtrDefCall(uint64 a){
    uint64* arr
    arr = ptr(a)
    return arr[0]
}

# =================== int =================== #
def sint64 intDefVar(sint64 a){
    sint64 b

    b = a
    return b
}
def sint64 intDefNum(){
    sint64 a

    a = 0
    return a
}
def sint64 intDefArrIndex(sint64 a){
    sint64* arr = new sint64[2]
    arr[0] = a

    sint64 b
    b = arr[0]
    return b
}
def sint64 intDefCall(sint64 a){
    sint64 b

    b = intDefVar(a)
    return b
}
def sint64 intDefBinary(sint64 a){
    sint64 b
    
    b = a*1
    return b
}