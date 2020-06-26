# EXPECT the return type of the function is void, cannot be the right value
def void helper(){

}

def void errorTest(){
    sint64 a = helper()
}