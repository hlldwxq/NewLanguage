# what need to be tesed in if

# Then -> ret   no ret   ret      no ret    
# Else -> ret   ret      no ret   no ret

#return max one
def sint64 if_2ret(sint64 a,sint64 b){
    if  a > b
    then return a
    else return b
}

def sint64 if_then_ret(sint64 a,sint64 b){
    sint64 max
    if b<a
    then max = a
    else return b

    return max
}

def sint64 if_else_ret(sint64 a,sint64 b){
    sint64 max
    if b > a
    then return b
    else max = a
    return max
}

def sint64 if_0ret(sint64 a,sint64 b){
    sint64 max
    if b >= a
    then max = b
    else max = a
    return max
}