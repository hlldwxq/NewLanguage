def sint64 FindMaxNum ( sint64* arr ,uint64 size){
    sint64 maxNum = arr[0]
    maxNum = arr[2]
     for uint64 i=1 , i < size, 1 {
            if arr[i] < maxNum
            then  {maxNum = arr[i]}
      }

     return maxNum
}

def sint64 FindMaxNum2( sint64** arr ,uint64 size1, uint64 size2){
     sint64 maxNum = arr[0][0]

     for uint64 i , i < size1, 1 {   #default value of i and j is 0
           sint64 j
           while j<size2{
                if arr[i][j] > maxNum 
                then {maxNum = arr[i][j]}
                else {maxNum = maxNum}

                j = j+1
           }
     }

     return maxNum
}

def uint1 hasNum (sint64* arr, sint64 a, uint64 index){

     for sint64 i, i<index, 1 {
           if a==arr[i]
           then return 1    
     }

     return 0
}

def sint64* FindCommonNum (  sint64* arr1, uint64 size1, sint64* arr2, uint64 size2 ){

      uint64 initSize

      if size1 < size2
      then initSize = size1
      else initSize = size2

      sint64* common
      common = new sint64 [initSize]
      uint64 index
      for uint64 i, i<size1, 1 {
            for uint64 j, j<size2, 1{
                if (arr1[i] == arr2[j]) | (hasNum(common, arr1[i],index))
                then {
                        common[index] = arr1[i]
                        index = index+1
                }
            }
      }

      sint64* result = new sint64 [index]
      for uint64 i , i<index , 1{
             result[i] = common[i]
      }

      return result
}
