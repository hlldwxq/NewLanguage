#include "../header/AST.h"
#include "../header/ASTExpr.h"

const QAlloca* VariableAST::codegenLeft(){

    const QAlloca* Alloca = scope.findSymbol(name);
    if(!Alloca){
        const QGlobalVariable* global = scope.findGlobalVar(name);
        if(!global){
           // TheModule->print(outs(), nullptr);
           // printf("%s\n",name.c_str());//<<std::endl;
            error("invalid number when getting LLVM TYPE");
        }

        Alloca = new QAlloca(global->getType(),global->getGlobalVariable());

    }
    return Alloca;
}

const QAlloca* ArrayIndexExprAST::codegenLeft(){

    QValue* left = pointer->codegen();
    Value* arrIndex = index->codegen()->getValue();
	
    if(index->codegen()->getType()->isConstant()){
        
        if(dynamic_cast<ConstantType*>(index->codegen()->getType())->getValue()<0){
            error("the index of array cannot be negative");
            //return NULL;
        }
    }
    if(!left->getType()->getIsPointer()){
        error("left expression must be a pointer");
        //return NULL;
    }
    
    Value* eleptr = Builder.CreateGEP(cast<PointerType>(left->getValue()->getType()->getScalarType())->getElementType(), left->getValue(), arrIndex);
    
    PointType* pt = dynamic_cast<PointType*>(left->getType());
    QType* elementT = pt->getElementType();
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
    llvm::Value* constInt =  ConstantInt::get(qtype->getLLVMType(), value);
    return new QValue(qtype,constInt);
}

QValue* ConstantBoolAST::codegen(){
    IntType* qtype = new IntType(false,b);
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
        error("The data type after the unary operator does not meet the requirements");
        //return NULL;
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
        return op->constantCodegen(left->getValue(),right->getValue());
    }

    if(leftQV->getType()->isConstant() || rightQV->getType()->isConstant()){
        if(leftQV->getType()->isConstant()){
            IntType* rightInt = dynamic_cast<IntType*>(rightQV->getType());
            leftQV = adjustSign(leftQV,rightInt->getSigned());
            if(!leftQV){
                error("unvalid binary calculation between signed number and unsigned number");
            }
        }else{
            IntType* leftInt = dynamic_cast<IntType*>(leftQV->getType());
            rightQV = adjustSign(rightQV,leftInt->getSigned());
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

void intCastCheck(Type* qtype, unsigned length, Value* value, int line){
    
    Function *TheFunction = Builder.GetInsertBlock()->getParent();
    
    // avoid overflow when the size of target platform is same with compile platform
    unsigned long long maxSize = 2*((1UL<<(length-1))-1)+1;  //unsigned
    llvm::Value* maxInt = ConstantInt::get(qtype, maxSize); //the max int on target platform
    
    //compare
    Value* cmp = Builder.CreateICmpUGE(maxInt, value, "cmptmp");
    BasicBlock *overFlowBB = BasicBlock::Create(TheContext, "castOver", TheFunction);
	BasicBlock *notOverBB = BasicBlock::Create(TheContext, "castNormal",TheFunction);
	Builder.CreateCondBr(cmp, notOverBB, overFlowBB);

    // overflow
	Builder.SetInsertPoint(overFlowBB);
    callError(qtype,line);
	overFlowBB = Builder.GetInsertBlock(); 

    //normal
	Builder.SetInsertPoint(notOverBB);
}

QValue* NewExprAST::codegen(){
    
    // get the size of target platform
    llvm::DataLayout* dataLayOut = new llvm::DataLayout(TheModule.get());
    Type* t = dataLayOut->getLargestLegalIntType(TheContext);
    unsigned length = dataLayOut->getTypeSizeInBits(t).getFixedSize();

    //cast arraysize
    Value* arraySize = (size->codegen())->getValue();
    intCastCheck(t, length, arraySize,line);
    arraySize = Builder.CreateIntCast(arraySize,t,false); 
    

    //cast mallocsize
    Value* mallocSize = ConstantExpr::getSizeOf(type->getElementType()->getLLVMType()); //the return type of getSizeOf is i64
    intCastCheck(t, length, mallocSize,line);
    mallocSize = Builder.CreateIntCast(mallocSize,t,false); 
    

    //check overflow when mul arraysize and malloc size
    star* starOp = new star(line);
    QType* qt = new IntType(false,length);
    starOp->OverFlowCheck(new QValue(qt,arraySize),new QValue(qt,mallocSize));

    //call malloc normally
    Instruction* var_malloc = CallInst::CreateMalloc(Builder.GetInsertBlock(),t, type->getElementType()->getLLVMType(), mallocSize,arraySize,nullptr,"");
    
    Value* result = Builder.Insert(var_malloc);
    return new QValue(type,result); // FIXME Type is pointerType(type)
}

QValue* NullExprAST::codegen(){
    Value* v = NULL;
    QType* qt = NULL;
    return new QValue(qt,v);
}

QValue* CallExprAST::codegen_internal(bool is_cmd) {

    const QFunction* call = scope.getFunction(functionName);
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
  return codegen_internal(false);
}