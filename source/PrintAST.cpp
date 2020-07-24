#include "../header/AST.h"
#include "../header/ASTExpr.h"
#include "../header/ASTCommand.h"
#include "../header/ASTStructure.h"
#include "../header/Operator.h"
void ReturnType::printAST() const{
    if(isVoid)
        printf("void");
    else{
        qType->printAST();
    }
}

void IntType::printAST() const{
    if(isSigned){
        printf("signed ");
    }else{
        printf("unsigned ");
    }

    printf("int%lld",width);
}

void PointType::printAST() const{
    const QType* p = elementType;
    printf("*");
    p->printAST();
}

void ConstantType::printAST() const{
    printf("%s", value.getValue().toString(10,true).c_str()); // TODO: implement!
}

void StringType::printAST() const{
    printf("string");
}
void exclamation::printAST(){
    printf("!");
}

void negative::printAST(){
    printf("-");
}

void equal_sign::printAST(){
    printf("==");
}

void not_equal::printAST(){
    printf("!=");
}

void less_equal::printAST(){
    printf("<=");
}

void greater_equal::printAST(){
    printf(">=");
}

void greater_than::printAST(){
    printf(">");
}

void less_than::printAST(){
    printf("<");
}

void plus::printAST(){
    printf("+");
}

void minus::printAST(){
    printf("-");
}

void star::printAST(){
    printf("*");
}

void division::printAST(){
    printf("/");
}

void andT::printAST(){
    printf("&");
}

void orT::printAST(){
    printf("|");
}

void VariableAST::printAST(int level){
    for(int i=0;i<level;i++){
        printf(" ");
    }
    if(isPointer){
        printf("Pointer Name: ");
    }else{
        printf("Variable Name: ");
    }

    printf("%s",name.c_str());
}

void ArrayIndexExprAST::printAST(int level){
    for(int i=0;i<level;i++){
        printf(" ");
    }
    printf("array index: \n");
    for(int i=0;i<level;i++){
        printf(" ");
    }
    printf("  left: \n");
    pointer->printAST(level+4);
    printf("\n");

    for(int i=0;i<level;i++){
        printf(" ");
    }
    printf("  index: \n");
    index->printAST(level+4);
}

void NumberExprAST::printAST(int level){
    for(int i=0;i<level;i++){
        printf(" ");
    }
    printf("%s",value.c_str());
}

void StringExprAST::printAST(int level){
    for(int i=0;i<level;i++){
        printf(" ");
    }
    printf("%s",str.c_str());
}

void ConstantBoolAST::printAST(int level){
    if(b==true)
        printf("true");
    else 
        printf("false");
}

void NullExprAST::printAST(int level) {
    for(int i=0;i<level;i++){
        printf(" ");
    }
    printf("null pointer");
}

void CallExprAST::printAST(int level){

    for(int i=0;i<level;i++){
        printf(" ");
    }
    printf("function call:\n");
    
    for(int i=0;i<level;i++){
        printf(" ");
    }
    printf("  name: %s\n",functionName.c_str());
    
    for(int i=0;i<level;i++){
        printf(" ");
    }
    printf("  args: \n");
    
    for(int i=0;i<args.size();i++){
        for(int j=0;j<level;j++){
            printf(" ");
        }
        printf("  arg%d: \n",i+1);
        args[i]->printAST(level+4);
        printf("\n");
    }

}

void UnaryExprAST::printAST(int level){

    printf("unary expr:\n"); 
    
    for(int i=0;i<level;i++){
        printf(" ");
    }
    printf("  operator: ");
    opCode->printAST();
    printf("\n");
    
    for(int i=0;i<level;i++){
        printf(" ");
    }
    printf("  operand: \n");
    Operand->printAST(level+4);
}

void BinaryExprAST::printAST(int level){
    for(int i=0;i<level;i++){
        printf(" ");
    }
    printf("binary expr: \n");

    for(int i=0;i<level;i++){
        printf(" ");
    }
    printf("  left:\n");
    LHS->printAST(level+4);
    printf("\n");

    for(int i=0;i<level;i++){
        printf(" ");
    }
    printf("  operator: ");
    op->printAST();
    printf("\n");

    for(int i=0;i<level;i++){
        printf(" ");
    }
    printf("  right:\n");
    RHS->printAST(level+4);
}

void NewExprAST::printAST(int level){
    for(int i=0;i<level;i++){
            printf(" ");
        }
    printf("New expr:\n");
    for(int i=0;i<level;i++){
            printf(" ");
        }
    printf("  type: ");
    type->printAST();
    printf("\n");
    for(int i=0;i<level;i++){
        printf(" ");
    }
    printf("sizes:\n");
    size->printAST(level+4);
    printf("\n");
}

