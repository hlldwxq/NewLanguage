/*
  TODO: (Meeting June 5)

  * Operator class

  * align-operations (upcast, etc) on QValue

  * QValue align_both(QValue,QValue) - to be used in binary operator

  * QValue align_right(QValue,QType) - to be used in assign, return, parameter

    uses QValue upcast(QValue)

  1. This refactoring
  2. Tests. And script that builds and runs them EASILY. E.g. "make test"

 */



#include "AST.h"
#include "Scope.h"
#include <assert.h>
std::unique_ptr<Module> TheModule;
static Scope<QAlloca,QFunction,QGlobalVariable> scope;


[[noreturn]] void error(std::string msg) {throw error_e(msg);}



void CallInitFunction(){
    std::vector<Function*> funs = scope.getInitFunction();
    if(funs.size()==0)
        return;
    std::vector<llvm::Type*> args;
    
    llvm::Type* returnT = llvm::Type::getVoidTy(TheContext);
    FunctionType *FT = FunctionType::get(returnT,args,false);
    Function *F = Function::Create(FT,Function::InternalLinkage, "init_all_global_array", TheModule.get());

    BasicBlock *BB = BasicBlock::Create(TheContext, "entry", F);
    Builder.SetInsertPoint(BB);

    
    for(int i = 0;i<funs.size();i++){
        Function* fun = funs[i];
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
            long long maxRange = 2*((1<<(initSize-2))-1)+1;
            long long minRange = -(1<<initSize);
            if(num<=maxRange && num>=minRange){
                break;
            }
            initSize*=2;
        }
    }else{
        while(initSize<=128){
            unsigned long long maxRange = 2*((1<<(initSize-1))-1)+1;
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
        maxRange = (2<<(initSize-2))-1;
        minRange = -(2<<(initSize-2));
    }else{
        maxRange = (2<<(initSize-1))-1;  // Notice the range of Numbers on the computer
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
        if(!(rightP->isNull()) && !leftP->compare(rightP)){
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
        TheModule->print(outs(), nullptr);
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

const QAlloca* VariableAST::codegenLeft(){

    const QAlloca* Alloca = scope.findSymbol(name);
    if(!Alloca){
        const QGlobalVariable* global = scope.findGlobalVar(name);
        if(!global){
            TheModule->print(outs(), nullptr);
            printf("%s\n",name.c_str());//<<std::endl;
            error("invalid number when getting LLVM TYPE");
        }

        Alloca = new QAlloca(global->getType(),global->getGlobalVariable());

    }
    return Alloca;
}

const QAlloca* ArrayIndexExprAST::codegenLeft(){

    QValue* left = pointer->codegen();
    Value* arrIndex = index->codegen()->getValue();
	
    if(left->getType()->isConstant()){
        
        if(dynamic_cast<ConstantType*>(left->getType())->getValue()<0){
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

/*
May be useful information:
1. bool llvm::DataLayout::isLegalInteger	(	uint64_t 	Width	)	const
Returns true if the specified type is known to be a native integer type supported by the CPU.
For example, i64 is not native on most 32-bit CPUs and i37 is not native on any known one. This returns false if the integer width is not legal.
The width is specified in bits.

Type* llvm::DataLayout::getLargestLegalIntType	(	LLVMContext & 	C	)	const
Returns the largest legal integer type, or null if none are set.
https://llvm.org/doxygen/classllvm_1_1DataLayout.html#a1fc35d2c9705ccecc0fe45c5ac123e47

2. 
const StructLayout * DataLayout::getStructLayout	(	StructType * 	Ty	)	const
Returns a StructLayout object, indicating the alignment of the struct, its size, and the offsets of its fields.
Note that this information is lazily cached.
Public Member Functions
   uint64_t 	getSizeInBytes () const
   uint64_t 	getSizeInBits () const
    https://llvm.org/doxygen/classllvm_1_1StructLayout.html
    https://llvm.org/doxygen/classllvm_1_1StructType.html

*/
QValue* NewExprAST::codegen(){
    
    /* have been tried, but does not work
        llvm::DataLayout* dataLayOut = new llvm::DataLayout(TheModule.get());
        Type* t = dataLayOut.getLargestLegalIntType(TheContext);
    */
    /* the result is zero
        llvm::DataLayout* dataLayOut = new llvm::DataLayout(TheModule.get());
        static StructType* 	structType = StructType::create(TheContext);
        const StructLayout* structLayout = dataLayOut->getStructLayout(structType);
        uint64_t sizeByte = structLayout->getSizeInBytes();
        //ErrorD("struct layout", sizeByte);
    */
    /* the result is always zero
        llvm::DataLayout* dataLayOut = new llvm::DataLayout(TheModule.get());
        bool result1 = dataLayOut->isLegalInteger(8);
        //ErrorD("is legal size", result1);
    */

    Type* t = Type::getIntNTy(TheContext,sizeof(size_t)*8);  // FIXME: Should use size-type of TARGET PLATFORM, not platform compiler is running on
    
    Value* arraySize = (size->codegen())->getValue();
    
    Value* mallocSize = ConstantExpr::getSizeOf(type->getElementType()->getLLVMType()); //the return type of getSizeOf is i64
    //ErrorD("new expr test",3);
    mallocSize = ConstantExpr::getTruncOrBitCast(cast<Constant>(mallocSize),t); // TODO: Downcast may overflow. Check!
    //ErrorD("new expr test",4);
    // TODO: Check if createMalloc can overflow computation! e.g. size = 2^31, type = i128 on 32-bit arch
    // check mallocSize * arraySize overflow
    // %res = call {i32, i1} @llvm.umul.with.overflow.i32(i32 %a, i32 %b)
    // %sum = extractvalue {i32, i1} %res, 0
    // %obit = extractvalue {i32, i1} %res, 1
    // br i1 %obit, label %overflow, label %normal

    Instruction* var_malloc = CallInst::CreateMalloc(Builder.GetInsertBlock(),t, type->getElementType()->getLLVMType(), mallocSize,arraySize,nullptr,"");
    
    Value* result = Builder.Insert(var_malloc);
    return new QValue(type,result); // FIXME Type is pointerType(type)
}

void DefAST::codegenCommand(){
    //ErrorD("def test",0);
    llvm::AllocaInst* Alloca = Builder.CreateAlloca(type->getLLVMType(), ConstantInt::get(Type::getInt32Ty(TheContext), 1), name);
    //ErrorD("def test",1);
    QAlloca* allo = new QAlloca(type,Alloca);
    //ErrorD("def test",2);
    if(!scope.addSymbol(name,allo)){
        CommandAST::lerror("");
    }

    QValue* init = value->codegen();
    //ErrorD("def test",3);
    if(!init)
        CommandAST::lerror("");

    init = assignCast(init,type);
    if(!init){
        CommandAST::lerror("type cannot be converted");
//         return false;
    }
    //ErrorD("def test",4);
    Value* store = Builder.CreateStore(init->getValue(), allo->getAlloca());
    //ErrorD("def test",5);
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
            Builder.CreateBr(&bb);
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
    BasicBlock *returnBB = BasicBlock::Create(TheContext, "returnBB", function);
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
    for(auto &Arg : function->args()){
        std::string name = args[i].first;
        QType* t = args[i].second; 
        llvm::AllocaInst* Alloca = Builder.CreateAlloca(t->getLLVMType(), ConstantInt::get(Type::getInt32Ty(TheContext), 1), name);
        QAlloca* allo = new QAlloca(t, Alloca);
        //QAlloca* allo = new QAlloca(t, &Arg);
        if(!scope.addSymbol(name,allo)){
            error("the identifier has been declared");
        }
        Builder.CreateStore(&Arg, Alloca);
        i++;
    }

    Body->codegenCommand();
    printf("a");
    Builder.SetInsertPoint(returnBB);
    printf("a1");
    if(P.getReturnType()->getIsVoid()){
        printf("a2");
        Builder.CreateRetVoid();
        printf("a3");
    }
    else{
        printf("a4");
        if(scope.getReturnNum()<=0)
            lerror("the function need return");
        llvm::Value* retValue = Builder.CreateLoad(retAlloca);
        printf("a5");
		Builder.CreateRet(retValue);
        printf("a6");
    }

    scope.removeScope();
    printf("a7");
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

void returnBr(){

    Function *f = Builder.GetInsertBlock()->getParent();
    for(auto iter = f->getBasicBlockList().begin(); iter != f->getBasicBlockList().end();iter++)
    {
        BasicBlock &bb = *iter;
        if(bb.getName().str() == "returnBB") {
            Builder.CreateBr(&bb);
        }
    }
} 

void IfAST::codegenCommand(){
    
    QValue* CondV = condition->codegen();
    
    if(!judgeValidCond(std::move(condition))){
        lerror("the condition is invalid");
    }
    printf("a\n");
    Function *TheFunction = Builder.GetInsertBlock()->getParent();
    BasicBlock *returnBB;
    printf("a1\n");
    for(auto iter = TheFunction->getBasicBlockList().begin(); iter != TheFunction->getBasicBlockList().end();iter++)
    {
        BasicBlock &bb = *iter;
        if(bb.getName().str() == "returnBB") {
            returnBB = &bb;
        }
    }
    printf("a2\n");
	// Create blocks for the then and else cases.  Insert the 'then' block at the
	// end of the function.
	BasicBlock *ThenBB = BasicBlock::Create(TheContext, "then", TheFunction,returnBB);
	BasicBlock *ElseBB = BasicBlock::Create(TheContext, "else",TheFunction,returnBB);
    BasicBlock *MergeBB;
    if(!isRet())
	    MergeBB = BasicBlock::Create(TheContext, "afterIf",TheFunction,returnBB);
    printf("a3\n");
	Builder.CreateCondBr(CondV->getValue(), ThenBB, ElseBB);

	// Emit then value.
	Builder.SetInsertPoint(ThenBB);
	thenC->codegenCommand();
    printf("a4\n");
    if(!thenC->isRet()) 
        Builder.CreateBr(MergeBB);
    printf("a5\n");
	// Codegen of 'Then' can change the current block, update ThenBB for the PHI.
	ThenBB = Builder.GetInsertBlock();

	// Emit else block.
	Builder.SetInsertPoint(ElseBB);
	elseC->codegenCommand();
	printf("a6\n");
    if(!elseC->isRet()) 
        Builder.CreateBr(MergeBB);
        
    printf("a7\n");
	// Codegen of 'Else' can change the current block, update ElseBB for the PHI.
	ElseBB = Builder.GetInsertBlock();
    printf("a8\n");
	// Emit merge block.
    if(!isRet()){
        Builder.SetInsertPoint(MergeBB);
    }
    printf("a9\n");
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
        scope.addInitFunction(F);
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

        scope.addInitFunction(F);
    }
    ////ErrorD("array",5);
}


/*

  sint8 a;

  a            + 128

  sint8        + sint16

  -->

  (sint16)a + 128 -> sint16


  128.signed_type  -> 16
  128.unsigned_type -> 8

  a + 128  -> max (a.type,128.type)


  127::?? + 127::?? -> 254::??

  127::?? + 127::?? + a::sint8
  254::sint16 + a::sint8

  a::sint8 + b::uint8 --> sint16

  ((sint16)a::sint8 + (128::sint16 + (sint16)b::sint8))::sint16 + (sint16)10::sint8

  128 & 129

  a + b

  if a.isnum && b.isnum then
    // Constant-propagation
    both_num=true
    // constant-propagation
  else {
    // Adjust sign
    if a.signed != b.signed
      if a.isnum then a = adjust_sign(a,b.signed)
      else if b.isnum then b = adjust_sign(b,a.signed)
      else error("sign mismatch")

    // No special cases for constants from this point on
    // Adjust width
    if (a.width < b.width) upcast(a,b.width) // Yields QValue!
    else if (a.width > b.width) upcast(b,b.width)
  }

  // Generate code for operator OR do constant propagation (based on both_num)


  class BinaryAST {
    Operator op;  // just a name

    codegen() {
      if both constants
        op.compute_const(a,b)
        find_suitable_type for result
      else
        align
        op.codegen(a,b)

    }

  }

  class Operator {
    virtual QValue codegen(QValue a, QValue b) = 0; // Assumes: a.type==b.type
    virtual long long compute_const(long long a, long long b) = 0;

  }

  // <, <=, >, >=, ==, !=
  class CompareOperator : Operator {

    virtual Value gen_llvm(bool, Value, Value) = 0;

    virtual QValue codegen(QValue a, QValue b) {
      llvm_result = gen_llvm(a.signed,a.value,b.value)
      return QValue(llvm_result,QType::bool)
    }

  }

  // +, -, *, div, mod, &, |
  class ArithOperator : Operator {
    virtual Value gen_llvm(bool, Value, Value) = 0;

    virtual QValue codegen(QValue a, QValue b) {
      llvm_result = gen_llvm(a.signed,a.value,b.value)
      return QValue(llvm_result,a.type)
    }

  }

  Less_Than_Operator::gen_llvm(signed,a,b) {
    if signed then llvm::gen_SLT(a,b) else llvm::gen_ULT(a,b)
  }

  long long Less_Than_Operator::compute_const(long long a,long long b) {
    return a<b ? 1 : 0
  }

  PlusOperator::codegen(signed, a,b) {
    return llvm_gen_plus(a,b)
  }

  long long PlusOperator::compute_const(long long a, long long b) {
    return a+b;
  }

  adjust_sign(a,signed)
    if (!signed && a.num<0) error();
    return Const(a.num,signed,suitable_type(a.num,signed))


  if a.signed != b.signed then
    if a.isnum then a = adjust_sign(a,b.signed)
    else if b.isnum then b = adjust_sign(b,a.signed)
    else error("sign mismatch")

*/
