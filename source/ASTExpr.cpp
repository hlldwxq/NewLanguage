#include "../header/AST.h"
#include "../header/ASTExpr.h"

const QAlloca* VariableAST::codegenLeft(){

    const QAlloca* Alloca = scope.findSymbol(name);
    if(!Alloca){
        const QGlobalVariable* global = scope.findGlobalVar(name);
        if(!global){
            error("the variable has not been declared");
        }

        Alloca = new QAlloca(global->getType(),global->getGlobalVariable());

    }

    return Alloca;
}

const QAlloca* ArrayIndexExprAST::codegenLeft(){

    QValue* left = pointer->codegen();
    QValue* arrI = index->codegen();
    Value* arrIndex = arrI->getValue();
	
    if(arrI->getType()->isConstant()){
        std::string ind = dynamic_cast<ConstantType*>(arrI->getType())->getValue();
        if(ind[0]=='-'){
            error("the index of array cannot be negative");
        }
    }

    if(!left->getType()->getIsPointer()){
        error("left expression must be a pointer");
    }

    PointType* pt = dynamic_cast<PointType*>(left->getType());
    if(pt->isNull()){
        error("the pointer has not been init");
    }
    
    if(doCheck){
        Function *TheFunction = Builder.GetInsertBlock()->getParent();
        Value* arraySize = Builder.CreateStructGEP(left->getValue(),0);
        arraySize = Builder.CreateLoad(arraySize);

        Value* zero = ConstantInt::get(sizet,0,true);
        Value* cmp1 = Builder.CreateICmpSGE(zero, arraySize);

        BasicBlock *freeBB = BasicBlock::Create(TheContext,"free", TheFunction);
        BasicBlock *noFreeBB = BasicBlock::Create(TheContext,"noFree", TheFunction);
        
        Builder.CreateCondBr(cmp1, freeBB, noFreeBB);
        Builder.SetInsertPoint(freeBB);
        callError("the pointer has been freed",line);
        freeBB = Builder.GetInsertBlock();  

        // normal
        Builder.SetInsertPoint(noFreeBB);

        arrIndex =Builder.CreateIntCast(arrIndex, arraySize->getType(),false);
        Value* cmp = Builder.CreateICmpSGT(arraySize, arrIndex);

        
        BasicBlock *OutBoundBB = BasicBlock::Create(TheContext,"outBound", TheFunction);
        BasicBlock *NormalBB = BasicBlock::Create(TheContext,"normal", TheFunction);
        
        Builder.CreateCondBr(cmp, NormalBB, OutBoundBB);
        Builder.SetInsertPoint(OutBoundBB);
        callError("array out of bound",line);
        OutBoundBB = Builder.GetInsertBlock();  

        // normal
        Builder.SetInsertPoint(NormalBB);

    }

    QType* elementT = pt->getElementType();
    Value* arrayAddress = Builder.CreateStructGEP(left->getValue(),1);
    arrayAddress = Builder.CreateLoad(arrayAddress);
    llvm::Value* eleptr = Builder.CreateGEP(elementT->getLLVMType(), arrayAddress,arrIndex);   
    const QAlloca* qv = new QAlloca(elementT,eleptr);

    return qv;
 
}

QValue* LeftValueAST::codegen() {

    const QAlloca* Alloca = codegenLeft();
    if(!Alloca){
        return NULL;
    }
    return new QValue(Alloca->getType(),Builder.CreateLoad(Alloca->getAlloca()));
}

QValue* NumberExprAST::codegen(){
    ConstantType* qtype = new ConstantType(value);
    llvm::Value* constInt = ConstantInt::get(TheContext,qtype->getApValue());
    return new QValue(qtype,constInt);
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
        error("The data type after the unary operator nust be bool");
    }
    return result;
}

