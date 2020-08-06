uint64 REVERSE_THRESHOLD = 18

def void swap(uint64* arr, uint64 i, uint64 j){
    uint64 temp = arr[j]
    arr[j] = arr[i]
    arr[i] = temp
}

def void reverse(uint64* list, uint64 size){
    if (size < REVERSE_THRESHOLD ) 
    then {
        uint64 mid = size / 2
        uint64 j = size - 1
        for uint64 i = 0, i < mid, 1 {
            swap(list, i, j)
            j = j - 1
        }
    } 
    else {
        uint64 mid = size / 2
        uint64 fwd = 0
        uint64 rev = size - 1
        for uint64 i=0, i<mid, 1 {
            uint64 tmp = list[fwd]
            list[fwd] = list[rev]
            list[rev] = tmp
            fwd = fwd + 1
            rev = rev - 1
        }
    }
}

def uint64 *newarray(uint64 n) { return new uint64[n] }
def void setarray(uint64* a,uint64 i,uint64 x) { a[i]=x }
def uint64 getarray(uint64* a,uint64 i) { return a[i] }