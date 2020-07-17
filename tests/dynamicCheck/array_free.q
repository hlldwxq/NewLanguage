# EXPECT the array has been free
def void errorDef(){
    sint8* a = new sint8[5]
    free a
    a[0] = 1
}