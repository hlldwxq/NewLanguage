# EXPECT the pointer has not been init

def void errorDef(){
    sint8* a
    sint8* b
    a = b
    a[1] = 0
}