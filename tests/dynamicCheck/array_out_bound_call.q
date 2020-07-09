# EXPECT array out of bound

def sint8* ret(){
    sint8* array = new sint8[4]
    return array
}

def void errorDef(){
    sint8* array = ret()
    array[5] = 7
}
