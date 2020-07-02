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
Scope<QAlloca,QFunction,QGlobalVariable,ReturnType,QValue> scope;

std::map<int,std::string> maxIntSignedValue;
std::map<int,std::string> minIntSignedValue;
std::map<int,std::string> maxIntUnSignedValue;
std::map<int,std::string> minIntUnSignedValue;

void initIntValueStr(){
    maxIntSignedValue.insert(std::pair<int,std::string>(8,"127"));
    maxIntSignedValue.insert(std::pair<int,std::string>(16,"32767"));
    maxIntSignedValue.insert(std::pair<int,std::string>(32,"2147483647"));
    maxIntSignedValue.insert(std::pair<int,std::string>(64,"9223372036854775807"));
    //maxIntSignedValue.insert(std::pair<int,std::string>(64,"9223372036854775807‬"));
    maxIntSignedValue.insert(std::pair<int,std::string>(128,"175642133179605998668418774123684105727"));

    minIntSignedValue.insert(std::pair<int,std::string>(8,"128"));
    minIntSignedValue.insert(std::pair<int,std::string>(16,"32768"));
    minIntSignedValue.insert(std::pair<int,std::string>(32,"2147483648"));
    minIntSignedValue.insert(std::pair<int,std::string>(64,"9223372036854775808"));
    minIntSignedValue.insert(std::pair<int,std::string>(128,"175642133179605998668418774123684105728"));

    maxIntUnSignedValue.insert(std::pair<int,std::string>(8,"255"));
    maxIntUnSignedValue.insert(std::pair<int,std::string>(16,"65535"));
    maxIntUnSignedValue.insert(std::pair<int,std::string>(32,"4294967295"));
    maxIntUnSignedValue.insert(std::pair<int,std::string>(64,"18446744073709551615"));
    maxIntUnSignedValue.insert(std::pair<int,std::string>(128,"340282366920938463463374607431768211455"));

    minIntUnSignedValue.insert(std::pair<int,std::string>(8,"256"));
    minIntUnSignedValue.insert(std::pair<int,std::string>(16,"65536"));
    minIntUnSignedValue.insert(std::pair<int,std::string>(32,"4294967296"));
    minIntUnSignedValue.insert(std::pair<int,std::string>(64,"18446744073709551616"));
    minIntUnSignedValue.insert(std::pair<int,std::string>(128,"340282366920938463463374607431768211456"));
}

int getBit(std::map<int,std::string> valueMap, std::string value){
    int bit = 8;
    std::string maxValue;

    while( bit <= 128 ){
        maxValue = valueMap[bit];
        
        if( value.size() > maxValue.size() ){

            bit *= 2;
        }else if( value.size() < maxValue.size() ){
            return bit;
        }else if( value.size() == maxValue.size() ){
            for(int i=0;i<value.size();i++){
                if(maxValue[i]<value[i]){
                    bit*=2;
                    break;
                }
                if(maxValue[i]>value[i]){
                    return bit;
                }
                if(i==value.size()-1 && maxValue[i]==value[i]){
                    return bit;
                }
            }
        }
    }
    if(bit>128)
        return -1;
    else
        return bit;
    
}

int getBitOfInt(std::string value/*, bool isPos*/, bool isSigned){
    
    if(maxIntSignedValue.empty()){
        initIntValueStr();
    }
    if(value=="0")
        return 8;
    bool isPos = (value[0]=='+');
    value.erase(0,1);
    if( isPos && isSigned ){   //有符号正数
        return getBit(maxIntSignedValue,value);
    }else if( isPos && !isSigned){   //无符号正数
        return getBit(maxIntUnSignedValue,value);
    }else if( !isPos && isSigned){   //有符号负数
        return getBit(minIntSignedValue,value);
    }else{  //无符号负数
        return getBit(minIntUnSignedValue,value);
    }
}

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
        if(type->getValue()[0]=='-'){
           // printf("unsigned and neg\n");
            return NULL;
        }
    }
    int width = getBitOfInt(type->getValue(),isSigned);
  //  printf("width %d\n",width);
    if(width<=0){
        return NULL;
    }
    IntType* t = new IntType(isSigned,width);
    llvm::Value* constInt = ConstantInt::get(TheContext,llvm::APInt(width, type->getValue(), 10));
    return new QValue(t,constInt);
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

Constant* geti8StrVal(char const* str, Type* t, std::string name) {
    //error
    //GlobalVariable* GVStr = TheModule->getGlobalVariable(name, true); //true allow internal global var
    Constant* strConstant = ConstantDataArray::getString(TheContext, str);
    auto GVStr = TheModule->getOrInsertGlobal(name, strConstant->getType());
   
    Builder.CreateStore(strConstant, GVStr);
    
    Constant* zero = Constant::getNullValue(t);
    Constant* indices[] = {zero, zero};
    Constant* strVal = ConstantExpr::getGetElementPtr(/*GVStr->getValueType()*/strConstant->getType(),GVStr, indices, true);
    return strVal;
}
//call error and exit function when overflow
void callError(std::string info, int line){

    llvm::DataLayout* dataLayOut = new llvm::DataLayout(TheModule.get());
    Type* qtype = dataLayOut->getLargestLegalIntType(TheContext);

    std::vector<llvm::Type*> args;
    args.push_back(Type::getInt8PtrTy(TheContext));
    args.push_back(qtype); //line number
    llvm::Type* returnT = qtype; //return type
    FunctionType *FT = FunctionType::get(returnT,args,false);
    FunctionCallee PrintFunc = TheModule->getOrInsertFunction("printf", FT);

    std::string str = info + " at line: %d\n";
    Constant *lineN = ConstantInt::get(qtype, line);
    /*Constant *info1 = geti8StrVal(str.c_str(), qtype,"str");
    std::vector<Value*> ArgsV;
    ArgsV.push_back(info1);
    ArgsV.push_back(lineN);
    */
    const char *str_ptr = str.c_str();
    Value *globalStrPtr = Builder.CreateGlobalStringPtr(str_ptr);
    std::vector<Value*> ArgsV;
    ArgsV.push_back(globalStrPtr);
    ArgsV.push_back(lineN);

    Function *F = dyn_cast<Function>(PrintFunc.getCallee());
    assert(F);
    Builder.CreateCall(F,ArgsV);

//     if (Function *F = dyn_cast<Function>(PrintFunc.getCallee())) {
//         Builder.CreateCall(F,ArgsV);
//     }

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
    int initSize = getBitOfInt(value,true);
    if(initSize<=0){
        error("invalid number when getting TYPE: "+ value);
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