void VarDefAST::printAST(int level){
    for(int i=0;i<level;i++){
        printf(" ");
    }
    printf("Variable def:\n");

    for(int i=0;i<level;i++){
        printf(" ");
    }
    printf("  type: ");
    type->printAST();
    printf("\n");
    for(int i=0;i<level;i++){
        printf(" ");
    }
    printf("  name: %s \n",name.c_str());
    for(int i=0;i<level;i++){
        printf(" ");
    }
    printf("  value: \n");
    value->printAST(level+4);

}

void StrDefAST::printAST(int level){
    for(int i=0;i<level;i++){
        printf(" ");
    }
    printf("String def:\n");

    for(int i=0;i<level;i++){
        printf(" ");
    }
    printf("  type: ");
    type->printAST();
    printf("\n");
    for(int i=0;i<level;i++){
        printf(" ");
    }
    printf("  name: %s \n",name.c_str());
    for(int i=0;i<level;i++){
        printf(" ");
    }
    printf("  value: \n");
    value->printAST(level+4);

}

void ArrayDefAST::printAST(int level){
    for(int i=0;i<level;i++){
        printf(" ");
    }
    printf("Array def:\n");
    for(int i=0;i<level;i++){
        printf(" ");
    }
    printf("  type: ");
    type->printAST();
    printf("\n");
    for(int i=0;i<level;i++){
        printf(" ");
    }
    printf("  name: %s\n",name.c_str());
    for(int i=0;i<level;i++){
        printf(" ");
    }
    printf("  value: \n");
    value->printAST(level+4);
}

void AssignAST::printAST(int level){
    for(int i=0;i<level;i++){
        printf(" ");
    }
    printf("Assign: \n");
    for(int i=0;i<level;i++){
        printf(" ");
    }
    printf("  left:\n");
    left->printAST(level+4);
    printf("\n");
    for(int i=0;i<level;i++){
        printf(" ");
    }
    printf("  right:\n");
    right->printAST(level+4);
}

void IfAST::printAST(int level){
    for(int i=0;i<level;i++){
        printf(" ");
    }
    printf("If:\n");

    for(int i=0;i<level;i++){
        printf(" ");
    }
    printf("  condition:\n");
    condition->printAST(level+4);
    printf("\n");

    for(int i=0;i<level;i++){
        printf(" ");
    }
    printf("  then:\n");
    thenC->printAST(level+4);
    printf("\n");

    for(int i=0;i<level;i++){
        printf(" ");
    }
    printf("  else\n");
    elseC->printAST(level+4);
    printf("\n");
}

void ForAST::printAST(int level){
    for(int i=0;i<level;i++){
        printf(" ");
    }
    printf("For:\n");

    for(int i=0;i<level;i++){
        printf(" ");
    }
    printf("  start:\n");
    start->printAST(level+4);
    printf("\n");

    for(int i=0;i<level;i++){
        printf(" ");
    }
    printf("  condition:\n");
    condition->printAST(level+4);
    printf("\n");
    for(int i=0;i<level;i++){
        printf(" ");
    }
    printf("  step: ");
    step->printAST();
    printf("\n");
    for(int i=0;i<level;i++){
        printf(" ");
    }
    printf("  for body:\n");
    body->printAST(level+4);
}

void WhileAST::printAST(int level){
    for(int i=0;i<level;i++){
        printf(" ");
    }
    printf("While:\n");

    for(int i=0;i<level;i++){
        printf(" ");
    }
    printf("  condition:");
    condition->printAST(level+4);
    printf("\n");

    for(int i=0;i<level;i++){
        printf(" ");
    }
    printf("  body:");
    body->printAST(level+4);
}

void ReturnAST::printAST(int level){
    for(int i=0;i<level;i++){
        printf(" ");
    }
    printf("Return: \n");
    value->printAST(level+4);
}

void BreakAST::printAST(int level){
    for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("Break");
}

void BlockAST::printAST(int level){
    for(int i=0;i<level;i++){
        printf(" ");
    }
    if(cmds.size()==0){
        printf("Block: empty\n");
    }else{
        printf("Block: \n");
        for(int i =0;i<cmds.size();i++){
            cmds[i]->printAST(level+4);
            printf("\n");
        }
    }
}

void PrototypeAST::printAST(int level){
    for(int i=0;i<level;i++){
        printf(" ");
    }
    printf("Prototype:\n");
    for(int i=0;i<level;i++){
        printf(" ");
    }
    printf("  returnType: ");  
    returnType->printAST();
    printf("\n");
    for(int i=0;i<level;i++){
        printf(" ");
    }
    printf("  name: %s\n",Name.c_str());
    for(int i=0;i<level;i++){
        printf(" ");
    }
    printf("  args:\n");
    for(int i=0;i<Args.size();i++){
        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("    arg%d :    name: %s\n",i,Args[i].first.c_str());
        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("              type: ");
        Args[i].second->printAST();
        printf("\n");
    }
}

void FunctionAST::printAST(int level){
    printf("function: \n");
    Proto->printAST(level+4);
    Body->printAST(level+4);
}
