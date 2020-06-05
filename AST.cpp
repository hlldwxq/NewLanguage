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

bool CompareQType(QType* q1, QType* q2){
    if(q1->getIsPointer() != q2->getIsPointer()){
        return false;
    }else{
        if(q1->getIsPointer()){
            PointType* Q1 = dynamic_cast<PointType*>(q1);
            PointType* Q2 = dynamic_cast<PointType*>(q2);
            return ComparePointType(Q1,Q2);
        }else{
            IntType* Q1 = dynamic_cast<IntType*>(q1);
            IntType* Q2 = dynamic_cast<IntType*>(q2);
            if(Q1->getSigned()!=Q2->getSigned() || Q1->getWidth()!=Q2->getWidth()){
                return false;
            }
            return true;
        }
    }
}

bool ComparePointType(PointType* p1,PointType* p2){

    if(p1->getElementType()->getIsPointer()!=p2->getElementType()->getIsPointer()){
        return false;
    }
    QType* elementP1 = p1->getElementType();
    QType* elementP2 = p2->getElementType();
    return CompareQType(elementP1,elementP2);
}

std::unique_ptr<NumberExprAST> getNumberAST(std::unique_ptr<ExprAST> number){
    NumberExprAST* ptr = dynamic_cast<NumberExprAST*>(number.get());
    std::unique_ptr<NumberExprAST> num;
    if (ptr != nullptr) {
        number.release();
        num.reset(ptr);
    }
    return num;
}

IntType* getSuitableType(long long num){
    int initSize = 8;
    while(initSize<=128){
        long long maxRange = (1<<(initSize-1))-1;
        long long minRange = -(1<<initSize);
        if(num<=maxRange && num>=minRange){
            break;
        }
        initSize*=2;
    }
    if(initSize>128)
        return NULL;
    else
    {
        return new IntType(true,initSize);
    }
    
}

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

QType* getBiggerIntType(QType* p1, QType* p2, long long numvalue){
    //The function of this function is simply to determine whether a constant 
    //or a numeric variable has a larger type value
    if(p1->getIsPointer() || p2->getIsPointer()){
        return NULL;
    }

    IntType* int1 = dynamic_cast<IntType*>(p1);
    IntType* int2 = dynamic_cast<IntType*>(p2);
    if(int1->getSigned()!=int2->getSigned()){
        if(numvalue<0)
            return NULL;
    }

    if(int1->getWidth()>int2->getWidth()){
        return p1;
    }else{
        return p2;
    }
}

llvm::Type* IntType::getLLVMType() const{
    check_valid();
    return Type::getIntNTy(TheContext,width);
}

llvm::PointerType* PointType::getLLVMType() const{

    return elementType->getLLVMType()->getPointerTo(0);   
}

llvm::Type* ReturnType::getLLVMType() const{
    if(isVoid){
        return llvm::Type::getVoidTy(TheContext);
    }else {
        return qType->getLLVMType();
    }
}

QValue* VariableAST::codegenLeft(){
    const QAlloca* Alloca = scope.findSymbol(name);

    if(!Alloca){
        const QGlobalVariable* global = scope.findGloableVar(name);
        if(!global){
            ErrorQ("Undeclared variables",line);
            return NULL;
        }
        QType* t = global->getType();
        GlobalVariable* v = global->getGlobalVariable();

        QValue* qv = new QValue(t,v);
        return qv;
    }else{
        QType* t = Alloca->getType();
        llvm::AllocaInst* v = Alloca->getAlloca();

        QValue* qv = new QValue(t,v);
        return qv;
    }
  
}