QValue* BinaryExprAST::codegen(){

    QValue* leftQV = LHS->codegen();
    QValue* rightQV = RHS->codegen();

    if(leftQV->getType()->getIsPointer() || rightQV->getType()->getIsPointer()){
        error("The operands of a binary operator cannot be a pointer");
    }

    if(leftQV->getType()->isConstant() && rightQV->getType()->isConstant()){
        ConstantType* left = dynamic_cast<ConstantType*>(leftQV->getType());
        ConstantType* right = dynamic_cast<ConstantType*>(rightQV->getType());
        return op->constantCodegen(left->getApValue(),right->getApValue());
    }

    if(rightQV->getType()->compare(new IntType(false,1)) && !leftQV->getType()->compare(new IntType(false,1))){
        error("the boolean(uint1) cannot do binary calculation with int");
    }
    if(!rightQV->getType()->compare(new IntType(false,1)) && leftQV->getType()->compare(new IntType(false,1))){
        error("the boolean(uint1) cannot do binary calculation with int");
    }

    if(leftQV->getType()->isConstant() || rightQV->getType()->isConstant()){
        if(leftQV->getType()->isConstant()){
            
            IntType* rightInt = dynamic_cast<IntType*>(rightQV->getType());
            leftQV = constAdjustSign(leftQV,rightInt->getSigned()); 
            //the initial sign of 0 or positive constant number is unsigned, negative num is signed
            //now we could based on the context to decide the sign of num 
            if(!leftQV){
                error("unvalid binary calculation between signed number and unsigned number");
            }
        }else{
            
            IntType* leftInt = dynamic_cast<IntType*>(leftQV->getType());
            rightQV = constAdjustSign(rightQV,leftInt->getSigned());
            if(!rightQV){
                error("unvalid binary calculation between signed number and unsigned number");
            }
        }
    }
    
    IntType* leftInt = dynamic_cast<IntType*>(leftQV->getType());
    IntType* rightInt = dynamic_cast<IntType*>(rightQV->getType());

    if(leftInt->getSigned()!=rightInt->getSigned()){
        error("invalid binary calculation between signed number and unsigned number");
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

    Function *TheFunction = Builder.GetInsertBlock()->getParent();
    
    // avoid overflow when the size of target platform is same with compile platform
    //unsigned long long maxSize = 2*((1ULL<<(length-1))-1)+1;  //unsigned
    assert(sizeof(unsigned long long)*8 >= length);    
    llvm::Value* maxInt = Builder.CreateIntCast(ConstantInt::get(TheContext, APInt::getMaxValue(length)),vtype,false);
    
    //compare
    Value* cmp = Builder.CreateICmpUGE(maxInt, value, "cmptmp");
    BasicBlock *overFlowBB = BasicBlock::Create(TheContext, "castOver", TheFunction);
	BasicBlock *notOverBB = BasicBlock::Create(TheContext, "castNormal",TheFunction);
	Builder.CreateCondBr(cmp, notOverBB, overFlowBB);

    // overflow
	Builder.SetInsertPoint(overFlowBB);
    callError("overflow when casting the array size or malloc size in new expression",line);
	overFlowBB = Builder.GetInsertBlock(); 

    //normal
	Builder.SetInsertPoint(notOverBB);
}

QValue* NewExprAST::codegen(){
    
    // get the size of target platform
    unsigned length = sizet->getIntegerBitWidth();

    if(length==0)
        Bug("does not get datalayout",line);

    Value* arraySize = (size->codegen())->getValue();
    QType* sizeType = (size->codegen())->getType();

    if(sizeType->getIsPointer()){      //check if arraysize is integer 
        error("the array size must be a integer");
    }

    if(doCheck){        //check array size > 0
        Function *TheFunction = Builder.GetInsertBlock()->getParent();

        Value* zero = ConstantInt::get(sizeType->getLLVMType(),0,true);
        Value* cmp = Builder.CreateICmpSLT(zero, arraySize);

        BasicBlock *negSizeBB = BasicBlock::Create(TheContext,"negArrSize", TheFunction);
        BasicBlock *normalBB = BasicBlock::Create(TheContext,"normal", TheFunction);
        
        Builder.CreateCondBr(cmp, normalBB, negSizeBB);
        Builder.SetInsertPoint(negSizeBB);
        callError("the size of array is negative or zero",line);
        negSizeBB = Builder.GetInsertBlock();  

        // normal
        Builder.SetInsertPoint(normalBB);
    }

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
    Value* mulResult = starOp->OverFlowCheck(new QValue(qt,arraySize),new QValue(qt,mallocSize));

    Instruction* var_malloc = CallInst::CreateMalloc(Builder.GetInsertBlock(),sizet,type->getElementType()->getLLVMType(),mulResult,nullptr,nullptr,"");
    Value* record = Builder.Insert(var_malloc);

    // new code
    Value* space = ConstantExpr::getSizeOf(type->getStructType());

    // call malloc normally
    Instruction* struct_malloc = CallInst::CreateMalloc(Builder.GetInsertBlock(),sizet,type->getStructType(),space,nullptr,nullptr,"");
    Value* result = Builder.Insert(struct_malloc);
    //llvm::AllocaInst* result = Builder.CreateAlloca(type->getStructType());

    // assign
    Value* sizeAddress = Builder.CreateStructGEP(result,0);
    Builder.CreateStore(arraySize,sizeAddress);

    Value* arrayAddress = Builder.CreateStructGEP(result,1);
    Builder.CreateStore(record,arrayAddress);
    
    return new QValue(type,result); 
}

QValue* NullExprAST::codegen(){
    Value* v = NULL;
    QType* qt = NULL;
    return new QValue(qt,v);
}

QValue* CallExprAST::codegen_internal(bool is_cmd) {

    const QFunction* call = scope.getFunction(functionName);
    if(!call){
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
