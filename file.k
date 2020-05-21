def int64 FindMaxNum ( int64* arr ,int64 size){
    int64 maxNum = arr[0]
    maxNum = arr[2]
     for int64 i=1 , i < size, 1 {
            if arr[i] < maxNum
            then  {maxNum = arr[i]}
      }

     return maxNum
}

def int64 FindMaxNum2( int64** arr ,int64 size1, int64 size2){
     int64 maxNum = arr[0][0]

     for int64 i , i < size1, 1 {   #default value of i and j is 0
           int64 j
           while j<size2{
                if arr[i][j] > maxNum 
                then {maxNum = arr[i][j]}
                else {maxNum = maxNum}

                j = j+1
           }
     }

     return maxNum
}

def int1 hasNum (int64* arr, int64 a, int64 index){

     for int64 i, i<index, 1 {
           if a==arr[i]
           then return 1    
     }

     return 0
}

def int64* FindCommonNum (  int64* arr1, int64 size1, int64* arr2, int64 size2 ){

      int64 initSize

      if size1 < size2
      then initsize = size1
      else initsize = size2

      int64* common
      common = new int64 [initSize]
      int64 index
      for int64 i, i<size1, 1 {
            for int64 j, j<size2, 1{
                if (arr1[i] == arr2[j]) | (hasNum(common, arr1[i],index))
                then {
                        common[index] = arr1[i]
                        index = index+1
                }
            }
      }

      int64* result = new int64 [index]
      for int64 i , i<index , 1{
             result[i] = common[i]
      }

      return result
}
