# what need be tested for assign
#   int -> variable   const number   array index   callFun   binary
#   ptr -> variable   new    array index    callFun   
#   bool -> variable   true/false    array index   callFun   binary

# ================= bool ================= #
def sint64 boolAssignVar(sint64 a){
    uint1 a1 = true
    uint1 b = a1

    if b
    then return a
    else return 0
}

def sint64 boolAssignTrue(sint64 a){
    uint1 b = true

    if b
    then return a
    else return 0
}

def sint64 boolAssignFalse(sint64 a){
    uint1 b = false
    
    if b
    then return 0
    else return a
}

def sint64 boolAssignBin(sint64 a){
    uint1 b = (a+1)>a

    if b
    then return a
    else return 0
}

def sint64 boolAssignUnary(sint64 a){
    uint1 b = !((a+1)<a)

    if b
    then return a
    else return 0
}

def sint64 boolAssignArrIndex(sint64 a){
    uint1* array = new uint1[2]
    array[0] = true
    uint1 b = array[0]

    if b
    then return a
    else return 0
}
def uint1 bool1(){
    return true
}
def sint64 boolAssignCall(sint64 a){
    uint1 b = bool1()

    if b
    then return a
    else return 0
}

# ================== ptr ================= #
def sint64 PtrAssignVar(sint64 a){
    sint64* arr1 = new sint64[1]
    arr1[0] = a
    sint64* arr = arr1
    return arr[0]
}

def sint64 PtrAssignNew(sint64 a){
    sint64* arr = new sint64[1]
    arr[0] = a
    return arr[0]
}

def sint64 PtrAssignArrIndex(sint64 a){
    sint64** arr1 = new sint64*[1]
    arr1[0] = new sint64[1]
    arr1[0][0] = a
    sint64* arr = arr1[0]
    return arr[0]
}

def sint64* ptr(sint64 a){
    sint64* arr = new sint64[1]
    arr[0] = a
    return arr
}
def sint64 PtrAssignCall(sint64 a){
    sint64* arr = ptr(a)
    return arr[0]
}

# =================== int =================== #
def sint64 intAssignVar(sint64 a){
    sint64 b = a
    return b
}
def sint64 intAssignNum(){
    sint64 a = 0
    return a
}
def sint64 intAssignArrIndex(sint64 a){
    sint64* arr = new sint64[2]
    arr[0] = a
    sint64 b = arr[0]
    return b
}
def sint64 intAssignCall(sint64 a){
    sint64 b = intAssignVar(a)
    return b
}
def sint64 intAssignBinary(sint64 a){
    sint64 b = a *1
    return b
}
