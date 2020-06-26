# what need to be tesed in for

# Condition ->compareBinary
# Break 
# step    positive negative
# start scope

# return a
def sint64 for_binary_pos(sint64 a){
    sint64 result = 0
    if a>=0
    then{
        for sint64 i=0 , i<a , 1
            result = result + 1
        return result 
    }  
    else{
        for sint64 i=0 , i<-a , 1
            result = result - 1
        return result 
    }  
}

def sint64 for_break1(sint64 a){
    sint64 result = 0
    if a>=0 
    then for sint64 i , i<a+1 , 1{
        if result<a
        then result = result + 1
        else break
    }
    else for sint64 i , i<-(a-1) , 1{
        if result>a
        then result = result - 1
        else break
        
    }
    return result
}

def sint64 for_binary_neg(sint64 a){
    sint64 result = a + a
    if a>=0
    then for sint64 i = a, i>0 , -1
         result = result - 1
    else for sint64 i = -a, i>0 , -1
        result = result + 1

    return result    
}

def sint64 for_break2(sint64 a){
    sint64 result = 0

    if a>0
    then for sint64 i , i<a+1 , 1{
        if result==a
        then break
        else result = result + 1
    }
    else for sint64 i , i<-(a-1) , 1{
        if result==a
        then break
        else result = result - 1
    }

    return result
    
}

def sint64 for_scope(sint64 a){
    sint64 result = a
    for sint64 i , i<5 , 1{
        result = result + i
    }

    for sint64 i , i<5 , 1{
        result = result - i
    }

    return result
}