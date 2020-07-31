#include "../header/AST.h"
#include "../header/Scope.h"

#include <assert.h>

//the definition of global variable is at the end of the file
IntType IntType::bool_type = IntType(false,1);
LLVMContext TheContext;
IRBuilder<> Builder(TheContext);
std::unique_ptr<Module> TheModule;
std::unique_ptr<TargetMachine> TM;
bool* doCheck = NULL;
llvm::Type* sizet;
Scope<QAlloca,QFunction,QGlobalVariable,ReturnType,QValue> scope;

std::map<int,std::string> maxIntSignedValue;
std::map<int,std::string> minIntSignedValue;
std::map<int,std::string> maxIntUnSignedValue;
std::map<int,std::string> minIntUnSignedValue;

void createBr(std::string errorMessage,Value* cmp,int line,std::string bbNameE, std::string bbNameN){
    Function *TheFunction = Builder.GetInsertBlock()->getParent();

    BasicBlock *errorBB = BasicBlock::Create(TheContext, bbNameE, TheFunction);
	BasicBlock *normalBB = BasicBlock::Create(TheContext, bbNameN,TheFunction);
	Builder.CreateCondBr(cmp, errorBB, normalBB);

    // overflow
	Builder.SetInsertPoint(errorBB);
    callError(errorMessage.c_str(),line);
	errorBB = Builder.GetInsertBlock(); 

    //normal
	Builder.SetInsertPoint(normalBB);
}

void initIntValueStr(){
    maxIntSignedValue.insert(std::pair<int,std::string>(8,"127"));
    maxIntSignedValue.insert(std::pair<int,std::string>(16,"32767"));
    maxIntSignedValue.insert(std::pair<int,std::string>(32,"2147483647"));
    maxIntSignedValue.insert(std::pair<int,std::string>(64,"9223372036854775807"));
    //maxIntSignedValue.insert(std::pair<int,std::string>(64,"9223372036854775807‬"));
    maxIntSignedValue.insert(std::pair<int,std::string>(128,"170141183460469231731687303715884105727"));
    

    minIntSignedValue.insert(std::pair<int,std::string>(8,"128"));
    minIntSignedValue.insert(std::pair<int,std::string>(16,"32768"));
    minIntSignedValue.insert(std::pair<int,std::string>(32,"2147483648"));
    minIntSignedValue.insert(std::pair<int,std::string>(64,"9223372036854775808"));
    minIntSignedValue.insert(std::pair<int,std::string>(128,"170141183460469231731687303715884105728"));

    maxIntUnSignedValue.insert(std::pair<int,std::string>(8,"255"));
    maxIntUnSignedValue.insert(std::pair<int,std::string>(16,"65535"));
    maxIntUnSignedValue.insert(std::pair<int,std::string>(32,"4294967295"));
    maxIntUnSignedValue.insert(std::pair<int,std::string>(64,"18446744073709551615"));
    maxIntUnSignedValue.insert(std::pair<int,std::string>(128,"340282366920938463463374607431768211455"));

    minIntUnSignedValue.insert(std::pair<int,std::string>(8,"0"));
    minIntUnSignedValue.insert(std::pair<int,std::string>(16,"0"));
    minIntUnSignedValue.insert(std::pair<int,std::string>(32,"0"));
    minIntUnSignedValue.insert(std::pair<int,std::string>(64,"0"));
    minIntUnSignedValue.insert(std::pair<int,std::string>(128,"0"));
}

