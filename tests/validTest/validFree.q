def sint64 testNewSint(uint64 a,sint64 s){
    sint64* arr = new sint64[a]
    
    for uint64 i,i<a,1{
        arr[i] = s
    }

    free arr
    return arr[3]
}

def uint64 testNewUint(uint64 a,uint64 s){
    uint64*** arr = new uint64**[a]
    
    for uint64 i,i<a,1{
        arr[i] = new uint64*[a]
    }

    for uint64 i,i<a,1{
        for uint64 j,j<a,1
            arr[i][j] = new uint64[a]
    }
    for uint64 i,i<a,1{
        for uint64 j,j<a,1
            for uint64 z,z<a,1
                arr[i][j][z] = s
    }

    return arr[a-1][a-1][a-1]
}
