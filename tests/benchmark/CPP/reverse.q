def void swap(sint64* arr, uint64 index1, uint64 index2){
    sint64 temp = arr[index1]
    arr[index1] = arr[index2]
    arr[index2] = temp
}

def void reverse(sint64* arr, uint64 first, uint64 last){

    while (first<last){
        last = last - 1
        swap(arr,first,last)
        first = first + 1
    }
}

def sint64* copyArr(sint64* arr, uint64 length){
    sint64* copyA = new sint64[length]
    for uint64 i=0, i < length, 1 {
        copyA[i] = arr[i]
    }
    return copyA
}

def uint64 *newarray(uint64 n) { return new uint64[n] }
def void setarray(uint64* a,uint64 i,uint64 x) { a[i]=x }
def uint64 getarray(uint64* a,uint64 i) { return a[i] }
