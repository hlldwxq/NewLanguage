#include "../header/Operator.h"
#include "../header/AST.h"
#include "../header/ASTExpr.h"

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

llvm::Value* division::OverFlowCheck(QValue* left,QValue* right,std::string bbName, std::string normalBBname){
    //Function *TheFunction = Builder.GetInsertBlock()->getParent();

    // check div 0
    llvm::Value* zero = ConstantInt::get(right->getType()->getLLVMType(), 0);
    llvm::Value* ifDivZero = Builder.CreateICmpEQ(right->getValue(), zero, "cmptmp");
    createBr("division zero",ifDivZero, line, "division0bb", "divisionNormal");
   
    //check overflow
    IntType* leftT = dynamic_cast<IntType*>(left->getType());
    if(leftT->getSigned()!=false){
        
        // check if right is -1
        llvm::Value* negOne = ConstantInt::get(right->getType()->getLLVMType(), -1);
        llvm::Value* ifDivOne = Builder.CreateICmpEQ(right->getValue(), negOne, "cmptmp");

        // check if left is the min
        llvm::Value* negMin = ConstantInt::get(TheContext,APInt::getSignedMinValue(leftT->getWidth()));
        llvm::Value* ifMin = Builder.CreateICmpEQ(left->getValue(), negMin, "cmptmp");

        llvm::Value* isOverFlow = Builder.CreateAnd(ifDivOne,ifMin);

        createBr("overflow",isOverFlow, line, "arithOverFlow", "arithNormal");
    }

    return gen_llvm(leftT->getSigned(), left->getValue(), right->getValue());    
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

    if(type->isConstant()){
        ConstantType* constType = dynamic_cast<ConstantType*>(type);
        IntConst result = constType->getValue().uminus();
        ConstantType* resType = new ConstantType(result,line);
        llvm::Value* constInt =  ConstantInt::get(TheContext, result.getValue());
        return new QValue(resType,constInt);
    }
    IntType* intType = dynamic_cast<IntType*>(type);
    if(intType->getSigned()!=true){
        error("! with unsigned");
    }
    llvm::Value* rightV = operand->getValue();
    if(doCheck[CheckLevel::check_arith]){
        IntType* qType = dynamic_cast<IntType*>(operand->getType());
        Function *TheFunction = Builder.GetInsertBlock()->getParent();
        BasicBlock *overflowBB = BasicBlock::Create(TheContext, "overflow", TheFunction);
        BasicBlock *normalBB = BasicBlock::Create(TheContext, "normal",TheFunction);

        llvm::Value* negMin = ConstantInt::get(TheContext,APInt::getSignedMinValue(qType->getWidth()));
        llvm::Value* ifMin = Builder.CreateICmpEQ(rightV, negMin, "cmptmp");
        Builder.CreateCondBr(ifMin, overflowBB, normalBB);

        // overflow
        Builder.SetInsertPoint(overflowBB);
        callError("overflow",line);
        overflowBB = Builder.GetInsertBlock();  

        // normal
        Builder.SetInsertPoint(normalBB);
    }
    llvm::Value* minu = Builder.CreateNeg(rightV);
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