int getBit(std::map<int,std::string> valueMap, std::string value){
    int bit = 8;
    std::string maxValue;

    while( bit < 256 ){
        maxValue = valueMap[bit];
        if(value==maxValue)
            return bit;
        if( value.size() > maxValue.size() ){
            bit *= 2;
        }else if( value.size() < maxValue.size() ){
            return bit;
        }else if( value.size() == maxValue.size() ){
            
            for(int i=0;i<value.size();i++){
                if(maxValue[i]<value[i]){
                    return bit*2;
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
    return bit;
    
}

int getBitOfInt(std::string value, bool isSigned){
    
    if(maxIntSignedValue.empty()){
        initIntValueStr();
    }
    if(value=="0")
        return 8;
    bool isPos = !(value[0]=='-');
    if(!isPos) 
        value.erase(0,1); //remove -
    
    if( isPos && isSigned ){   //有符号正数
        return getBit(maxIntSignedValue,value);

    }else if( isPos && !isSigned){   //无符号正数
        return getBit(maxIntUnSignedValue,value);

    }else if( !isPos && isSigned){   //有符号负数
        return getBit(minIntSignedValue,value);

    }else{  
        return -1;
    }
}

int getBitOfAPInt(llvm::APInt value, bool isSigned){
    int width = 8;
    int currentW= value.getBitWidth();
    while(true){    
        if(isSigned){
            if(value.isNegative()){
                llvm::APInt minValue = APInt::getSignedMinValue(width).sext(currentW);
                //Gets maximum signed value of APInt for specific bit width.
                if(value.slt(minValue)){ //if current value is bigger than the max size, the width is not suitable
                    width = width*2;
                }else{
                    break;
                }
            }
            else{
                llvm::APInt maxValue = APInt::getSignedMaxValue(width).sext(currentW);
                //Gets maximum signed value of APInt for specific bit width.
                if(value.sgt(maxValue)){ //if current value is bigger than the max size, the width is not suitable
                    width = width*2;
                }else{
                    break;
                }
            }
        }else{
            llvm::APInt maxValue = APInt::getMaxValue(width).zext(currentW);
            //Gets maximum signed value of APInt for specific bit width.
            if(value.ugt(maxValue)){ //if current value is bigger than the max size, the width is not suitable
                width = width*2;
            }else{
                break;
            }
        }
    }
    return width;
}

//return if str <= range
bool compareStr(std::string str, std::string range){
    if(str==range)
        return true;
    if(str.length()<range.length())
        return true;
    if(str.length()>range.length())
        return false;
    for(int i=0;i<range.length();i++){
        if(str[i]-'0' > range[i]-'0'){
            return false;
        }
        if(str[i]-'0' < range[i]-'0'){
            return true;
        }
    }
    return true;
}
//return if in valid range
bool checkRange(std::string str, bool isSigned){
    std::string maxSigned = "170141183460469231731687303715884105727";
    std::string minSigned = "170141183460469231731687303715884105728";
    std::string maxUnsigned = "340282366920938463463374607431768211455";
                               
    bool isNeg = str[0]=='-';
    if(isNeg) str.erase(0,1);

    if(isNeg && isSigned){
        return compareStr(str,minSigned);
    }else if(!isNeg && isSigned){
        return compareStr(str,maxSigned);
    }else if(!isNeg && !isSigned){
        return compareStr(str,maxUnsigned);
    }else{
        Bug("unsigned negative number",0);
    }
}
//return if in valid range
bool checkRange(llvm::APInt apint, bool isSigned){

    int currentW= apint.getBitWidth();
    assert(currentW>128);  
    if(isSigned){
        if(apint.isNegative()){
            llvm::APInt minValue = APInt::getSignedMinValue(128).sext(currentW);
            //Gets maximum signed value of APInt for specific bit width.
            return apint.slt(minValue);
        }
        else{
            llvm::APInt maxValue = APInt::getSignedMaxValue(128).sext(currentW);
            //Gets maximum signed value of APInt for specific bit width.
            return apint.sgt(maxValue);
        }
    }else{
        llvm::APInt maxValue = APInt::getMaxValue(128).zext(currentW);
        //Gets maximum signed value of APInt for specific bit width.
        return apint.ugt(maxValue);
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

    llvm::DataLayout* dataLayOut = new llvm::DataLayout(TheModule.get());
    sizet = dataLayOut->getLargestLegalIntType(TheContext);
}

void isCheckLevelValid(){
    if( !doCheck[CheckLevel::check_array_bound] && doCheck[CheckLevel::check_free]){
        error("if you want to do free check, you must do array bound check together");
    }
}

//does user need dnamic check
void initCheck(std::string check){
    if(doCheck == NULL){
        doCheck = new bool[3];
        doCheck[CheckLevel::check_arith] = false;
        doCheck[CheckLevel::check_free] = false;
        doCheck[CheckLevel::check_array_bound] = false;
    }
    if(check == "DyCheck"){
        doCheck[CheckLevel::check_arith] = true;
        doCheck[CheckLevel::check_free] = true;
        doCheck[CheckLevel::check_array_bound] = true;
    }
    else if(check == "notDyCheck"){
        doCheck[CheckLevel::check_arith] = false;
        doCheck[CheckLevel::check_free] = false;
        doCheck[CheckLevel::check_array_bound] = false;
    }
    else if(check == "check_arith"){
        doCheck[CheckLevel::check_arith] = true;
    }
    else if(check == "check_free"){
        doCheck[CheckLevel::check_free] = true;
    }
    else if(check == "check_array_bound"){
        doCheck[CheckLevel::check_array_bound] = true;
    }
    else{
        error("unexpected flag: "+check+"\nonly allow check_arith, check_free, check_array_bound");
    }
}

//call the top level global variable init function
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

// when the constant number is signed(default), but another operand of leftValue is unsigned
//if valid, convert the constantType as IntType
QValue* constAdjustSign(QValue* num, bool isSigned){
    assert(num->getType()->isConstant());

    ConstantType* type = dynamic_cast<ConstantType*>(num->getType());

    if(isSigned==false){
        if(type->isNegative()){
            return NULL; // Cannot convert negative const to unsigned
        }
    }

    llvm::APInt alignedVal = type->getValue().getAlignedValue(isSigned);
    llvm::Value* constInt = ConstantInt::get(TheContext,alignedVal);
    IntType* t = new IntType(isSigned,alignedVal.getBitWidth()); //get suitable type

    return new QValue(t,constInt);

}

//just for int type
QValue* upCast(QValue* qv, IntType* type){
    llvm::Value* newQv = Builder.CreateIntCast(qv->getValue(),type->getLLVMType(),type->getSigned());
    IntType* newType = new IntType(type->getSigned(),type->getWidth());
    return new QValue(newType,newQv);
}

// for defAST, assignAST, returnAST, call argument; check if the width fix and do bit cast  
QValue* assignCast(QValue* varValue, QType* leftT){

    if(varValue->getType()!=NULL && leftT->getIsPointer() != varValue->getType()->getIsPointer()){
        return NULL;
    }

    if(varValue->getType()!=NULL && leftT->isString() != varValue->getType()->isString()){
        return NULL;
    }

    if(leftT->isString()){
        return varValue;
    }

    // variable or arrayIndex
    if(!leftT->getIsPointer()){

        if(varValue->getType()==NULL || varValue->getValue()==NULL){ 
            return NULL;
        }
        
        if(varValue->getType()->isConstant()){

            varValue =constAdjustSign(varValue,(dynamic_cast<IntType*>(leftT))->getSigned());
            
            if(!varValue){
                return NULL;
            }
        }
        IntType* leftInt = dynamic_cast<IntType*>(leftT);
        IntType* rightInt = dynamic_cast<IntType*>(varValue->getType());

        if(leftInt->getSigned()==false && leftInt->getWidth()==1){
            if(!leftInt->compare(rightInt)){
                return NULL;
            }
        }
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

            PointType* leftP = dynamic_cast<PointType*>(leftT);
            PointType* nullPtr = new PointType(leftP->getElementType(),true);
            varValue = new QValue(nullPtr, Constant::getNullValue(leftT->getLLVMType()));
        }
        else{

            PointType* leftP = dynamic_cast<PointType*>(leftT);
            PointType* rightP = dynamic_cast<PointType*>(varValue->getType());
            if(!(rightP->isNull()) && !(leftP->compare(rightP))){
                return NULL;
            }
            if(leftP->isNull()){
                leftP->setNull( rightP->isNull());
            }
        }
    }

    return varValue;
}

//call error and exit function when overflow
void callError(std::string info, int line){

   // llvm::DataLayout* dataLayOut = new llvm::DataLayout(TheModule.get());
   // Type* qtype = dataLayOut->getLargestLegalIntType(TheContext);

    std::vector<llvm::Type*> args;
    args.push_back(Type::getInt8PtrTy(TheContext));
    args.push_back(sizet); //line number
    llvm::Type* returnT = sizet; //return type
    FunctionType *FT = FunctionType::get(returnT,args,false);
    FunctionCallee PrintFunc = TheModule->getOrInsertFunction("printf", FT);

    std::string str = info + " at line: %d\n";
    Constant *lineN = ConstantInt::get(sizet, line);

   // const char *str_ptr = str.c_str();
    Value *globalStrPtr = scope.findStr(str);
    if(globalStrPtr==NULL){
        globalStrPtr = Builder.CreateGlobalStringPtr(str);
        scope.addStr(str,globalStrPtr);
    }

    std::vector<Value*> ArgsV;
    ArgsV.push_back(globalStrPtr);
    ArgsV.push_back(lineN);

    Function *F = dyn_cast<Function>(PrintFunc.getCallee());
    assert(F);
    Builder.CreateCall(F,ArgsV);

    //call exit
    std::vector<llvm::Type*> exitargs;
    exitargs.push_back(sizet);
    llvm::Type* exitreturnT = llvm::Type::getVoidTy(TheContext);
    FunctionType *exitFT = FunctionType::get(exitreturnT,exitargs,false);
    FunctionCallee exitFunc = TheModule->getOrInsertFunction("exit", exitFT);
    llvm::Value* one = ConstantInt::get(sizet, 1);

    if (Function *F = dyn_cast<Function>(exitFunc.getCallee())) {
       Builder.CreateCall(F,one);
    }

    Builder.CreateUnreachable();
}


//   ///////////////////          save old code          /////////////////   //
//used when constant num is a operand of binaty operator and anther operand is not contant
//I hope to make the type of constant num becomes IntType
/*int getSuitableWidth(long long num, bool isSigned){
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

}*/

//only be used when the constant number is the value of global var 
/*llvm::Constant* alignConst(long long value, QType* type){

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
}*/

/*Constant* geti8StrVal(char const* str, Type* t, std::string name) {
    //error
    //GlobalVariable* GVStr = TheModule->getGlobalVariable(name, true); //true allow internal global var
    Constant* strConstant = ConstantDataArray::getString(TheContext, str);
    auto GVStr = TheModule->getOrInsertGlobal(name, strConstant->getType());
   
    Builder.CreateStore(strConstant, GVStr);
    
    Constant* zero = Constant::getNullValue(t);
    Constant* indices[] = {zero, zero};
    Constant* strVal = ConstantExpr::getGetElementPtr(strConstant->getType(),GVStr, indices, true);
    return strVal;
} */
