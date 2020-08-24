#include "../header/AST.h"
#include "../header/ASTExpr.h"

bool checkUnignedNum(QValue* value){
    
    QType* type = value->getType();
    if(type->getIsPointer() ||type->isString()){
        return false;
    }

    if(type->isConstant()){
        ConstantType* ctype = dynamic_cast<ConstantType*>(type);
        return !(ctype->isNegative());
    }else{
        IntType* itype = dynamic_cast<IntType*>(type);
        return !(itype->getSigned());
    }
} 

const QAlloca* VariableAST::codegenLeft(){

    const QAlloca* Alloca = scope.findSymbol(name);
    if(!Alloca){
        //const QValue* arg = scope.findArg(name);
        //if(!arg){
            const QGlobalVariable* global = scope.findGlobalVar(name);
            if(!global){
                lerror("the variable has not been declared");
            }
            Alloca = new QAlloca(global->getType(),global->getGlobalVariable());
        //}

        /*llvm::AllocaInst* All = Builder.CreateAlloca(arg->getType()->getLLVMType(), ConstantInt::get(Type::getInt32Ty(TheContext), 1), name);
        Alloca = new QAlloca(arg->getType(), All);
        scope.addArg(name,Alloca);
        Builder.CreateStore(arg->getValue(), All);*/
    }

    return Alloca;
}

const QAlloca* ArrayIndexExprAST::codegenLeft(){

    QValue* left = pointer->codegen();
    QValue* arrI = index->codegen();

    if(!checkUnignedNum(arrI)){
        lerror("the array index must be unsigned number");
    }

    Value* arrIndex = arrI->getValue();

    if(!left->getType()->getIsPointer()){
        lerror("left expression must be a pointer");
    }

    PointType* pt = dynamic_cast<PointType*>(left->getType());
    if(pt->isNull()){
        lerror("the pointer has not been init");
    }
    
    Value* arrayAddress = left->getValue();
    if( doCheck[CheckLevel::check_free]){
        // check free
        arrayAddress = Builder.CreateStructGEP(left->getValue(),1);
        arrayAddress = Builder.CreateLoad(arrayAddress);
        Value* isNULL = Builder.CreateIsNull(arrayAddress);
        //isNULL = Builder.CreateICmpEQ(isNULL, ConstantInt::get(isNULL->getType(),1,false));

        createBr("the array has been free",isNULL,line, "hasFree", "noFree");
    }
    
    if(doCheck[CheckLevel::check_array_bound]){
        //check array size out of bound

        Value* arraySize = Builder.CreateStructGEP(left->getValue(),0);
        arraySize = Builder.CreateLoad(arraySize);

        arrIndex = Builder.CreateIntCast(arrIndex, arraySize->getType(),false);
        Value* cmp = Builder.CreateICmpSLE(arraySize, arrIndex);
        createBr("array out of bound", cmp,line, "outBound", "notOutBound");

        arrayAddress = Builder.CreateStructGEP(left->getValue(),1);
        arrayAddress = Builder.CreateLoad(arrayAddress);
    }

    QType* elementT = pt->getElementType();
    llvm::Value* eleptr = Builder.CreateGEP(cast<PointerType>(arrayAddress->getType()->getScalarType())->getElementType(), arrayAddress,arrIndex);   

    const QAlloca* qv = new QAlloca(elementT,eleptr);

    return qv;
 
}

QValue* VariableAST::codegen() {

    const QAlloca* Alloca = codegenLeft();
    return new QValue(Alloca->getType(),Builder.CreateLoad(Alloca->getAlloca()));
}

QValue* ArrayIndexExprAST::codegen(){
    const QAlloca* Alloca = codegenLeft();
    return new QValue(Alloca->getType(),Builder.CreateLoad(Alloca->getAlloca()));
}

QValue* NumberExprAST::codegen(){
    ConstantType* qtype = new ConstantType(value,line);
    llvm::Value* constInt = ConstantInt::get(TheContext,qtype->getValue().getValue());
    return new QValue(qtype,constInt);
}

