#include "AST.h"
#include "Scope.h"
#include <assert.h>
std::unique_ptr<Module> TheModule;


bool ComparePointType(PointType* p1,PointType* p2){
    PointType* copyP1 = p1;
    PointType* copyP2 = p2;

    while(true){
        if(copyP1->getElementType()->getIsPointer()!=copyP2->getElementType()->getIsPointer()){
            return false;
        }else if(copyP1->getElementType()->getIsPointer()){
            copyP1 = dynamic_cast<PointType*>(copyP1->getElementType());
            copyP2 = dynamic_cast<PointType*>(copyP2->getElementType());
        }else{
            IntType* ip1 = dynamic_cast<IntType*>(copyP1->getElementType());
            IntType* ip2 = dynamic_cast<IntType*>(copyP2->getElementType());
            if(ip1->getSigned()!=ip2->getSigned() || ip1->getWidth()!=ip2->getWidth()){
                return false;
            }else{
                return true;
            }
            break;
        }
    }
    return true;
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

llvm::Type* IntType::getLLVMType() const{
    check_valid();
    return Type::getIntNTy(TheContext,width);

//     switch(width){
//         case 1:
//             return Type::getInt1Ty(TheContext);
//         case 8:
//             return Type::getInt8Ty(TheContext);
//         case 16:
//             return Type::getInt16Ty(TheContext);
//         case 32:
//             return Type::getInt32Ty(TheContext);
//         case 64:
//             return Type::getInt64Ty(TheContext);
//         case 128:
//             return Type::getIntNTy(TheContext,128);
//         default:
//             Bug("unvalid int width",0);
//             exit(1);
//     }
}

llvm::PointerType* PointType::getLLVMType() const{

    return elementType->getLLVMType()->getPointerTo(0); // TODO Is it correct to set address space to 0 here?


//     return PointerType::get(elementType->getLLVMType());

//     if(elementType->getIsPointer()){
//         PointType* pointT = dynamic_cast<PointType*>(elementType);
//         llvm::PointerType* pt = PointerType::get(pointT->getLLVMType(),pointT->getLLVMType()->getAddressSpace());
//         return pt;
//     }else{
//         IntType* intT = dynamic_cast<IntType*>(elementType);
//         llvm::PointerType* pt = Type::getIntNPtrTy(TheContext,intT->getWidth());
//         return pt;
//     }
    
}

llvm::Type* ReturnType::getLLVMType() const{
    if(isVoid){
        return llvm::Type::getVoidTy(TheContext);
    }else {
        return qType->getLLVMType();
    }
}

QValue* VarOrPointerAST::codegenLeft(){
    const QAlloca* Alloca = findSymbol(name);

    if(!Alloca){
        ErrorQ("Undeclared variables",0);
        return NULL;
    }

    QType* t = Alloca->getType();
    llvm::AllocaInst* v = Alloca->getAlloca();

    QValue* qv = new QValue(t,v);
    return qv;
}

QValue* ArrayIndexExprAST::codegenLeft(){
    QValue* left = pointer->codegen();
    Value* arrIndex = index->codegen()->getValue();
	
    /*if(pointer->getType()==ASTType::arrayIndex){
        ArrayIndexExprAST* ptr = dynamic_cast<ArrayIndexExprAST*>(pointer.get());
        std::unique_ptr<ArrayIndexExprAST> leftptr;
        if (ptr != nullptr) {
            pointer.release();
            leftptr.reset(ptr);
        }
        left = leftptr->codegenLeft();

    }else if(pointer->getType()==ASTType::var_or_pointer){
        VarOrPointerAST* ptr = dynamic_cast<VarOrPointerAST*>(pointer.get());
        std::unique_ptr<VarOrPointerAST> leftptr;
        if (ptr != nullptr) {
            pointer.release();
            leftptr.reset(ptr);
        }
        left = leftptr->codegenLeft();
    }else{
        left = pointer->codegen();
    }*/

    if(!left->getType()->getIsPointer()){
        ErrorQ("left expression must be a pointer",0);
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


// QValue* VarOrPointerAST::codegen(){
//
//     QValue *leftv = codegenLeft();
//     llvm::Value* load = Builder.CreateLoad(leftv->getValue());
//
//     return new QValue(leftv->getType(),load);
//
//     const QAlloca* Alloca = findSymbol(name);
//
//     if(!Alloca){
//         ErrorQ("Undeclared variables",0);
//         return NULL;
//     }
//
//     llvm::AllocaInst* v = Alloca->getAlloca();
//     llvm::Value* load = Builder.CreateLoad(v);
//     QType* type = Alloca->getType();
//     return new QValue(type,load);
// }
//
// QValue* ArrayIndexExprAST::codegen(){
//     QValue* Alloca = codegenLeft();
//     return new QValue(Alloca->getType(),Builder.CreateLoad(Alloca->getValue()));
// }

QValue* NumberExprAST::codegen(){
    llvm::Value* constInt;
    QType* qtype // = this->ty;
    if(value>=0){
        constInt = ConstantInt::get(TheContext, APInt(128,value));
        qtype = new IntType(false,128);
    }else{
        constInt = ConstantInt::get(TheContext, APInt(128,value,true));
        qtype = new IntType(true,128);
    }
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
            ErrorQ("the right of the exclamation point cannot be a pointer",0);
            return NULL;
        }    

        IntType* intType = dynamic_cast<IntType*>(type);
        if(intType->getSigned()!=false || intType->getWidth()!=1){
            ErrorQ("the type of right of the exclamation point must be unsigned int of width 1",0);
            return NULL;
        }

        llvm::Value* realValue = Builder.CreateNot(value->getValue());
        return new QValue(type,realValue);

    }else if(opCode == Operators::minus){
        QType* type = value->getType();
        if(type->getIsPointer()){
            ErrorQ("the right of the minus cannot be a pointer",0);
            return NULL;
        }

        IntType* intType = dynamic_cast<IntType*>(type);
        if(intType->getSigned()!=true){
            ErrorQ("the type of right of the minus must be signed int",0);
            return NULL;
        }

        llvm::Value* minu = Builder.CreateNeg(value->getValue());
        return new QValue(type,minu);
    }else{
        ErrorQ("unvalid unary operator",0);
        return NULL;
    }
    return NULL;
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


QValue* BinaryExprAST::codegen(){
    
    llvm::Value* result;
    
    QValue* leftQV = LHS->codegen();
    QValue* rightQV = RHS->codegen();

    QType* leftT = leftQV->getType();
    QType* rightT = rightQV->getType();

    QType* resultType = leftT;
    // a+b :: intN * intN -> intN
    // a<b :: intN * intN -> bool

    // TODO "ptr1 - ptr2" only makes sense if you also have "ptr1 + intN". Maybe do not support minus for ptrs!
    if(leftT->getIsPointer()==true && rightT->getIsPointer() && op==Operators::minus){
        return NULL;
    }else if(leftT->getIsPointer()==false && rightT->getIsPointer()==false){
        
        llvm::Value* leftV = leftQV->getValue();
        llvm::Value* rightV = rightQV->getValue();
        
        IntType* leftInt = dynamic_cast<IntType*>(leftT);
        IntType* rightInt = dynamic_cast<IntType*>(rightT); 

        bool isSigned = true;

        //a+b
        if(LHS->getType()!=ASTType::number && RHS->getType()!=ASTType::number){
            if(leftInt->getSigned() != rightInt->getSigned()){
                ErrorQ("unvalid binary calculation between signed number and unsigned number",0);
                return NULL;
            }
            isSigned = leftInt->getSigned();
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
            }else if( (leftInt->getWidth()==rightInt->getWidth()) && leftInt->getWidth()==1){
                
                if(op == Operators::andT){
                    //??
                }else if(op == Operators::orT){
                    //??
                }

            }
        }else{
            //a+6 or 6+a ;  a+(-6)  a must be signed ;  
            if(LHS->getType()!=ASTType::number){
                NumberExprAST* ptr = dynamic_cast<NumberExprAST*>(RHS.get());
                std::unique_ptr<NumberExprAST> anotherptr;
                if (ptr != nullptr) {
                    RHS.release();
                    anotherptr.reset(ptr);
                }
                isSigned = leftInt->getSigned();
                if(!isSigned && anotherptr->getValue()<0){
                    ErrorQ("unvalid binary calculation between signed number and unsigned number",1);
                    return NULL;
                }
                resultType = leftT;
            }else if(RHS->getType()!=ASTType::number){
                isSigned = rightInt->getSigned();
                NumberExprAST* ptr = dynamic_cast<NumberExprAST*>(LHS.get());
                std::unique_ptr<NumberExprAST> anotherptr;
                if (ptr != nullptr) {
                    LHS.release();
                    anotherptr.reset(ptr);
                }
                if(!isSigned && anotherptr->getValue()<0){
                    ErrorQ("unvalid binary calculation between signed number and unsigned number",2);
                    return NULL;
                }
                resultType = rightT;
            }
        }

        switch(op){   
            case Operators::equal_sign:  //==
                result = Builder.CreateICmpEQ(leftV, rightV, "cmptmp");
                break;
            case Operators::not_equal:   //!=
                result = Builder.CreateICmpNE(leftV, rightV, "cmptmp");
                break;
            case Operators::less_equal:  //<=
                result = Builder.CreateICmpULE(leftV, rightV, "cmptmp");
                break;
            case Operators::more_equal:  //>=
                result = Builder.CreateICmpUGE(leftV, rightV, "cmptmp");
                break;
            case Operators::plus_sign:   //+
                result = Builder.CreateAdd(leftV, rightV, "addtmp");
                break;
            case Operators::minus:       //-
                result = Builder.CreateSub(leftV, rightV, "subtmp");
                break;
            case Operators::star:        //*
                result = Builder.CreateMul(leftV, rightV, "multmp");
                break;
            case Operators::division:    // /
                if(isSigned)
                    result = Builder.CreateSDiv(leftV, rightV, "divmp");
                else
                    result = Builder.CreateUDiv(leftV, rightV, "divmp");
                break;
            case Operators::more_sign:   // >
                result = Builder.CreateICmpUGT(leftV, rightV, "cmptmp");
                break;
            case Operators::less_sign:   // <
                result = Builder.CreateICmpULT(leftV, rightV, "cmptmp");
                break;
            default:
                ErrorQ("unvalis binary operator",0);
                return NULL;
        }

    }else{
        ErrorQ("unvalid binary calculation between different types",0);
        return NULL;
    }
    return new QValue(resultType,result);
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
    if(!addSymbol(name,allo)){
        return false;
    }

    QValue* init = value->codegen();
    if(!init)
        return false;
    
    QType* initType = init->getType();
    llvm::Value* initValue = init->getValue();

    if(initType->getIsPointer()){
        ErrorQ("pointer type cannot be converted to int type",0);
        return false;
    }

    if(value->getType()!=ASTType::number){
        IntType* initT= dynamic_cast<IntType*>(initType);
        if(initT->getSigned() != type->getSigned() ){
            ErrorQ("type cannot be converted",0);
            return false;
        }

        if(initT->getWidth() > type->getWidth()){
            ErrorQ("type cannot be converted",3);
            return false;
        }
        
        if(initT->getWidth() < type->getWidth()){
            bool isSigned = type->getSigned();
            initValue = Builder.CreateIntCast(initValue,type->getLLVMType(),isSigned);
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
    if(!addSymbol(name,allo)){
        return false;
    }

    QValue* rightQv = value->codegen();
    QType* rightQt = rightQv->getType();
    Value* rightV = rightQv->getValue();

    if(!rightQt->getIsPointer()){
        ErrorQ("the right value must be a pointer",0);
        return false;
    }
    PointType* rightPt = dynamic_cast<PointType*>(rightQt);
    if(ComparePointType(type,rightPt)){
        ErrorQ("The types on both sides of the equal sign must be the same",0);
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

    if(leftT->getIsPointer() != rightT->getIsPointer()){
        if(right->getType()!=ASTType::newT){
            ErrorQ("type cannot be converted",4);
            return false;
        }
    }
    // variable or arrayIndex
    if(!leftT->getIsPointer()){
        IntType* leftInt = dynamic_cast<IntType*>(leftT);
        IntType* rightInt = dynamic_cast<IntType*>(rightT);
        
        if(right->getType()!=ASTType::number){
            if(leftInt->getSigned() != rightInt->getSigned()){
                ErrorQ("type cannot be converted",1);
                return false;
            }
            if(leftInt->getWidth()< rightInt->getWidth()){
                ErrorQ("type cannot be converted",2);
                return false;
            }
            if(leftInt->getWidth() > rightInt->getWidth()){
                bool isSigned = leftInt->getSigned();
                rightValue = Builder.CreateIntCast(rightV->getValue(),rightInt->getLLVMType(),isSigned);
            }
        }

        if(!leftInt->getSigned() && right->getType()==ASTType::number){
            NumberExprAST* ptr = dynamic_cast<NumberExprAST*>(right.get());
            std::unique_ptr<NumberExprAST> anotherptr;
            if (ptr != nullptr) {
                right.release();
                anotherptr.reset(ptr);
            }
            if(anotherptr->getValue()<0){
                ErrorQ("cannot assign negative Numbers to unsigned value",1);
                return false;
            }
        }
        
    }else{
        if(right->getType()==ASTType::newT){
            PointType* leftP = dynamic_cast<PointType*>(leftT);
            QType* qt = leftP->getElementType();
            if(!CompareQType(qt,rightT)){
                ErrorQ("type cannot be converted",6);
                return false;
            }
        }else{
            PointType* leftP = dynamic_cast<PointType*>(leftT);
            PointType* rightP = dynamic_cast<PointType*>(rightT);
            if(!ComparePointType(leftP,rightP)){
                ErrorQ("type cannot be converted",5);
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
    const QAlloca* returnAlloca = findSymbol("return");
    if(!returnAlloca){
        ErrorQ("The function does not need return value",0);
        return false;
    }
    QValue* returnQv = value->codegen();
    Value* returnV = returnQv->getValue();

    if(value->getType()!=ASTType::number){
        QType* returnQt = returnQv->getType();
        QType* allocaQt = returnAlloca->getType();

        if(returnQt->getIsPointer()!=allocaQt->getIsPointer()){
            ErrorQ("unvlaid return value because of different type",0);
            return false;
        }

        if(!returnQt->getIsPointer()){
            IntType* returnInt = dynamic_cast<IntType*>(returnQt);
            IntType* allocaInt = dynamic_cast<IntType*>(allocaQt);
            if(returnInt->getSigned()!=allocaInt->getSigned()){
                ErrorQ("There is no conversion between signed and unsigned numbers",0);
                return false;
            }
            if(returnInt->getWidth()>allocaInt->getWidth()){
                ErrorQ("Implicit type down conversion is not allowed",0);
                return false;
            }
            if(returnInt->getWidth()<allocaInt->getWidth()){
                bool isSigned = returnInt->getSigned();
                returnV = Builder.CreateIntCast(returnV,allocaInt->getLLVMType(),isSigned);
            }

        }else{
            //loading
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

    bool success = addFunction(Name,functionQ);
    if(!success){
        ErrorQ("the function has been declared",0);
        return false;
    }
    return true;
}

bool FunctionAST::codegenStructure(){
    
    Proto->codegenStructure();
    Function* function = getFunction(Proto->getFunctionName())->getFunction();
    
    auto &P = *Proto;

    BasicBlock *BB = BasicBlock::Create(TheContext, "entry", function);
	Builder.SetInsertPoint(BB);

    addScope();
    llvm::AllocaInst* retAlloca;
    if(!P.getReturnType()->getIsVoid()){
        retAlloca = Builder.CreateAlloca(P.getReturnType()->getLLVMType(), ConstantInt::get(Type::getInt32Ty(TheContext), 1));
        addSymbol("return",new QAlloca(P.getReturnType()->getType(),retAlloca));
    }
    std::vector<std::pair<std::string,QType*>> args = P.getArgs();
    int i=0;
    for(auto &Arg : function->args()){
        std::string name = args[i].first;
        QType* t = args[i].second; 
        llvm::AllocaInst* Alloca = Builder.CreateAlloca(t->getLLVMType(), ConstantInt::get(Type::getInt32Ty(TheContext), 1), name);
        QAlloca* allo = new QAlloca(t, Alloca);
        if(!addSymbol(name,allo)){
            ErrorQ("the identifier has been declared",0);
            return false;
        }
        Builder.CreateStore(&Arg, Alloca);
        i++;
    }
    Body->codegenCommand();
    if(P.getReturnType()->getIsVoid())
        Builder.CreateRetVoid();
    else{
        llvm::Value* retValue = Builder.CreateLoad(retAlloca);
		Builder.CreateRet(retValue);
    }
    return true;
}

QValue* NullExprAST::codegen(){
    return NULL;
}

QValue* CallExprAST::codegen(){
    return NULL;
}

bool CallExprAST::codegenCommand(){
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
    return true;
}

bool ArrayDefAST::codegenStructure(){
    return true;
}
