# ret void in for loop/ in if/ in while/ at the end of function
def void retVoid1(){
    sint64 a = 0
    return void
}
#abstract value
def void retVoid2(sint32 a){
    if a < -a
    then return void
    else a = -a
}
#neg value
def void retVoid3(sint32 a){
    if a < -a
    then a = -a
    else return void
}

def void retVoid4(uint32 b){
    uint32 a = 0
    for uint32 i,i<b,1{
        a = a + i
        if(a>b)
        then return void
    }
}

def void retVoid5(uint32 b){
    uint32 a = 0
    uint32 i = 1
    while (i<b) {
        a = a + i
        if(a>b)
        then return void
        else i = i+1
    }
}

def void retVoid6(uint32 sum){

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
            return void
        }
    }
}