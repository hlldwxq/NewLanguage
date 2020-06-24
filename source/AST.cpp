#include "../header/AST.h"
#include "../header/Scope.h"
#include <assert.h>

//the definition of global variable is at the end of the file
IntType IntType::bool_type = IntType(false,1);
LLVMContext TheContext;
IRBuilder<> Builder(TheContext);
std::unique_ptr<Module> TheModule;
std::unique_ptr<TargetMachine> TM;
bool doCheck;
Scope<QAlloca,QFunction,QGlobalVariable,ReturnType> scope;

[[noreturn]] void error(std::string msg) {
    throw error_e(msg);
}

std::unique_ptr<Module>& getModule(){
    return TheModule;
}

void initModule(std::string fileName){
    InitializeNativeTarget();
    InitializeNativeTargetAsmPrinter();
    InitializeNativeTargetAsmParser();
    TheModule = std::make_unique<Module>(fileName, TheContext); 
    TM =std::unique_ptr<TargetMachine>(EngineBuilder().selectTarget());
    TheModule->setDataLayout(TM->createDataLayout());
}

void initCheck(std::string check){
    if(check == "DyCheck"){
        doCheck = true;
    }
    else if(check == "notDyCheck"){
        doCheck = false;
    }else{
        printf("need DyCheck or notDyCheck\n");
        exit(1);
    }
}

void CallInitFunction(){
    std::vector<const QFunction*> funs = scope.getInitFunction();
    if(funs.size()==0)
        return;
    std::vector<llvm::Type*> args;
    
    llvm::Type* returnT = llvm::Type::getVoidTy(TheContext);
    FunctionType *FT = FunctionType::get(returnT,args,false);
    Function *F = Function::Create(FT,Function::InternalLinkage, "init_all_global_array", TheModule.get());

    BasicBlock *BB = BasicBlock::Create(TheContext, "entry", F);
    Builder.SetInsertPoint(BB);

    for(int i = 0;i<funs.size();i++){
        Function* fun = funs[i]->getFunction();
        std::vector<llvm::Value*> arg;
        Builder.CreateCall(fun, arg);
    }
    Builder.CreateRetVoid();

    llvm::appendToGlobalCtors(*TheModule.get(), F, 1); 
}

//used when constant num is a operand of binaty operator and anther operand is not contant
//I hope to make the type of constant num becomes IntType
int getSuitableWidth(long long num, bool isSigned){
    int initSize = 8;
    if(isSigned){
        while(initSize<=128){
            long long maxRange = 2*((1L<<(initSize-2))-1)+1;                      
            long long minRange = -(1L<<(initSize-1));

            if(num<=maxRange && num>=minRange){
                break;
            }
            initSize*=2;
        }
    }else{
        while(initSize<=128){
            unsigned long long maxRange = 2*((1UL<<(initSize-1))-1)+1;
            long long minRange = 0;
            if(num<=maxRange && num>=minRange){
                break;
            }
            initSize*=2;
        }
    }

    if(initSize>128)
        return 0;
    else
        return initSize;

}

//only be used when the constant number is the value of global var 
llvm::Constant* alignConst(long long value, QType* type){

    if(type->getIsPointer())
        return NULL;

    IntType* intT = dynamic_cast<IntType*>(type);
    long long maxRange;
    long long minRange;
    long long initSize = intT->getWidth();
    if(intT->getSigned()){
        maxRange = (2L<<(initSize-2))-1;
        minRange = -(2L<<(initSize-2));
    }else{
        maxRange = (2L<<(initSize-1))-1;  // Notice the range of Numbers on the computer
        minRange = 0; 
    }

    if(value<minRange || value>maxRange){
        return NULL;
    }

    return ConstantInt::get(intT->getLLVMType(),value);
}

// when the constant number is signed(default), but another operand of leftValue is unsigned
QValue* adjustSign(QValue* num, bool isSigned){
    ConstantType* type = dynamic_cast<ConstantType*>(num->getType());
    if(isSigned==false){
        if(type->getValue()<0){
            return NULL;
        }
    }
    int width = getSuitableWidth(type->getValue(),isSigned);
    if(width==0){
        return NULL;
    }
    IntType* t = new IntType(isSigned,width);

    return new QValue(t,ConstantInt::get(t->getLLVMType(),type->getValue()));
}

QValue* upCast(QValue* qv, IntType* type){
    llvm::Value* newQv = Builder.CreateIntCast(qv->getValue(),type->getLLVMType(),type->getSigned());
    IntType* newType = new IntType(type->getSigned(),type->getWidth());
    return new QValue(newType,newQv);
}

