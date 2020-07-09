# EXPECT the pointer has been freed
def void errorDef(){
    sint8* a = new sint8[5]
    free a
    a[0] = 1
}