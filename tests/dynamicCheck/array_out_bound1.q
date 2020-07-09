# EXPECT array out of bound

def void errorDef(){
    sint8* array = new sint8[4]
    array[5] = 7
}