QValue* StringExprAST::codegen(){
    Value* stringExpr = scope.findStr(str);
    if(stringExpr == NULL){
        stringExpr = Builder.CreateGlobalStringPtr(str);
        scope.addStr(str,stringExpr);
    }
    return new QValue(new StringType(), stringExpr);
}

QValue* ConstantBoolAST::codegen(){
    IntType* qtype = new IntType(false,1);
    llvm::Value* constInt = ConstantInt::get(qtype->getLLVMType(), b);
    return new QValue(qtype,constInt);
}

QValue* UnaryExprAST::codegen(){

    QValue* value = Operand->codegen();
    if(!value){
        return NULL;
    }
    QValue* result = opCode->codegen(value);
    if(!result){
        lerror("The data type after the unary operator nust be bool");
    }
    return result;
}

QValue* BinaryExprAST::codegen(){

    QValue* leftQV = LHS->codegen();
    QValue* rightQV = RHS->codegen();

    if(leftQV->getType()->getIsPointer() || rightQV->getType()->getIsPointer()){
        lerror("The operands of a binary operator cannot be a pointer");
    }

    if(leftQV->getType()->isConstant() && rightQV->getType()->isConstant()){
        ConstantType* left = dynamic_cast<ConstantType*>(leftQV->getType());
        ConstantType* right = dynamic_cast<ConstantType*>(rightQV->getType());
        return op->constantCodegen(left->getValue(),right->getValue());
    }

    if(rightQV->getType()->compare(new IntType(false,1)) && !leftQV->getType()->compare(new IntType(false,1))){
        lerror("the boolean(uint1) cannot do binary calculation with int");
    }
    if(!rightQV->getType()->compare(new IntType(false,1)) && leftQV->getType()->compare(new IntType(false,1))){
        lerror("the boolean(uint1) cannot do binary calculation with int");
    }

    if(leftQV->getType()->isConstant() || rightQV->getType()->isConstant()){
        if(leftQV->getType()->isConstant()){
            
            IntType* rightInt = dynamic_cast<IntType*>(rightQV->getType());
            leftQV = constAdjustSign(leftQV,rightInt->getSigned()); 
            //the initial sign of 0 or positive constant number is unsigned, negative num is signed
            //now we could based on the context to decide the sign of num 
            if(!leftQV){
                lerror("unvalid binary calculation between signed number and unsigned number");
            }
        }else{
            
            IntType* leftInt = dynamic_cast<IntType*>(leftQV->getType());
            rightQV = constAdjustSign(rightQV,leftInt->getSigned());
            if(!rightQV){
                lerror("unvalid binary calculation between signed number and unsigned number");
            }
        }
    }
    
    IntType* leftInt = dynamic_cast<IntType*>(leftQV->getType());
    IntType* rightInt = dynamic_cast<IntType*>(rightQV->getType());

    if(leftInt->getSigned()!=rightInt->getSigned()){
        lerror("invalid binary calculation between signed number and unsigned number");
    }

    if(leftInt->getWidth()!=rightInt->getWidth()){

        unsigned long long widthL = leftInt->getWidth();
        unsigned long long widthR = rightInt->getWidth();

        if(widthL>widthR){
            rightQV = upCast(rightQV,leftInt);
        }else{
            leftQV = upCast(leftQV,rightInt);
        }
    }

    return op->codegen(leftQV,rightQV);
    
}

//just for new expr
void intCastCheck(Type* qtype, Value* value, int line){
    
    unsigned length = qtype->getIntegerBitWidth();
    llvm::Type* vtype = value->getType();
    if (length >= vtype->getIntegerBitWidth()) {
        return;
    }

    assert(sizeof(unsigned long long)*8 >= length);    
    llvm::Value* maxInt = Builder.CreateIntCast(ConstantInt::get(TheContext, APInt::getMaxValue(length)),vtype,false);
    
    //compare
    Value* cmp = Builder.CreateICmpULT(maxInt, value, "cmptmp");
    createBr("overflow when casting the array size or malloc size in new expression",cmp,line,"newOverF","newNotOverF");
    
}

