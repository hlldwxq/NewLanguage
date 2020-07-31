# create new basicblock in for/while loop
# what generates new basicblock:
#   for / while / if
#   new array
#   arithmatic calculation ( when doing dynamic check ) / array index

# ----------------contain if else and arithmatic cal in while loop------ #
def uint32 findContinueNum(uint32 sum){

    uint32 a
    uint32 l = 1
    uint32 r = 1
    uint32 s = 1
    while(l<=sum/2){
        if(s<sum)
        then {
            r = r + 1
            s = s + r
        }else if(s>sum)
        then{
            s = s - l
            l = l + 1
        }else {
            a = a + 1
            s = s - l
            l = l + 1
        }
    }
    return a
}


# ----------------contain new array/ while/ if / array index / or loop in while loop------------------------- #
def uint32 findMaxNumWhile(uint32** arr, uint32 col, uint32 row){
    uint32 max = 0 
    uint32 i = 0
    while (i<row)
    {
        uint32 j = 0
        while j<col{
            if arr[i][j] > max
            then max = arr[i][j]
            j = j+1
        }
        i = i + 1
    }
    return max
}
def uint32 findMaxNumAccessWhile(uint32 col, uint32 row){
    uint32** arr = new uint32*[row]
    uint32 i = 0
    while i < row {
        arr[i] = new uint32[col]
        i = i + 1
    }
    i = 0
    while i < row{
        for uint32 j=0 , j < col , 1{
            arr[i][j] = i*j
        }
        i = i+1
    }

    return findMaxNumWhile(arr,col,row)
}

# ===================================for================================ #

# ----------------contains if and arithmatic cal in for loop------------ #
def uint32 findNumWithSum(uint32 count, uint32 sum){
    uint32 a = 0
    for uint32 i = 1 , i < count , 1 {
        a = a + i
        if a > sum
        then return i - 1
    }
    return 0
}


# ----------------contains for loop and array index in a for loop------- #
def uint32* bubbleSort(uint32*arr, uint32 length){  
   
    # it is a sort
    for uint32 i=0, i<length, 1{
        for uint32 j=0, j<length-1-i, 1{
            if arr[j] > arr[j+1]
            then{
                uint32 temp = arr[j+1]
                arr[j+1] = arr[j]
                arr[j] = temp
            }
        }
    }
    return arr
}
# ----------------contains if and array index in a for loop------------- #
def uint32 findUniqueNum(uint32* array, uint32 length){
    
    array = bubbleSort(array,length)  # sort

    for uint32 i=0 , i<length-1, 0 {
        if(array[i] != array[i+1]) 
        then{
            return array[i]
        }else{
            i = i + 2
        }
    }
    return array[length-1]  
}
def uint32 findUniqueNumAccess(uint32 length){
   
    uint32* arr = new uint32[length]
    arr[0] = length*length
    for uint32 index = 1, index < length-1, 2{
        arr[index] = length * index
        arr[index+1] = arr[index]
    }
    return findUniqueNum(arr,length)
}


# ----------------contain for loop/if/new array/while loop in for loop------------------------- #
def uint32 findMaxNumFor(uint32** arr, uint32 col, uint32 row){
    uint32 max = 0
    for uint32 i=0, i<row,1{
        for uint32 j=0,j<col,1{
            if arr[i][j] > max
            then max = arr[i][j]
        }
    }
    return max
}
def uint32 findMaxNumAccessFor(uint32 col, uint32 row){
    uint32** arr = new uint32*[row]
    for uint32 i = 0, i < row, 1{
        arr[i] = new uint32[col]
    }

    for uint32 i=0, i<row, 1{
        uint32 j=0
        while j<col{
            arr[i][j] = i*j
            j = j+1
        }
    }

    return findMaxNumFor(arr,col,row)
}