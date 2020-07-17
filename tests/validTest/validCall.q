# what need to be tested in call
# return value
# parameter

def sint64 intReturnType(sint64 a){
    return a 
}

def sint64* pointerReturnType(sint64 a){
    sint64* array = new sint64[5]
    for uint64 i , i < 5 , 1
        array[i] = a
    return array
}

def sint64 intPara(sint64 a, sint64 b, sint64 c){
    sint64 temp = a - b + c
    return temp
}

def sint64 ptrPara(sint64* a, sint64* b, sint64* c){
    sint64 temp = a[0] - b[0] + c[0]
    return temp
}

#return a
def sint64 callRetInt(sint64 a){
    sint64 temp = intReturnType(a)
    return temp
}

def sint64 callRetPtr(sint64 a){
    sint64* temp = pointerReturnType(a)
    sint64 t = temp[0]
    return t
}

def sint64 callParaInt(sint64 a){
    sint64 temp = intPara(a,a,a)
    return temp
}

def sint64 callParaPtr(sint64 a){
    sint64* array = new sint64[5]
    for uint64 i, i<5 , 1
        array[i] = a
    sint64 temp = ptrPara(array, array, array)
    return temp
}

