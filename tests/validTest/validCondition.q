
# condition -> unary    andT orT    compareBinary    true false
#   unary -> !condition
#   compareBinary -> var1 op var2    var op num    num op var    num1 op num2
#   andT orT ->  condition andT|orT condition

# return the big one
def sint64 cond_gt_var(sint64 a,sint64 b){
    sint64 max
    if a > b
    then max = a
    else max = b
    return max
}

def sint64 cond_lt_var(sint64 a,sint64 b){
    sint64 max
    if b < a
    then max = a
    else max = b
    return max
}

def sint64 cond_ge_var(sint64 a,sint64 b){
    sint64 max
    if a >= b
    then max = a
    else max = b
    return max
}

def sint64 cond_le_var(sint64 a,sint64 b){
    sint64 max
    if b<=a
    then max = a
    else max = b
    return max
}


# if a==b return 1 else return 0
def sint64 cond_eq_var(sint64 a,sint64 b){
    sint64 max
    if b == a
    then max = 1
    else max = 0
    return max
}

def sint64 cond_ne_var(sint64 a,sint64 b){
    sint64 max
    if b != a
    then max = 0
    else max = 1
    return max
}

# if a==0 return 0 else return a
def sint64 cond_eq_numvar(sint64 a){
    sint64 max
    if 0 == a
    then max = 0
    else max = a
    return max
}

def sint64 cond_eq_varnum(sint64 a){
    sint64 max
    if a == 0
    then max = 0
    else max = a
    return max
}

def sint64 cond_ne_numvar(sint64 a){
    sint64 max
    if 0 != a
    then max = a
    else max = 0
    return max
}

def sint64 cond_ne_varnum(sint64 a){
    sint64 max
    if a != 0
    then max = a
    else max = 0
    return max
}
#if a>0 return a else return 0
def sint64 cond_gt_varnum(sint64 a){
    sint64 max
    if a > 0
    then max = a
    else max = 0
    return max
}
def sint64 cond_gt_numvar(sint64 a){
    sint64 max
    if 0>a
    then max = 0
    else max = a
    return max
}
def sint64 cond_lt_numvar(sint64 a){
    sint64 max
    if 0 < a
    then max = a
    else max = 0
    return max
}
def sint64 cond_lt_varnum(sint64 a){
    sint64 max
    if a<0
    then max = 0
    else max = a
    return max
}
def sint64 cond_ge_varnum(sint64 a){
    sint64 max
    if a >= 0
    then max = a
    else max = 0
    return max
}
def sint64 cond_ge_numvar(sint64 a){
    sint64 max
    if 0>=a
    then max = 0
    else max = a
    return max
}
def sint64 cond_le_numvar(sint64 a){
    sint64 max
    if 0<=a
    then max = a
    else max = 0
    return max
}
def sint64 cond_le_varnum(sint64 a){
    sint64 max
    if a<=0
    then max = 0
    else max = a
    return max
}

# return 1
def sint64 cond_true(){
    sint64 max
    if true
    then max = 1
    else max = 0
    return max
}

def sint64 cond_false(){
    sint64 max
    if false
    then max = 0
    else max = 1
    return max
}

def sint64 cond_and_truefalse(){
    sint64 max
    if true & false
    then max = 0
    else max = 1
    return max
}

def sint64 cond_and_falsetrue(){
    sint64 max
    if false & true
    then max = 0
    else max = 1
    return max
}

def sint64 cond_andtruetrue(){
    sint64 max
    if true & true
    then max = 1
    else max = 0
    return max
}

def sint64 cond_andfalsefalse(){
    sint64 max
    if false & false
    then max = 0
    else max = 1
    return max
}

def sint64 cond_ortruefalse(){
    sint64 max
    if true | false
    then max = 1
    else max = 0
    return max
}

def sint64 cond_ortruetrue(){
    sint64 max
    if true | true
    then max = 1
    else max = 0
    return max
}
def sint64 cond_orfalsetrue(){
    sint64 max
    if false | true
    then max = 1
    else max = 0
    return max
}

def sint64 cond_unary_false(){
    sint64 max
    if !(true)
    then max = 0
    else max = 1
    return max
}

def sint64 cond_unary_true(){
    sint64 max
    if !(false)
    then max = 1
    else max = 0
    return max
}