QValue* NewExprAST::codegen(){

    // get the size of target platform
    unsigned length = sizet->getIntegerBitWidth();
    if(length==0)
        Bug("does not get datalayout",line);

    QValue* sizeValue = size->codegen();

    // sizecheck size >= 0
    if(!checkUnignedNum(sizeValue)){
        lerror("the size of array must be unsigned number");
    }
 
    Value* arraySize = sizeValue->getValue();


    //cast arraysize
    intCastCheck(sizet, arraySize,line);
    arraySize = Builder.CreateIntCast(arraySize,sizet,false); 


    //cast mallocsize
    Value* mallocSize = ConstantExpr::getSizeOf(type->getElementType()->getLLVMType()); //the return type of getSizeOf is i64
    intCastCheck(sizet, mallocSize,line);
    mallocSize = Builder.CreateIntCast(mallocSize,sizet,false); 


    //check overflow when mul arraysize and malloc size
    star* starOp = new star(line);
    QType* qt = new IntType(false,length);
    Value* mulResult = starOp->OverFlowCheck(new QValue(qt,arraySize),new QValue(qt,mallocSize),"newMulOverFlow","newMulNormal");

    Instruction* malloc_inst = CallInst::CreateMalloc(Builder.GetInsertBlock(),sizet,type->getElementType()->getLLVMType(),mulResult,nullptr,nullptr,"");
    Value* malloc_value = Builder.Insert(malloc_inst);

    // new code
    if(doCheck[CheckLevel::check_array_bound] || doCheck[CheckLevel::check_free]){
        assert(type->getStructType()!=nullptr);
        Value* struct_size = ConstantExpr::getSizeOf(type->getStructType());

        // call malloc normally
        Instruction* struct_malloc = CallInst::CreateMalloc(Builder.GetInsertBlock(),sizet,type->getStructType(),struct_size,nullptr,nullptr,"");
        Value* result = Builder.Insert(struct_malloc);

        Value* sizeAddress = Builder.CreateStructGEP(result,0);
        Builder.CreateStore(arraySize,sizeAddress);

        Value* arrayAddress = Builder.CreateStructGEP(result,1);
        Builder.CreateStore(malloc_value,arrayAddress);

        return new QValue(type,result); 
    }else{
        return new QValue(type,malloc_value);
    }

}

QValue* NullExprAST::codegen(){
    Value* v = NULL;
    QType* qt = NULL;
    return new QValue(qt,v);
}

QValue* CallExprAST::codegen_internal(bool is_cmd) {

    const QFunction* call = scope.getFunction(functionName);
    if(!call){
        call = scope.getFunctionProto(functionName);
        if(!call)
            ExprAST::lerror("the function has not been declared");
    }
    Function* func = call->getFunction();
    std::vector<QType*> argsType = call->getArgsType();

    ReturnType* returnType = call->getReturnType();
    if(!is_cmd && returnType->getIsVoid()){
        ExprAST::lerror("the return type of the function is void, cannot be the right value");
    }

    if(func->arg_size()!=args.size()){
        ExprAST::lerror("The number of participating arguments does not match");
    }
    
    std::vector<Value*> ArgsV;

    for(int i=0 ; i<args.size(); i++){

        QType* needArgType = argsType[i];
        QValue* realArg = args[i]->codegen();
        realArg = assignCast(realArg,needArgType);

        if(!realArg){
            ExprAST::lerror("type cannot be converted");
        }
        ArgsV.push_back(realArg->getValue());
    }

    if (is_cmd) {
      Builder.CreateCall(func, ArgsV);
      return NULL;
    } else {
        return new QValue(returnType->getType(),Builder.CreateCall(func, ArgsV, "calltmp"));
    }

}

QValue* CallExprAST::codegen(){
    QValue* qv = codegen_internal(false);
    return qv;
}
