sint64 a
sint64 b = 100
uint64 a1
uint64 b1 = 100

sint32* p1
uint32* p2 = new uint32[5]

string s1
string s2 = "abc"

def string strTest0(){
    return s1
}

def string strTest1(string str){
    s1 = str
    return s1
}

def string strTest2(){
    return s2
}

def uint32 utestPtrArg(uint32 n){
   
    for uint32 i, i<5, 1{
        p2[i] = n
    }

    return p2[4]
}
def sint64 stest0(){
    return a
}

def sint64 stest100(){
    return b
}

def sint64 stest(sint64 arg){
    a = arg
    return a
}

def sint64 stest100arg(sint64 arg){
    b = arg + b
    return b
}

def uint64 utest0(){
    return a1
}

def uint64 utest100(){
    return b1
}

def uint64 utest(uint64 arg){
    a1 = arg
    return a1
}

def uint64 utest100arg(uint64 arg){
    b1 = arg + b1
    return b1
}