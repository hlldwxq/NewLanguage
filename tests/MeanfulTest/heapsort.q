def void h__push_heap(uint64* arr,uint64 first, uint64 holeIndex, uint64 topIndex, uint64 value)
{
    if  holeIndex<1
    then return void
    
    uint64 parent = (holeIndex - 1) / 2
    while (holeIndex > topIndex & arr[first + parent] < value) {
        arr[first + holeIndex] = arr[first + parent]
        holeIndex = parent
        if holeIndex<1 then return void
        parent = (holeIndex - 1) / 2
    }
    arr[first + holeIndex] = value
}

def void h__push_heap_aux(uint64* arr,uint64 first, uint64 last)
{
    h__push_heap(arr, first, last-first-1, 0,arr[last-1])
}

def void push_heap(uint64* arr, uint64 first, uint64 last)
{
    h__push_heap_aux(arr, first, last)
}

def void h__adjust_heap(uint64* arr, uint64 first, uint64 holeIndex, uint64 len, uint64 value)
{
    uint64 topIndex = holeIndex
    uint64 secondChild = 2 * holeIndex + 2
    while (secondChild < len) {
        
        if (arr[first + secondChild] < arr[first + secondChild - 1])
        then   secondChild = secondChild - 1

        arr[first + holeIndex] = arr[first + secondChild]
        holeIndex = secondChild
        secondChild = 2 * (secondChild + 1)
    }
    if (secondChild == len)
    then{
        arr[first + holeIndex] = arr[first + (secondChild - 1)]
        holeIndex = secondChild - 1
    }
    
    h__push_heap(arr, first, holeIndex, topIndex, value)
}

def void make_heap(uint64* arr, uint64 first, uint64 last)
{
    if (last - first < 2)
    then return void

    uint64 len = last - first
    uint64 parent = (len - 2) / 2
    while (true) {
        h__adjust_heap(arr, first, parent, len,arr[first+parent])
        if (parent == 0) then return void
        parent = parent - 1
    }
}

def void h__pop_heap(uint64* arr, uint64 first, uint64 last, uint64 result, uint64 value)
{
    arr[result] = arr[first]
    h__adjust_heap(arr,first, 0, last-first, value)
}

def void h__pop_heap_aux( uint64* arr, uint64 first, uint64 last)
{
    h__pop_heap(arr, first, last-1, last-1,arr[last-1])
}

def void pop_heap(uint64* arr, uint64 first, uint64 last)
{
    h__pop_heap_aux(arr, first, last)
}


def void sort_heap(uint64* arr, uint64 first, uint64 last) {
    while (last - first > 1){
        pop_heap(arr, first, last)
        last = last - 1
    }
}

def uint64 *newarray(uint64 n) { return new uint64[n] }
def void setarray(uint64* a,uint64 i,uint64 x) { a[i]=x }
def uint64 getarray(uint64* a,uint64 i) { return a[i] }