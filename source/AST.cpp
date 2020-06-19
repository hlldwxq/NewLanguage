#include "../header/Parser.h"
#include "../header/Scope.h"
#include <assert.h>

Scope<QAlloca,QFunction,QGlobalVariable> scope;
[[noreturn]] void error(std::string msg) {throw error_e(msg);}

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


    if(leftT->getIsPointer() != varValue->getType()->getIsPointer()){
        return NULL;
    }
    // variable or arrayIndex
    if(!leftT->getIsPointer()){
        
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
        PointType* leftP = dynamic_cast<PointType*>(leftT);
        PointType* rightP = dynamic_cast<PointType*>(varValue->getType());
        if(!(rightP->isNull()) && !(leftP->compare(rightP))){
            return NULL;
        }
    }
    return varValue;
}

Value* plus::gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right){
    Value* result = Builder.CreateAdd(left, right, "addtmp");
    //TheModule->print(errs(), nullptr);
    return result;
}

Value* minus::gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right){
    return Builder.CreateSub(left, right, "subtmp");
}

Value* star::gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right){
    return Builder.CreateMul(left, right, "multmp");
}

Value* division::gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right){
    if(isSigned)
        return Builder.CreateSDiv(left, right, "divmp");
    else
        return Builder.CreateUDiv(left, right, "divmp");
}

Value* andT::gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right){
    return Builder.CreateAnd(left,right,"andmp");
}

Value* orT::gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right){
    return Builder.CreateOr(left,right,"ormp");
}

Value* equal_sign::gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right){
    return Builder.CreateICmpEQ(left, right, "cmptmp");
}

Value* not_equal::gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right){
    return Builder.CreateICmpNE(left, right, "cmptmp");
}

Value* less_equal::gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right){
    if(isSigned){
        return Builder.CreateICmpSLE(left, right, "cmptmp");
    }
    else{
        return Builder.CreateICmpULE(left, right, "cmptmp");
    }
}

Value* greater_equal::gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right){
    if(isSigned){
        return Builder.CreateICmpSGE(left, right, "cmptmp");
    }else{
        return Builder.CreateICmpUGE(left, right, "cmptmp");
    }
}

Value* greater_than::gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right){
    if(isSigned){
        return Builder.CreateICmpSGT(left, right, "cmptmp");
    }else{
        return Builder.CreateICmpUGT(left, right, "cmptmp");
    }
}

