def sint8* helper(){
    return new sint8[3]
}

def void test(){
    sint8* a = helper()
    a[0] = 1
}