def sint64 findNum(sint64* numList,sint64 num,sint64 len){
    for sint64 i , i<len ,1{
        if numList[i] == num
        then return 1
    }
    return 0
}
# return the first repeated number, if no repeated num, return 0
def sint64 duplicate(sint64* number,sint64 length) {
    sint64 repeatedNum = 0
    sint64 len = 0
    sint64* newList = new sint64[length]
    for sint64 i, i<length, 1{
        if findNum(newList,number[i],len)==1
        then { 
            repeatedNum = number[i]  
            break 
        }
        else { 
            newList[len]=number[i] 
            len = len+1
        }
    }
    return repeatedNum
}