Value* less_than::gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right){
    if(isSigned){
        return Builder.CreateICmpSLT(left, right, "cmptmp");
    }else{
        return Builder.CreateICmpULT(left, right, "cmptmp");
    }
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

QValue* exclamation::codegen(QValue* operand){
    QType* type = operand->getType();
    if(type->getIsPointer()){
        error("! with pointer");
    }    

    IntType* intType = dynamic_cast<IntType*>(type);
    if(intType->getSigned()!=false || intType->getWidth()!=1){
        error("! with non-bool");
    }

    llvm::Value* realValue = Builder.CreateNot(operand->getValue());
    return new QValue(type,realValue);
}

QValue* negative::codegen(QValue* operand){
    QType* type = operand->getType();
    if(type->getIsPointer()){
        error("- with pointer");
    }

    IntType* intType = dynamic_cast<IntType*>(type);
    if(intType->getSigned()!=true){
        error("! with unsigned");
    }
    llvm::Value* minu = Builder.CreateNeg(operand->getValue());
    return new QValue(type,minu);
}

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
    if(!leftQV){
        return NULL;
    }
    QValue* rightQV = RHS->codegen();
    if(!rightQV){
        return NULL;
    }

    if(leftQV->getType()->getIsPointer() || rightQV->getType()->getIsPointer()){
        error("The operands of a binary operator cannot be a pointer");
        //return NULL;
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
                //return NULL;
            }
        }else{
            IntType* leftInt = dynamic_cast<IntType*>(leftQV->getType());
            rightQV = adjustSign(rightQV,leftInt->getSigned());
            if(!rightQV){
                error("unvalid binary calculation between signed number and unsigned number");
               // return NULL;
            }
        }
    }

    IntType* leftInt = dynamic_cast<IntType*>(leftQV->getType());
    IntType* rightInt = dynamic_cast<IntType*>(rightQV->getType());


    if(leftInt->getSigned()!=rightInt->getSigned()){
        
        error("invalid binary calculation between signed number and unsigned number");
        //return NULL;
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

Constant* geti8StrVal(char const* str, Type* t, Twine const& name) {
  
  Constant* strConstant = ConstantDataArray::getString(TheContext, str);
  GlobalVariable* GVStr =
      new GlobalVariable(*(TheModule.get()), strConstant->getType(), true,
                         GlobalValue::InternalLinkage, strConstant, name);

  Constant* zero = Constant::getNullValue(t);
  Constant* indices[] = {zero, zero};
  Constant* strVal = ConstantExpr::getGetElementPtr(GVStr->getValueType(),GVStr, indices, true);
  //Value* strVal = Builder.CreateGEP(GlobalVariable->getValueType(),indices);
  return strVal;
}

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

void DefAST::codegenCommand(){

    llvm::AllocaInst* Alloca = Builder.CreateAlloca(type->getLLVMType(), ConstantInt::get(Type::getInt32Ty(TheContext), 1), name);
    QAlloca* allo = new QAlloca(type,Alloca);

    if(!scope.addSymbol(name,allo)){
        CommandAST::lerror("");
    }

    QValue* init = value->codegen();
    if(!init)
        CommandAST::lerror("");

    init = assignCast(init,type);
    if(!init){
        CommandAST::lerror("type cannot be converted");
    }

    Value* store = Builder.CreateStore(init->getValue(), allo->getAlloca());
    if(!store){
        Bug("failed store",1);
    }
}

void AssignAST::codegenCommand(){

    const QAlloca* leftV = left->codegenLeft();
    if(!leftV)
        lerror("");

    QValue* rightV = right->codegen();
    if(!rightV)
        lerror("");

    rightV = assignCast(rightV,leftV->getType());
    if(!rightV){
        lerror("type cannot be converted");
    }

    llvm::Value* store = Builder.CreateStore(rightV->getValue(), leftV->getAlloca());
    if(!store){
        Bug("failed store",0);
    }

}

void ReturnAST::codegenCommand(){

    //const QAlloca* returnAlloca = scope.findSymbol("return");
    const QAlloca* returnAlloca = scope.getReturnAlloca();
    if(!returnAlloca){
        lerror("The function does not need return value");
    }
    QValue* returnQv = value->codegen();
    
    returnQv = assignCast(returnQv,returnAlloca->getType());
    if(!returnQv){
        lerror("type cannot be converted");
    }

    Value* store = Builder.CreateStore(returnQv->getValue(), returnAlloca->getAlloca());
    if(!store){
        Bug("failed store",0);
    }

    Function *f = Builder.GetInsertBlock()->getParent();
    for(auto iter = f->getBasicBlockList().begin(); iter != f->getBasicBlockList().end();iter++)
    {
        BasicBlock &bb = *iter;
        if(bb.getName().str() == "returnBB") {
            Builder.CreateBr(&bb); //will not meet an error when there is no returnBB
        }
    }
}

void BlockAST::codegenCommand(){

    scope.addScope();
    for(int i=0 ; i < cmds.size(); i++){
        cmds[i]->codegenCommand();

    }
    
    scope.removeScope();
}

void PrototypeAST::codegenStructure(){

    std::vector<llvm::Type*> args;
    
    for(int i=0;i<Args.size();i++){
        args.push_back(Args[i].second->getLLVMType());
    }
    
    llvm::Type* returnT = returnType->getLLVMType();
    FunctionType *FT = FunctionType::get(returnT,args,false);
    Function *F = Function::Create(FT,Function::ExternalLinkage, Name, TheModule.get());
    
    int idx = 0;
    for(auto &Arg : F->args()){
        Arg.setName(Args[idx].first);
        idx++;
    }

    std::vector<QType*> argsType;
    for(int i=0;i<Args.size();i++){
        argsType.push_back(Args[i].second);
    }
    QFunction* functionQ = new QFunction(returnType, argsType ,F);

    bool success = scope.addFunction(Name,functionQ);
    if(!success){
        error("the function has been declared");
    }
}

void FunctionAST::codegenStructure(){

    Proto->codegenStructure();
    Function* function = scope.getFunction(Proto->getFunctionName())->getFunction();
    auto &P = *Proto;

    BasicBlock *BB = BasicBlock::Create(TheContext, "entry", function);
    BasicBlock *returnBB = NULL;
    if(!P.getReturnType()->getIsVoid()){
        if(Body->isRet() && Parser::getReturnNum()>1){ 
            //the last instruction is return and it is the only return
            returnBB = BasicBlock::Create(TheContext, "returnBB", function);
        }
    }
    
	Builder.SetInsertPoint(BB);

    scope.addScope();
    llvm::AllocaInst* retAlloca;
    if(!P.getReturnType()->getIsVoid()){
        retAlloca = Builder.CreateAlloca(P.getReturnType()->getLLVMType(), ConstantInt::get(Type::getInt32Ty(TheContext), 1));
        scope.setReturnAlloca(new QAlloca(P.getReturnType()->getType(),retAlloca));
       // scope.addSymbol("return",new QAlloca(P.getReturnType()->getType(),retAlloca));
    }

    std::vector<std::pair<std::string,QType*>> args = P.getArgs();
    int i=0;
    //args
    for(auto &Arg : function->args()){
        std::string name = args[i].first;
        QType* t = args[i].second; 
        llvm::AllocaInst* Alloca = Builder.CreateAlloca(t->getLLVMType(), ConstantInt::get(Type::getInt32Ty(TheContext), 1), name);
        QAlloca* allo = new QAlloca(t, Alloca);

        if(!scope.addSymbol(name,allo)){
            error("the identifier has been declared");
        }
        Builder.CreateStore(&Arg, Alloca);
        i++;
    }

    Body->codegenCommand();
    if(returnBB != NULL){
        //make sure the returnBB at the end of func
        returnBB->moveAfter(&(function->getBasicBlockList().back()));
        Builder.SetInsertPoint(returnBB);
    }

    if(P.getReturnType()->getIsVoid()){
        Builder.CreateRetVoid();
    }
    else{
        llvm::Value* retValue = Builder.CreateLoad(retAlloca);
		Builder.CreateRet(retValue);
    }

    scope.removeScope();
    
}

QValue* NullExprAST::codegen(){
    Bug("not yet implemented",line);
}

QValue* CallExprAST::codegen(){

    const QFunction* call = scope.getFunction(functionName);
    Function* func = call->getFunction();
    std::vector<QType*> argsType = call->getArgsType();

    ReturnType* returnType = call->getReturnType();
    if(returnType->getIsVoid()){
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
    return new QValue(returnType->getType(),Builder.CreateCall(func, ArgsV, "calltmp"));

}

void CallExprAST::codegenCommand(){
    codegen();
}

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

void IfAST::codegenCommand(){
    
    QValue* CondV = condition->codegen();
    
    if(!judgeValidCond(std::move(condition))){
        lerror("the condition is invalid");
    }

    Function *TheFunction = Builder.GetInsertBlock()->getParent();

	// Create blocks for the then and else cases.  Insert the 'then' block at the
	// end of the function.
	BasicBlock *ThenBB = BasicBlock::Create(TheContext, "then", TheFunction);
	BasicBlock *ElseBB = BasicBlock::Create(TheContext, "else",TheFunction);
    BasicBlock *MergeBB;
    if(!isRet())
	    MergeBB = BasicBlock::Create(TheContext, "afterIf",TheFunction);

	Builder.CreateCondBr(CondV->getValue(), ThenBB, ElseBB);

    // then
	Builder.SetInsertPoint(ThenBB);
	thenC->codegenCommand();
    if(!thenC->isRet()){ 
        Builder.CreateBr(MergeBB);
    }
	ThenBB = Builder.GetInsertBlock();  // Codegen of 'Then' can change the current block.

	// else
	Builder.SetInsertPoint(ElseBB);
	elseC->codegenCommand();
    if(!elseC->isRet()) {
        Builder.CreateBr(MergeBB);
    }
	ElseBB = Builder.GetInsertBlock();

	// merge
    if(!isRet()){
        Builder.SetInsertPoint(MergeBB);
    }

}

void ForAST::codegenCommand(){
    Bug("not yet implemented",line);
}

void WhileAST::codegenCommand(){
    Bug("not yet implemented",line);
}

void BreakAST::codegenCommand(){
    Bug("not yet implemented",line);
}

GlobalVariable* DefAST::globalInit(){

    llvm::Constant* constantP = Constant::getNullValue(type->getLLVMType());
    
    GlobalVariable * globalV = new GlobalVariable(type->getLLVMType(),false, 
                                    GlobalValue::CommonLinkage,constantP,name); 
    TheModule->getGlobalList().push_back(globalV);
    if(!scope.addGlobalVar(name,new QGlobalVariable(type,globalV))){
        CommandAST::lerror("the gloabal variable has been declared");
    }
    return globalV;
}

Function* DefAST::globalInitFunc(){
    ////ErrorD("error",3);
    std::vector<llvm::Type*> args;
    
    llvm::Type* returnT = llvm::Type::getVoidTy(TheContext);
    FunctionType *FT = FunctionType::get(returnT,args,false);
    Function *F = Function::Create(FT,Function::InternalLinkage, "init_global_var_"+name, TheModule.get());

    BasicBlock *BB = BasicBlock::Create(TheContext, "entry", F);
    Builder.SetInsertPoint(BB);
    return F;
    ////ErrorD("error",4);
}

void VarDefAST::codegenStructure(){
    ////ErrorD("var",1);
    GlobalVariable* globalV = globalInit();
    assert(globalV);

    QValue* qvalue = value->codegen();
    if(!(qvalue->getType()->isConstant())){
        CommandAST::lerror("the value of global variable must be constant");
    }

    ConstantType* numType = dynamic_cast<ConstantType*>(qvalue->getType());
    if(numType->getValue() != 0){
        IntType* leftType = dynamic_cast<IntType*>(type);
        if(leftType->getSigned() && numType->getValue()<0){
            CommandAST::lerror("cannot assign a negative value to an unsigned variable");
        }
        if(getSuitableWidth(numType->getValue(),leftType->getSigned()) > leftType->getWidth()){
            CommandAST::lerror("type cannot be converted");
        }
        
        Function* F = globalInitFunc();
        llvm::Value* rightV = ConstantInt::get(leftType->getLLVMType(),numType->getValue()); 
        Builder.CreateStore(rightV, globalV);
        Builder.CreateRetVoid();
        std::vector<QType*> argsT;
        scope.addInitFunction(new QFunction(new ReturnType(),argsT,F));
    }
    ////ErrorD("var",2);
}

void ArrayDefAST::codegenStructure(){
    ////ErrorD("array",1);
    GlobalVariable* globalV = globalInit();

    ////ErrorD("array",2);
    if(value->getType()!=ASTType::nullT){
        
        Function* F = globalInitFunc();
        ////ErrorD("array",21);
        QValue* rightQv = value->codegen();
        ////ErrorD("array",22);
        QType* rightQt = rightQv->getType();
        ////ErrorD("array",23);
        Value* rightV = rightQv->getValue();
        ////ErrorD("array",3);
        if(!type->compare(rightQt)){
            CommandAST::lerror("The types on both sides of the equal sign must be the same");
        }
        ////ErrorD("array",4);
        Builder.CreateStore(rightV, globalV);
        Builder.CreateRetVoid();
        std::vector<QType*> args;
        scope.addInitFunction(new QFunction(new ReturnType(),args,F));
    }
    ////ErrorD("array",5);
}
