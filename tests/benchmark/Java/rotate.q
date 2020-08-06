uint64 ROTATE_THRESHOLD = 100
uint64 REVERSE_THRESHOLD = 18

def void swap(uint64* arr, uint64 i, uint64 j){
    uint64 temp = arr[j]
    arr[j] = arr[i]
    arr[i] = temp
}

def void reverse(uint64* arr,uint64 first, uint64 last){
    uint64 size = last - first
    if (size < REVERSE_THRESHOLD ) 
    then {
        uint64 mid = size / 2
        uint64 j = last - 1
        for uint64 i = first, i < mid, 1 {
            swap(arr, i, j)
            j = j - 1
        }
    } 
    else {
        uint64 mid = (last-first) / 2
        uint64 fwd = first
        uint64 rev = last - 1
        for uint64 i=0, i<mid, 1 {
            uint64 tmp = arr[fwd]
            arr[fwd] = arr[rev]
            arr[rev] = tmp
            fwd = fwd + 1
            rev = rev - 1
        }
    }
}

def uint64 set(uint64* arr, uint64 index, uint64 value){
    uint64 temp = arr[index]
    arr[index] = value
    return temp
}

def uint64 mod(uint64 m, uint64 med){
    while(m>=med){
        m = m - med
    }
    return m
}

def void rotate1(uint64* arr, uint64 distance, uint64 length){
    uint64 size = length
    if (size == 0) then
        return void

    distance = mod(distance,size)
    if (distance < 0) then
        distance = distance + size
    if (distance == 0) then
        return void
    
    uint64 nMoved = 0
    for uint64 cycleStart = 0, nMoved != size, 1 {

        uint64 displaced = arr[cycleStart]
        uint64 i = cycleStart

        i = i + distance
        if (i >= size)
        then i = i - size
        
        displaced = set(arr, i, displaced)
        nMoved = nMoved + 1

        while (i != cycleStart){
            i = i + distance
            if (i >= size)
            then i = i - size
            displaced = set(arr, i, displaced)
            nMoved = nMoved + 1
        }
    }
}

def uint64 negmod(uint64 m, uint64 med){
    while( m>=med ){
        m = m - med
    }
    
    return med-m
}

def void rotate2(uint64* arr, uint64 distance, uint64 last){
    uint64 size = last 
    if (size == 0)
    then return void

    uint64 mid = negmod(distance,size)

    if (mid < 0)
    then mid = mid + size
    if (mid == 0)
    then return void

    reverse(arr, 0, mid)
    reverse(arr, mid, last)
    reverse(arr, 0, last)
}

def void rotate(uint64* arr, uint64 distance, uint64 last){
    if distance < ROTATE_THRESHOLD
    then {
        rotate1(arr, distance, last)
    }
    else{
        rotate2(arr, distance, last)
    }
}

def uint64 *newarray(uint64 n) { return new uint64[n] }
def void setarray(uint64* a,uint64 i,uint64 x) { a[i]=x }
def uint64 getarray(uint64* a,uint64 i) { return a[i] }