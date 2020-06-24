#include "../header/Operator.h"
#include "../header/AST.h"

//overflow
Function* plus::overFlowDeclare(std::vector<Type*> args_type, bool isSigned){
    Function* overFlow;
    if(isSigned)
        overFlow = llvm::Intrinsic::getDeclaration(TheModule.get(), llvm::Intrinsic::sadd_with_overflow,args_type);
    else
        overFlow = llvm::Intrinsic::getDeclaration(TheModule.get(), llvm::Intrinsic::uadd_with_overflow,args_type);
    
    return overFlow;
}

Function* minus::overFlowDeclare(std::vector<Type*> args_type, bool isSigned){
    Function* overFlow;
    if(isSigned)
        overFlow = llvm::Intrinsic::getDeclaration(TheModule.get(), llvm::Intrinsic::ssub_with_overflow,args_type);
    else
        overFlow = llvm::Intrinsic::getDeclaration(TheModule.get(), llvm::Intrinsic::usub_with_overflow,args_type);
    
    return overFlow;
}

Function* star::overFlowDeclare(std::vector<Type*> args_type, bool isSigned){
    Function* overFlow;
    if(isSigned)
        overFlow = llvm::Intrinsic::getDeclaration(TheModule.get(), llvm::Intrinsic::smul_with_overflow,args_type);
    else
        overFlow = llvm::Intrinsic::getDeclaration(TheModule.get(), llvm::Intrinsic::umul_with_overflow,args_type);
    
    return overFlow;
}

Function* division::overFlowDeclare(std::vector<Type*> args_type, bool isSigned){
    return NULL;
}

Function* andT::overFlowDeclare(std::vector<Type*> args_type, bool isSigned){
    return NULL;
}

Function* orT::overFlowDeclare(std::vector<Type*> args_type, bool isSigned){
    return NULL;
}

//codegen & gen_llvm
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

Value* plus::gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right){
    return Builder.CreateAdd(left, right, "addtmp");
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