QValue* ArrayIndexExprAST::codegenLeft(){
    QValue* left = pointer->codegen();
    Value* arrIndex = index->codegen()->getValue();
	
    if(index->getType()==ASTType::number){
        /*NumberExprAST* leftnum = dynamic_cast<NumberExprAST*>(index.get());
        std::unique_ptr<NumberExprAST> ptr = getNumberAST(std::move(index));
        if (leftnum != nullptr) {
            index.release();
            ptr.reset(leftnum);
        }
        IntType* type = getSuitableType(ptr->getValue());
        arrIndex = ConstantInt::get(type->getLLVMType(),ptr->getValue(),false);*/
        std::unique_ptr<NumberExprAST> ptr = getNumberAST(std::move(index));
        if(ptr->getValue()<0){
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
    //elementT->printAST();
    return new QValue(elementT,eleptr);
 
}

QValue* LeftValueAST::codegen() {
    QValue* Alloca = codegenLeft();
    return new QValue(Alloca->getType(),Builder.CreateLoad(Alloca->getValue()));
}

QValue* NumberExprAST::codegen(){
    IntType* qtype = getSuitableType(value); 
    // in some cases, like array index, as long as the number is not negtive, 
    // its type is not important
    llvm::Value* constInt = ConstantInt::get(qtype->getLLVMType(),value);
    return new QValue(qtype,constInt);
}

QValue* UnaryExprAST::codegen(){
    QValue* value = Operand->codegen();
    if(!value){
        return NULL;
    }

    if(opCode == Operators::exclamation_point){
        QType* type = value->getType();
        if(type->getIsPointer()){
            ErrorQ("the right of the exclamation point cannot be a pointer",line);
            return NULL;
        }    

        IntType* intType = dynamic_cast<IntType*>(type);
        if(intType->getSigned()!=false || intType->getWidth()!=1){
            ErrorQ("the type of right of the exclamation point must be unsigned int of width 1",line);
            return NULL;
        }

        llvm::Value* realValue = Builder.CreateNot(value->getValue());
        return new QValue(type,realValue);

    }else if(opCode == Operators::minus){
        QType* type = value->getType();
        if(type->getIsPointer()){
            ErrorQ("the right of the minus cannot be a pointer",line);
            return NULL;
        }

        IntType* intType = dynamic_cast<IntType*>(type);
        if(intType->getSigned()!=true){
            ErrorQ("the type of right of the minus must be signed int",line);
            return NULL;
        }

        llvm::Value* minu = Builder.CreateNeg(value->getValue());
        return new QValue(type,minu);
    }else{
        ErrorQ("unvalid unary operator",line);
        return NULL;
    }
    return NULL;
}

QValue* BinaryExprAST::codegen(){
    
    llvm::Value* result;
    
    bool bothNum = false; // for both num
    long long lnum;
    long long rnum;
    long long resultNum;  // make sure that the type of result is correct; 127 + 127 -> sint16

    QValue* leftQV = LHS->codegen();
    QValue* rightQV = RHS->codegen();

    QType* leftT = leftQV->getType();
    QType* rightT = rightQV->getType();
    QType* resultType = leftT; //the resulttype may be changed

    if((leftT!=NULL&&leftT->getIsPointer()) || (rightT!=NULL&&rightT->getIsPointer())){
        ErrorQ("Variables on either side of a binary operator cannot be Pointers",line);
        return NULL;
    }
    
    llvm::Value* leftV = leftQV->getValue();
    llvm::Value* rightV = rightQV->getValue();
    
    if(LHS->getType() == ASTType::number && RHS->getType()!=ASTType::number){
        std::unique_ptr<NumberExprAST> num = getNumberAST(std::move(LHS));

        /* 
           sint8 var = 12
           sint16 var1 = 190[sint16] + var
        */
        QType* resType = getBiggerIntType(leftT,rightT,num->getValue());
        if(!resType){
            ErrorQ("A number on one side of a binary operator cannot be converted to the type of a variable on the other side",line);
            return NULL;
        }
        if(resType == leftT){
            IntType* oldType = dynamic_cast<IntType*>(rightT);
            rightV = Builder.CreateIntCast(rightV,resType->getLLVMType(),oldType->getSigned());
            
            // if the var is unsinged, the number type and result type also should be unsigned
            // so we cannot easily make the     result type = leftT
            if(!oldType->getSigned()){
                leftT = new IntType(oldType->getSigned(),(dynamic_cast<IntType*>(leftT))->getWidth());
            }
            rightT = leftT;
            resultType = leftT;
            
        }else{
            leftV = Builder.CreateIntCast(leftV,resType->getLLVMType(),(dynamic_cast<IntType*>(rightT))->getSigned());
            leftT = rightT;
            resultType = rightT;
        }
        
    }else if(LHS->getType() != ASTType::number && RHS->getType()==ASTType::number){

        std::unique_ptr<NumberExprAST> num = getNumberAST(std::move(RHS));
        QType* resType = getBiggerIntType(leftT,rightT,num->getValue());
        if(!resType){
            ErrorQ("A number on one side of a binary operator cannot be converted to the type of a variable on the other side",line);
            return NULL;
        }
        if(resType == rightT){
            IntType* oldType = dynamic_cast<IntType*>(leftT);
            leftV = Builder.CreateIntCast(leftV,resType->getLLVMType(),oldType->getSigned());
            if(!oldType->getSigned()){
                rightT = new IntType(oldType->getSigned(),(dynamic_cast<IntType*>(rightT))->getWidth());
            }
            leftT = rightT;
            resultType = rightT;
        }else{
            rightV = Builder.CreateIntCast(rightV,resType->getLLVMType(),(dynamic_cast<IntType*>(leftT))->getSigned());
            rightT = leftT;
            resultType = leftT;
        }
    }else if(LHS->getType() == ASTType::number && RHS->getType()==ASTType::number){
        // just need to record the value, donot need to convert the type
        std::unique_ptr<NumberExprAST> ptrL = getNumberAST(std::move(LHS));
        std::unique_ptr<NumberExprAST> ptrR = getNumberAST(std::move(RHS));

        bothNum = true;
        
        lnum = ptrL->getValue();
        rnum = ptrR->getValue();

    }

    IntType* leftInt = dynamic_cast<IntType*>(leftT);
    IntType* rightInt = dynamic_cast<IntType*>(rightT); 
    //a+b
   
    if(leftInt->getSigned() != rightInt->getSigned()){
        ErrorQ("unvalid binary calculation between signed number and unsigned number",line);
        return NULL;
    }

    bool isSigned = leftInt->getSigned();
    if(leftInt->getWidth()!=rightInt->getWidth()){

        unsigned long long widthL = leftInt->getWidth();
        unsigned long long widthR = rightInt->getWidth();

        if(widthL>widthR){
            leftV = leftQV->getValue();
            rightV = Builder.CreateIntCast(rightQV->getValue(),leftInt->getLLVMType(),isSigned);
            resultType = leftT;
        }else{
            rightV = rightQV->getValue();
            leftV = Builder.CreateIntCast(leftQV->getValue(),rightInt->getLLVMType(),isSigned);
            resultType = rightT;

        }
    }

    switch(op){   
        case Operators::equal_sign:  //==
            result = Builder.CreateICmpEQ(leftV, rightV, "cmptmp");
            resultType = new IntType(false,1);
            break;
        case Operators::not_equal:   //!=
            result = Builder.CreateICmpNE(leftV, rightV, "cmptmp");
            resultType = new IntType(false,1);
            break;
        case Operators::less_equal:  //<=
            result = Builder.CreateICmpULE(leftV, rightV, "cmptmp");
            resultType = new IntType(false,1);
            break;
        case Operators::more_equal:  //>=
            result = Builder.CreateICmpUGE(leftV, rightV, "cmptmp");
            resultType = new IntType(false,1);
            break;
        case Operators::plus_sign:   // +
            if(bothNum){
                resultNum = lnum + rnum;
                resultType = getSuitableType(resultNum);
            }
            else{
                result = Builder.CreateAdd(leftV, rightV, "addtmp");
            }
            break;
        case Operators::minus:       // -
            if(bothNum){
                resultNum = lnum - rnum;
                resultType = getSuitableType(resultNum);
            }else{
                result = Builder.CreateSub(leftV, rightV, "subtmp");
            }
            break;
        case Operators::star:        // *
            if(bothNum){
                resultNum = lnum * rnum;
                resultType = getSuitableType(resultNum);
            }else{
                result = Builder.CreateMul(leftV, rightV, "multmp");
            }
            break;
        case Operators::division:    // /
            if(bothNum){
                long long result = lnum / rnum;
                resultType = getSuitableType(result);
            }else{
                if(isSigned)
                    result = Builder.CreateSDiv(leftV, rightV, "divmp");
                else
                    result = Builder.CreateUDiv(leftV, rightV, "divmp");
            }
            break;
        case Operators::andT:        // &
            result = Builder.CreateAnd(leftV,rightV,"andmp");
            resultType = new IntType(false,1);
            break;
        case Operators::orT:         // |
            result = Builder.CreateOr(leftV,rightV,"ormp");
            resultType = new IntType(false,1);
            break;
        case Operators::more_sign:   // >
            result = Builder.CreateICmpUGT(leftV, rightV, "cmptmp");
            resultType = new IntType(false,1);
            break;
        case Operators::less_sign:   // <
            result = Builder.CreateICmpULT(leftV, rightV, "cmptmp");
            resultType = new IntType(false,1);
            break;
        default:
            ErrorQ("unvalis binary operator",line);
            return NULL;
    }

    if(bothNum){
        result = ConstantInt::get(TheContext, APInt((dynamic_cast<IntType*>(resultType))->getWidth(),resultNum));
    }
    QValue* v = new QValue(resultType,result);
    return v;
}

QValue* NewExprAST::codegen(){

    Type* t = Type::getIntNTy(TheContext,sizeof(size_t)*8);
    Value* arraySize = (size->codegen())->getValue();
        
    Value* mallocSize = ConstantExpr::getSizeOf(type->getLLVMType());
    mallocSize = ConstantExpr::getTruncOrBitCast(cast<Constant>(mallocSize),t);

    // out of memory ???
    Instruction* var_malloc = CallInst::CreateMalloc(Builder.GetInsertBlock(),t, type->getLLVMType(), mallocSize,arraySize,nullptr,"");
    Value* result = Builder.Insert(var_malloc);
    
   // type->printAST();
    return new QValue(type,result);
}

bool VarDefAST::codegenCommand(){
    
    llvm::AllocaInst* Alloca = Builder.CreateAlloca(type->getLLVMType(), ConstantInt::get(Type::getInt32Ty(TheContext), 1), name);
    QAlloca* allo = new QAlloca(type,Alloca);
    if(!scope.addSymbol(name,allo)){
        return false;
    }

    QValue* init = value->codegen();
    if(!init)
        return false;

    QType* initType = init->getType();
    llvm::Value* initValue = init->getValue();

    if(initType!=NULL&&initType->getIsPointer()){
        ErrorQ("pointer type cannot be converted to int type",line);
        return false;
    }

    if(value->getType()!=ASTType::number){
        IntType* initT= dynamic_cast<IntType*>(initType);
        if(initT->getSigned() != type->getSigned() ){
            ErrorQ("type cannot be converted",line);
            return false;
        }

        if(initT->getWidth() > type->getWidth()){
            ErrorQ("type cannot be converted",line);
            return false;
        }
        
        if(initT->getWidth() < type->getWidth()){
            bool isSigned = type->getSigned();
            initValue = Builder.CreateIntCast(initValue,type->getLLVMType(),isSigned);
        }
    }else{
        std::unique_ptr<NumberExprAST> ptrL = getNumberAST(std::move(value));
        initValue = alignConst(ptrL->getValue(),type);
        if(!initValue){
            ErrorQ("the type cannot been converted",line);
            return false;
        }
    }
 
    Value* store = Builder.CreateStore(initValue, Alloca);

    if(!store){
        Bug("failed store",0);
    }
    return true;
}

bool ArrayDefAST::codegenCommand(){
    
    llvm::AllocaInst* Alloca = Builder.CreateAlloca(type->getLLVMType(), ConstantInt::get(Type::getInt32Ty(TheContext), 1), name);
    QAlloca* allo = new QAlloca(type,Alloca);
    if(!scope.addSymbol(name,allo)){
        return false;
    }

    QValue* rightQv = value->codegen();
    QType* rightQt = rightQv->getType();
    Value* rightV = rightQv->getValue();

    if(!CompareQType(type->getElementType(),rightQt)){
        ErrorQ("The types on both sides of the equal sign must be the same",line);
        return false;
    }

    Value* store = Builder.CreateStore(rightV, Alloca);
    if(!store){
        Bug("failed store",1);
    }
    return true;
}

bool AssignAST::codegenCommand(){
    
    QValue* leftV = left->codegenLeft();

    if(!leftV)
        return false;

    llvm::Value* allocaL = leftV->getValue();
    QValue* rightV = right->codegen();
    if(!rightV)
        return false;

    llvm::Value* rightValue = rightV->getValue();

    QType* leftT = leftV->getType();
    QType* rightT = rightV->getType();

    if(rightT!=NULL && leftT->getIsPointer() != rightT->getIsPointer()){
        if(right->getType()!=ASTType::newT){
            ErrorQ("type cannot be converted",line);
            return false;
        }
    }
    // variable or arrayIndex
    if(!leftT->getIsPointer()){
        IntType* leftInt = dynamic_cast<IntType*>(leftT);
        IntType* rightInt;
        if(right->getType()!=ASTType::number){
            rightInt = dynamic_cast<IntType*>(rightT);
            if(leftInt->getSigned() != rightInt->getSigned()){
                ErrorQ("type cannot be converted",line);
                return false;
            }
            if(leftInt->getWidth()< rightInt->getWidth()){
                ErrorQ("type cannot be converted",line);
                return false;
            }
            if(leftInt->getWidth() > rightInt->getWidth()){
                bool isSigned = leftInt->getSigned();
                rightValue = Builder.CreateIntCast(rightV->getValue(),leftInt->getLLVMType(),isSigned);
            }
        }else{
            std::unique_ptr<NumberExprAST> anotherptr = getNumberAST(std::move(right));
            rightValue = alignConst(anotherptr->getValue(),leftInt);
            if(!rightValue){
                ErrorQ("the type cannot be converted",line);
                return false;
            }
        }
        
    }else{
        if(right->getType()==ASTType::newT){
            PointType* leftP = dynamic_cast<PointType*>(leftT);
            QType* qt = leftP->getElementType();
            if(!CompareQType(qt,rightT)){
                ErrorQ("type cannot be converted",line);
                return false;
            }
        }else{
            PointType* leftP = dynamic_cast<PointType*>(leftT);
            PointType* rightP = dynamic_cast<PointType*>(rightT);
            if(!ComparePointType(leftP,rightP)){
                ErrorQ("type cannot be converted",line);
                return false;
            }
        }
    }

    llvm::Value* store = Builder.CreateStore(rightValue, allocaL);
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
    Value* returnV = returnQv->getValue();

    if(value->getType()!=ASTType::number){
        QType* returnQt = returnQv->getType();
        QType* allocaQt = returnAlloca->getType();

        if(returnQt->getIsPointer()!=allocaQt->getIsPointer()){
            ErrorQ("unvlaid return value because of different type",line);
            return false;
        }

        if(!returnQt->getIsPointer()){
            IntType* returnInt = dynamic_cast<IntType*>(returnQt);
            IntType* allocaInt = dynamic_cast<IntType*>(allocaQt);
            if(returnInt->getSigned()!=allocaInt->getSigned()){
                ErrorQ("There is no conversion between signed and unsigned numbers",line);
                return false;
            }
            if(returnInt->getWidth()>allocaInt->getWidth()){
                ErrorQ("Implicit type down conversion is not allowed",line);
                return false;
            }
            if(returnInt->getWidth()<allocaInt->getWidth()){
                bool isSigned = returnInt->getSigned();
                returnV = Builder.CreateIntCast(returnV,allocaInt->getLLVMType(),isSigned);
            }

        }else{
            //loading
        }
    }else{
        /*NumberExprAST* ptr = dynamic_cast<NumberExprAST*>(value.get());
        std::unique_ptr<NumberExprAST> anotherptr;
        if (ptr != nullptr) {
            value.release();
            anotherptr.reset(ptr);
        }*/
        std::unique_ptr<NumberExprAST> anotherptr = getNumberAST(std::move(value));
        returnV = alignConst(anotherptr->getValue(),returnAlloca->getType());
        if(!returnV){
            ErrorQ("The return value type does not match",line);
            return false;
        }
    }
    
    Value* store = Builder.CreateStore(returnV, returnAlloca->getAlloca());
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
        ErrorQ("the return type of the function is void, cannot be the left value",line);
        return NULL;
    }

    if(func->arg_size()!=args.size()){
        ErrorQ("The number of participating arguments does not match",line);
        return NULL;
    }
    std::vector<Value*> ArgsV;

    for(int i=0 ; i<args.size(); i++){
        
        QType* needArgType = argsType[i];
        Value* realArg;
        if(args[i]->getType() == ASTType::number){
           /* NumberExprAST* arg = dynamic_cast<NumberExprAST*>(args[i].get());
            std::unique_ptr<NumberExprAST> argAST;
            if (arg != nullptr) {
                args[i].release();
                argAST.reset(arg);
            }*/
            std::unique_ptr<NumberExprAST> argAST = getNumberAST(std::move(args[i]));
            realArg = alignConst(argAST->getValue(),needArgType);
            if(!realArg){
                ErrorQ("The number of participating arguments does not match",line);
                return NULL;
            }
        }else{
            QValue* argValue = args[i]->codegen();
            realArg = argValue->getValue();
            QType* argType = argValue->getType();
            if(argType->getIsPointer()!=needArgType->getIsPointer()){
                ErrorQ("The number of participating arguments does not match",line);
                return NULL;
            }
            if(argType->getIsPointer()){
                if(!ComparePointType(dynamic_cast<PointType*>(argType),dynamic_cast<PointType*>(needArgType))){
                    ErrorQ("The number of participating arguments does not match",line);
                    return NULL;
                }
            }else{
                IntType* needIntType = dynamic_cast<IntType*>(needArgType);
                IntType* argIntType = dynamic_cast<IntType*>(argType);
                if(needIntType->getSigned()!=argIntType->getSigned()){
                    ErrorQ("The number of participating arguments does not match",line);
                    return NULL;
                }
                if(needIntType->getWidth()<argIntType->getWidth()){
                    ErrorQ("The number of participating arguments does not match",line);
                    return NULL;
                }
                if(needIntType->getWidth()>argIntType->getWidth()){
                    realArg = Builder.CreateIntCast(realArg,needIntType->getLLVMType(),needIntType->getSigned());
                }
            }
        }
        ArgsV.push_back(realArg);
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

bool VarDefAST::codegenStructure(){
    
    llvm::Constant* constantV;
    if(value->getType()==ASTType::number){
        std::unique_ptr<NumberExprAST> num = getNumberAST(std::move(value));
        constantV = alignConst(num->getValue(),type);
        if(!constantV){
            ErrorQ("the type cannot be coverted",line);
            return false;
        }
    }else{
        ErrorQ("the value of gloable variable must be constant",line);
        return false;
    }

    GlobalVariable * gloabalV = new GlobalVariable(type->getLLVMType(),false, 
                                    GlobalValue::CommonLinkage,constantV,name); 

    TheModule->getGlobalList().push_back(gloabalV);
    if(!scope.addGloabalVar(name,new QGlobalVariable(type,gloabalV))){
        ErrorQ("the gloabal variable has been declared",line);
        return false;
    }
    return true;
}

bool ArrayDefAST::codegenStructure(){
    
    llvm::Constant* constantP;
    constantP = ConstantPointerNull::get(type->getLLVMType());
    
    GlobalVariable * gloabalV = new GlobalVariable(type->getLLVMType(),false, 
                                    GlobalValue::CommonLinkage,constantP,name); 
    TheModule->getGlobalList().push_back(gloabalV);
    if(!scope.addGloabalVar(name,new QGlobalVariable(type,gloabalV))){
        ErrorQ("the gloabal variable has been declared",line);
        return false;
    }

    if(value->getType()!=ASTType::nullT){
        std::vector<llvm::Type*> args;
    
        llvm::Type* returnT = llvm::Type::getVoidTy(TheContext);
        FunctionType *FT = FunctionType::get(returnT,args,false);
        Function *F = Function::Create(FT,Function::InternalLinkage, "init_global_array_"+name, TheModule.get());

        BasicBlock *BB = BasicBlock::Create(TheContext, "entry", F);
	    Builder.SetInsertPoint(BB);
        
        QValue* rightQv = value->codegen();
        QType* rightQt = rightQv->getType();
        Value* rightV = rightQv->getValue();
        if(!CompareQType(type->getElementType(),rightQt)){
            ErrorQ("The types on both sides of the equal sign must be the same",line);
            return false;
        }

        Builder.CreateStore(rightV, gloabalV);
        Builder.CreateRetVoid();

        scope.addInitFunction(F);
        
    }
    
    return true;
}

/*

  align_const (c, type)
    check that c fits into type
    return (QValue(type,LLVM_get_const(type->llvm_type(),c)))

  align expr1, expr2

    qv1 = expr1->codegen
    qv2 = expr2->codegen

     check that qv1, qv2 are integers

     // Const align
     if expr1.is_const && !expr2.is_const
       qv1 = align_const(expr1.get_const(),qv2.type)
     else if !qv1.is_const && qv2.is_const
       qv1 = align_const(expr2.get_const(),qv1.type)

    // signed check
    check qv1.signed == qv2.signed

     v1 = qv1.codegen()
     v2 = qv2.codegen()

    // operand align
    if (qv1.width < qv2.width)
      v1 = llvm_extend_to(v1,qv2.type.llvm_type())  // createIntCast
    else if (qv2.width < qv1.width)
      v2 = extend_to(v2,qv1.type.llvm_type())       // createIntCast

    // Now v1, v2 are of same width and sign

    generate llvm instruction with v1,v2

*/