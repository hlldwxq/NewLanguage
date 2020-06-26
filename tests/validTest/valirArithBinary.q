# + - * /  & |
# constant  var  call  arrayindex  

#helper function
def sint64 call1(sint64 a){
    return a
}

def sint64 plusNum200(){
    sint64 a = 100 + 100
    return a
}
def sint64 plusVar(sint64 a,sint64 b){
    sint64 x = a+b
    return x
}
def sint64 plusCall(sint64 a,sint64 b){
    sint64 x = call1(a) + call1(b)
    return x
}
def sint64 plusArrIndex(sint64 a,sint64 b){
    sint64* a1 = new sint64[2]
    a1[0] = a 
    a1[1] = b
    sint64 x = a1[0]+a1[1]
    return x
}

def sint64 minusNum0(){
    sint64 a = 100 - 100
    return a
}
def sint64 minusVar(sint64 a,sint64 b){
    sint64 x = a-b
    return x
}
def sint64 minusCall(sint64 a,sint64 b){
    sint64 x = call1(a)-call1(b)
    return x
}
def sint64 minusArrIndex(sint64 a,sint64 b){
    sint64* a1 = new sint64[2]
    a1[0] = a 
    a1[1] = b
    sint64 x = a1[0]-a1[1]
    return x
}

def sint64 starNum100(){
    sint64 a = 10*10
    return a
}
def sint64 starVar(sint64 a,sint64 b){
    sint64 x = a*b
    return x
}
def sint64 starCall(sint64 a,sint64 b){
    sint64 x = call1(a)*call1(b)
    return x
}
def sint64 starArrIndex(sint64 a,sint64 b){
    sint64* a1 = new sint64[2]
    a1[0] = a 
    a1[1] = b
    sint64 x = a1[0]*a1[1]
    return x
}

def sint64 divNum1(){
    sint64 a = 100 / 100
    return a
}
def sint64 divVar(sint64 a,sint64 b){
    sint64 x = a/b
    return x
}
def sint64 divCall(sint64 a,sint64 b){
    sint64 x = call1(a) / call1(b)
    return x
}
def sint64 divArrIndex(sint64 a,sint64 b){
    sint64* a1 = new sint64[2]
    a1[0] = a 
    a1[1] = b
    sint64 x = a1[0]/a1[1]
    return x
}

def sint64 andNum4(){
    sint64 a = 4 & 4
    return a
}
def sint64 andVar(sint64 a,sint64 b){
    sint64 x = a&b
    return x
}
def sint64 andCall(sint64 a,sint64 b){
    sint64 x = call1(a) & call1(b)
    return x
}
def sint64 andArrIndex(sint64 a,sint64 b){
    sint64* a1 = new sint64[2]
    a1[0] = a 
    a1[1] = b
    sint64 x = a1[0]&a1[1]
    return x
}

def sint64 orNum4(){
    sint64 a = 4 | 4
    return a
}
def sint64 orVar(sint64 a,sint64 b){
    sint64 x = a|b
    return x
}
def sint64 orCall(sint64 a,sint64 b){
    sint64 x = call1(a) | call1(b)
    return x
}
def sint64 orArrIndex(sint64 a,sint64 b){
    sint64* a1 = new sint64[2]
    a1[0] = a 
    a1[1] = b
    sint64 x = a1[0]|a1[1]
    return x
}
