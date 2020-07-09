# EXPECT array out of bound
def void h(sint8* a){
    a[6] = 1
}
def void errorDef(){
    sint32 size = 6
    sint8* array = new sint8[size]
    h(array)
}

