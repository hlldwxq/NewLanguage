def void merge(uint64* arr, uint64 left, uint64 mid, uint64 right, uint64* temp) {

    uint64 i = left
    uint64 j = mid + 1
    uint64 t = 0

    while (i <= mid & j <= right) { 
        if(arr[i] <= arr[j]) 
        then {
            temp[t] = arr[i]
            t = t + 1
            i = i + 1
        } 
        else { 
            temp[t] = arr[j]
            t = t + 1
            j = j + 1
        }
    }

    while( i <= mid) {
        temp[t] = arr[i]
        t = t + 1
        i = i + 1
    }
    while( j <= right) { 
        temp[t] = arr[j]
        t = t + 1
        j = j + 1
    }

    t = 0
    uint64 tempLeft = left
    while(tempLeft <= right) {
        arr[tempLeft] = temp[t]
        t = t + 1
        tempLeft = tempLeft + 1
    }
}

def void mergeSort(uint64* arr, uint64 left, uint64 right, uint64* temp) {
    if(left < right) 
    then{
        uint64 mid = (left + right) / 2
        mergeSort(arr, left, mid, temp)
        mergeSort(arr, mid + 1, right, temp)
        merge(arr, left, mid, right, temp)
    }
}

def void sort_merge(uint64* arr, uint64 left, uint64 right){
    uint64* temp = new uint64[right - left]
    mergeSort(arr, left, right-1, temp)
    free temp
}

def uint64 *newarray(uint64 n) { return new uint64[n] }
def void setarray(uint64* a,uint64 i,uint64 x) { a[i]=x }
def uint64 getarray(uint64* a,uint64 i) { return a[i] }