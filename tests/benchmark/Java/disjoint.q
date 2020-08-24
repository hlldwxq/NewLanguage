def uint64 indexOfRange(uint64* arr, uint64 first, uint64 n, uint64 val){
    for uint64 i=first, i<n, 1{
        if(val == arr[i])
        then return i
    }
    return n
}

def uint64 indexOf(uint64* arr, uint64 n, uint64 val){
    return indexOfRange(arr,0,n,val)
}

def uint1 contain(uint64* arr, uint64 n, uint64 val){
    return indexOf(arr,n,val)<n
}

def uint1 disjoint(uint64* arr1, uint64 n1, uint64* arr2, uint64 n2){
    if(n1==0 | n2==0)
    then{
        return true
    }

    uint64* iterate
    uint64* contains
    uint64 n_it
    uint64 n_cnt


    if(n1<n2)
    then{
        iterate = arr1
        contains = arr2
        n_it = n1
        n_cnt = n2
    }
    else{
        iterate = arr2
        contains = arr1
        n_it = n2
        n_cnt = n1
    }
    for uint64 i=0, i<n_it, 1{
        if( contain(contains, n_cnt, iterate[i]) ) #because I use ArrayList in Java code, so I copy the contains func of ArrayList
        then return false
    }
    return true
}

def uint64 *newarray(uint64 n) { return new uint64[n] }
def void setarray(uint64* a,uint64 i,uint64 x) { a[i]=x }
def uint64 getarray(uint64* a,uint64 i) { return a[i] }
