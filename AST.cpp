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
        ErrorQ("invalid number when getting LLVM TYPE",value);
        return NULL;
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
        const QGlobalVariable* global = scope.findGloableVar(name);
        if(!global){
            ErrorQ("Undeclared variables",line);
            return NULL;
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
            ErrorQ("the index of array cannot be negative",line);
            return NULL;
        }
    }
    if(!left->getType()->getIsPointer()){
        ErrorQ("left expression must be a pointer",line);
        return NULL;
    }
    
    Value* eleptr = Builder.CreateGEP(cast<PointerType>(left->getValue()->getType()->getScalarType())->getElementType(), left->getValue(), {arrIndex});
    
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
        ErrorQ("The data type after the unary operator does not meet the requirements",line);
        return NULL;
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
        ErrorQ("The operands of a binary operator cannot be a pointer",line);
        return NULL;
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
                ErrorQ("unvalid binary calculation between signed number and unsigned number",line);
                return NULL;
            }
        }else{
            IntType* leftInt = dynamic_cast<IntType*>(leftQV->getType());
            rightQV = adjustSign(rightQV,leftInt->getSigned());
            if(!rightQV){
                ErrorQ("unvalid binary calculation between signed number and unsigned number",line);
                return NULL;
            }
        }
    }

    IntType* leftInt = dynamic_cast<IntType*>(leftQV->getType());
    IntType* rightInt = dynamic_cast<IntType*>(rightQV->getType());


    if(leftInt->getSigned()!=rightInt->getSigned()){
        
        ErrorQ("invalid binary calculation between signed number and unsigned number",line);
        return NULL;
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

QValue* NewExprAST::codegen(){
    
    Type* t = Type::getIntNTy(TheContext,sizeof(size_t)*8);  // FIXME: Should use size-type of TARGET PLATFORM, not platform compiler is running on
    Value* arraySize = (size->codegen())->getValue();
    
    Value* mallocSize = ConstantExpr::getSizeOf(type->getElementType()->getLLVMType());
    
    mallocSize = ConstantExpr::getTruncOrBitCast(cast<Constant>(mallocSize),t); // TODO: Downcast may overflow. Check!
    
    Instruction* var_malloc = CallInst::CreateMalloc(Builder.GetInsertBlock(),t, type->getElementType()->getLLVMType(), mallocSize,arraySize,nullptr,"");
    
    // TODO: Check if createMalloc can overflow computation! e.g. size = 2^31, type = i128 on 32-bit arch
    Value* result = Builder.Insert(var_malloc);
    return new QValue(type,result); // FIXME Type is pointerType(type)
}

bool DefAST::codegenCommand(){

    llvm::AllocaInst* Alloca = Builder.CreateAlloca(type->getLLVMType(), ConstantInt::get(Type::getInt32Ty(TheContext), 1), name);
    QAlloca* allo = new QAlloca(type,Alloca);
    if(!scope.addSymbol(name,allo)){
        return false;
    }

    QValue* init = value->codegen();
    if(!init)
        return false;

    init = assignCast(init,type);
    if(!init){
        ErrorQ("type cannot be converted",line);
        return false;
    }

    Value* store = Builder.CreateStore(init->getValue(), allo->getAlloca());

    if(!store){
        Bug("failed store",1);
    }
    return true;
}

bool AssignAST::codegenCommand(){

    const QAlloca* leftV = left->codegenLeft();
    if(!leftV)
        return false;

    QValue* rightV = right->codegen();
    if(!rightV)
        return false;

    rightV = assignCast(rightV,leftV->getType());
    if(!rightV){
        ErrorQ("type cannot be converted",line);
        return false;
    }

    llvm::Value* store = Builder.CreateStore(rightV->getValue(), leftV->getAlloca());
    if(!store){
        Bug("failed store",0);
    }
    return true;

}

bool ReturnAST::codegenCommand(){

    const QAlloca* returnAlloca = scope.findSymbol("return");
    if(!returnAlloca){
        ErrorQ("The function does not need return value",line);
        return false;
    }
    QValue* returnQv = value->codegen();
    if(!returnQv)
        return false;

    returnQv = assignCast(returnQv,returnAlloca->getType());
    if(!returnQv){
        ErrorQ("type cannot be converted",line);
        return false;
    }

    Value* store = Builder.CreateStore(returnQv->getValue(), returnAlloca->getAlloca());
    if(!store){
        Bug("failed store",0);
        return false;
    }
    return true;
}

bool BlockAST::codegenCommand(){
 
    for(int i=0 ; i < cmds.size(); i++){
        bool success = cmds[i]->codegenCommand();
        if(!success)
            return false;
    }
    return true;
}

bool PrototypeAST::codegenStructure(){

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
        ErrorQ("the function has been declared",line);
        return false;
    }
    return true;
}

