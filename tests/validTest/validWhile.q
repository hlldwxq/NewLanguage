# what need to be tesed in while

# Condition ->compareBinary
# Break 
# step    positive negative
# start scope

# return a


def sint64 while_break1(sint64 a){
    sint64 result = 0
    sint64 i =0
    if a>=0
    then while i<a+1{
        if result<a
        then result = result + 1
        else break

        i = i + 1
    }
    else while i<-(a-1){
        if result>a
        then result = result - 1
        else break

        i = i + 1
    }
    return result
}

def sint64 while_break2(sint64 a){
    sint64 result = a 
    sint64 i = 0
    while i<a+1{
        if result==a
        then break
        else result = result + 1

        i = i + 1
    }
    return result
}

def sint64 while_scope(sint64 a){
    sint64 result = a
    sint64 i =0
    while i<a {
        result = result + i
        i = i+1
    }
    sint64 j = 0
    while j<a {
        result = result - j
        j = j+1
    }

    return result
}