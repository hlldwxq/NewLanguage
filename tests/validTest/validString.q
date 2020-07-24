# test what: def /assign /default value /para /return value

def void stringDef(){
    string s = "abc"
}

def void stringAssign(){
    string s
    s = "abcdefg"
}

def void stringDefault(){
    string s
}

def void stringPara1(string s){
    string str = s
}

def void stringPara2(string s){
    string str 
    str = s
}

def string stringRet1(string s){
    string str = s
    return str
}

def string stringRet2(string s){
    string str
    str = s
    return str
}

def string stringRet3(string s){
    return s
}

def void stringHelp(string s){
    stringPara1(s)
    stringPara2(s)
    string str = stringRet1(s)
    str = stringRet2(s)
    str = stringRet3(s)
}

def void stringDiffSymbol(){
    string str
    str = "abcdefg"
    str = "1234567"
    str = "_+=@$%^&*(){}'\?></;"
    str = "g65hj# #hui7@gy_ 87^&gg11`]$"
    str = "hjhijoi900_
        ioiuiui"
    str = " j j "
}