bool FunctionAST::codegenStructure(){

    bool pro = Proto->codegenStructure();
    if(!pro)
        return false;
    Function* function = scope.getFunction(Proto->getFunctionName())->getFunction();

    auto &P = *Proto;

    BasicBlock *BB = BasicBlock::Create(TheContext, "entry", function);
	Builder.SetInsertPoint(BB);

    scope.addScope();
    llvm::AllocaInst* retAlloca;
    if(!P.getReturnType()->getIsVoid()){
        retAlloca = Builder.CreateAlloca(P.getReturnType()->getLLVMType(), ConstantInt::get(Type::getInt32Ty(TheContext), 1));
        scope.addSymbol("return",new QAlloca(P.getReturnType()->getType(),retAlloca));
    }

    std::vector<std::pair<std::string,QType*>> args = P.getArgs();
    int i=0;
    for(auto &Arg : function->args()){
        std::string name = args[i].first;
        QType* t = args[i].second; 
        llvm::AllocaInst* Alloca = Builder.CreateAlloca(t->getLLVMType(), ConstantInt::get(Type::getInt32Ty(TheContext), 1), name);
        QAlloca* allo = new QAlloca(t, Alloca);
        if(!scope.addSymbol(name,allo)){
            ErrorQ("the identifier has been declared",line);
            return false;
        }
        Builder.CreateStore(&Arg, Alloca);
        i++;
    }

    bool bo = Body->codegenCommand();
    if(!bo)
        return false;

    if(P.getReturnType()->getIsVoid())
        Builder.CreateRetVoid();
    else{
        llvm::Value* retValue = Builder.CreateLoad(retAlloca);
		Builder.CreateRet(retValue);
    }

    scope.removeScope();

    return true;
}

QValue* NullExprAST::codegen(){
    return NULL;
}

QValue* CallExprAST::codegen(){

    const QFunction* call = scope.getFunction(functionName);
    Function* func = call->getFunction();
    std::vector<QType*> argsType = call->getArgsType();

    ReturnType* returnType = call->getReturnType();
    if(returnType->getIsVoid()){
        ErrorQ("the return type of the function is void, cannot be the right value",line);
        return NULL;
    }

    if(func->arg_size()!=args.size()){
        ErrorQ("The number of participating arguments does not match",line);
        return NULL;
    }
    
    std::vector<Value*> ArgsV;

    for(int i=0 ; i<args.size(); i++){
        
        QType* needArgType = argsType[i];
        QValue* realArg = args[i]->codegen();
        if(!realArg)
            return NULL;
        
        realArg = assignCast(realArg,needArgType);
        if(!realArg){
            ErrorQ("type cannot be converted",line);
            return NULL;
        }
        ArgsV.push_back(realArg->getValue());
    }
    return new QValue(returnType->getType(),Builder.CreateCall(func, ArgsV, "calltmp"));

}

bool CallExprAST::codegenCommand(){

    QValue* callCodegen = codegen();
    if(!callCodegen)
        return false;

    return true;
}

bool IfAST::codegenCommand(){
    return true;
}

bool ForAST::codegenCommand(){
    return true;
}

bool WhileAST::codegenCommand(){
    return true;
}

bool BreakAST::codegenCommand(){
    return true;
}

GlobalVariable* DefAST::globalInit(){

    llvm::Constant* constantP = Constant::getNullValue(type->getLLVMType());
    
    GlobalVariable * globalV = new GlobalVariable(type->getLLVMType(),false, 
                                    GlobalValue::CommonLinkage,constantP,name); 
    TheModule->getGlobalList().push_back(globalV);
    if(!scope.addGloabalVar(name,new QGlobalVariable(type,globalV))){
        ErrorQ("the gloabal variable has been declared",line);
        return NULL;
    }
    return globalV;
}

Function* DefAST::globalInitFunc(){
    //ErrorD("error",3);
    std::vector<llvm::Type*> args;
    
    llvm::Type* returnT = llvm::Type::getVoidTy(TheContext);
    FunctionType *FT = FunctionType::get(returnT,args,false);
    Function *F = Function::Create(FT,Function::InternalLinkage, "init_global_var_"+name, TheModule.get());

    BasicBlock *BB = BasicBlock::Create(TheContext, "entry", F);
    Builder.SetInsertPoint(BB);
    return F;
    //ErrorD("error",4);
}

bool VarDefAST::codegenStructure(){
    //ErrorD("var",1);
    GlobalVariable* globalV = globalInit();
    if(!globalV){
        return false;
    }

    QValue* qvalue = value->codegen();
    if(!(qvalue->getType()->isConstant())){
        ErrorQ("the value of gloable variable must be constant",line);
        return false;
    }

    ConstantType* numType = dynamic_cast<ConstantType*>(qvalue->getType());
    if(numType->getValue() != 0){
        IntType* leftType = dynamic_cast<IntType*>(type);
        if(leftType->getSigned() && numType->getValue()<0){
            ErrorQ("cannot assign a negative value to an unsigned variable",line);
            return false;
        }
        if(getSuitableWidth(numType->getValue(),leftType->getSigned()) > leftType->getWidth()){
            ErrorQ("type cannot be converted",line);
            return false;
        }
        
        Function* F = globalInitFunc();
        llvm::Value* rightV = ConstantInt::get(leftType->getLLVMType(),numType->getValue()); 
        Builder.CreateStore(rightV, globalV);
        Builder.CreateRetVoid();
        scope.addInitFunction(F);
    }
    //ErrorD("var",2);
    return true;
}

bool ArrayDefAST::codegenStructure(){
    //ErrorD("array",1);
    GlobalVariable* globalV = globalInit();
    if(!globalV){
        return false;
    }
    //ErrorD("array",2);
    if(value->getType()!=ASTType::nullT){
        
        Function* F = globalInitFunc();
        //ErrorD("array",21);
        QValue* rightQv = value->codegen();
        //ErrorD("array",22);
        QType* rightQt = rightQv->getType();
        //ErrorD("array",23);
        Value* rightV = rightQv->getValue();
        //ErrorD("array",3);
        if(!type->compare(rightQt)){
            ErrorQ("The types on both sides of the equal sign must be the same",line);
            return false;
        }
        //ErrorD("array",4);
        Builder.CreateStore(rightV, globalV);
        Builder.CreateRetVoid();

        scope.addInitFunction(F);
    }
    //ErrorD("array",5);
    return true;
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