// for defAST, assignAST, returnAST, call argument
QValue* assignCast(QValue* varValue, QType* leftT){

    if(varValue->getType()!=NULL && leftT->getIsPointer() != varValue->getType()->getIsPointer()){
        return NULL;
    }
    // variable or arrayIndex
    if(!leftT->getIsPointer()){
        if(varValue->getType()==NULL || varValue->getValue()==NULL){ 
            return NULL;
        }
        if(varValue->getType()->isConstant()){
            varValue = adjustSign(varValue,(dynamic_cast<IntType*>(leftT))->getSigned());
            
            if(!varValue){
                return NULL;
            }
        }
        IntType* leftInt = dynamic_cast<IntType*>(leftT);
        IntType* rightInt = dynamic_cast<IntType*>(varValue->getType());

        if(leftInt->getSigned() != rightInt->getSigned()){
            return NULL;
        }
        if(leftInt->getWidth()< rightInt->getWidth()){
            return NULL;
        }
        if(leftInt->getWidth() > rightInt->getWidth()){
            varValue = upCast(varValue,leftInt);
        }        
    }else{
        if(varValue->getType()==NULL && varValue->getValue()==NULL){ //null
            varValue = new QValue(leftT, Constant::getNullValue(leftT->getLLVMType()));
        }
        else{
            PointType* leftP = dynamic_cast<PointType*>(leftT);
            PointType* rightP = dynamic_cast<PointType*>(varValue->getType());
            if(!(rightP->isNull()) && !(leftP->compare(rightP))){
                return NULL;
            }
        }
    }
    return varValue;
}

Constant* geti8StrVal(char const* str, Type* t, Twine const& name) {
  
    Constant* strConstant = ConstantDataArray::getString(TheContext, str);
    GlobalVariable* GVStr = new GlobalVariable(*(TheModule.get()), strConstant->getType(), true,
                            GlobalValue::InternalLinkage, strConstant, name);

    Constant* zero = Constant::getNullValue(t);
    Constant* indices[] = {zero, zero};
    Constant* strVal = ConstantExpr::getGetElementPtr(GVStr->getValueType(),GVStr, indices, true);
    //Value* strVal = Builder.CreateGEP(GlobalVariable->getValueType(),indices);
    return strVal;
}
//call error and exit function when overflow
void callError(Type* qtype,int line){

    std::vector<llvm::Type*> args;
    args.push_back(Type::getInt8PtrTy(TheContext));
    llvm::Type* returnT = llvm::Type::getVoidTy(TheContext);
    FunctionType *FT = FunctionType::get(returnT,args,false);
    FunctionCallee ErrorFunc = TheModule->getOrInsertFunction("perror", FT);

    std::string str = "overflow at line: " + std::to_string(line) + "\n";
    Constant *info = geti8StrVal(str.c_str(), qtype,"str");

    if (Function *F = dyn_cast<Function>(ErrorFunc.getCallee())) {
        Builder.CreateCall(F,info);
    }

    std::vector<llvm::Type*> exitargs;
    exitargs.push_back(qtype);
    llvm::Type* exitreturnT = llvm::Type::getVoidTy(TheContext);
    FunctionType *exitFT = FunctionType::get(exitreturnT,exitargs,false);
    FunctionCallee exitFunc = TheModule->getOrInsertFunction("exit", exitFT);
    llvm::Value* one = ConstantInt::get(qtype, 1);

    if (Function *F = dyn_cast<Function>(exitFunc.getCallee())) {
       Builder.CreateCall(F,one);
    }

    Builder.CreateUnreachable();
}

llvm::Type* IntType::getLLVMType() const{
    check_valid();
    return Type::getIntNTy(TheContext,width);
}

llvm::PointerType* PointType::getLLVMType() const{
    return elementType->getLLVMType()->getPointerTo(0);   
}

llvm::Type* ConstantType::getLLVMType() const{
    int initSize = getSuitableWidth(value,true);
    if(initSize==0){
        error("invalid number when getting LLVM TYPE1");
    }

    return (new IntType(true,initSize))->getLLVMType();

}

llvm::Type* ReturnType::getLLVMType() const{
    if(isVoid){
        return llvm::Type::getVoidTy(TheContext);
    }else {
        return qType->getLLVMType();
    }
    
}

/*
bool judgeValidCond(std::unique_ptr<ExprAST> cond){
    if( cond->getType() == ASTType::unary ){

        UnaryExprAST* unary = dynamic_cast<UnaryExprAST*>(cond.get());
        if(unary->getOperatorType()==Operators::minus)
            return false;

    }else if(cond->getType() == ASTType::binary){

        BinaryExprAST* binary = dynamic_cast<BinaryExprAST*>(cond.get());
        return binary->isCompareOp();

    }
    return true;
}
*/


