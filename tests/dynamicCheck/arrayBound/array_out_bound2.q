# EXPECT array out of bound

def void errorDef(){
    uint32 size = 6
    sint8* array = new sint8[size]
    array[7] = 7
}