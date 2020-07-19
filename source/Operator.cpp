#include "../header/Operator.h"
#include "../header/AST.h"
#include "../header/ASTExpr.h"

/*
bool bigger(std::string left,std::string right){
    assert(left!=right);
    bool LisPos = left[0] == '+';
    bool RisPos = right[0] == '+';
    if(LisPos && (!RisPos))
        return true;
    if(!LisPos && RisPos)
        return false;

    // symbols are the same
    bool result=true;
    if(left!="0"){
        left.erase(0,1);
    }
    if(right!="0"){
        right.erase(0,1);
    }

    if(left.size()>right.size()){
        result = true;
    }else if(left.size()<right.size()){
        result = false;
    }else{
        for( int i=0 ; i<left.size() ; i++ ){
            if(left[i]>right[i]){
                result = true;
                break;
            }else if(left[i]<right[i]){
                result = false;
                break;
            }
        }
    }
    result = result & LisPos;
    return result;
}

std::string getZero(int size){
	std::string result = "";
	for(int i=0;i<size;i++){
		result += '0';
	}
	return result;
}
std::string removeExtra0(std::string s){
    std::string pre="";
    if(s[0]=='+'||s[0]=='-'){
        pre+=s[0];
        s.erase(0,1);
    }
    //keep the last 0 if it is 00000
    while(s.size()>1 && s[0]=='0'){
        s.erase(0,1);
    }
    if(s=="0")
        return s;
    return pre+s;
}
std::string plus1(std::string l,std::string r){
	int carry = 0;
	int length;
	std::string result = "";

    if(l=="0"){
        return r;
    }else if(r=="0"){
        return l;
    }
    bool isLP = (l[0]=='+');
    bool isRP = (r[0]=='+');

    l.erase(0,1);
    r.erase(0,1);
      
	int sizeGap;
	if(l.size()>r.size()){
		length = l.size();
		sizeGap = l.size() - r.size();
		r = getZero(sizeGap)+r;
	}else if(r.size()>l.size()){
		length = r.size();
		sizeGap = r.size() - l.size();
		l = getZero(sizeGap)+l;
	}else{
		length = l.size();
	}
	if(isLP == isRP){
		for(int i=length-1;i>=0;i--){
			int left = l[i]-'0';
			int right = r[i]-'0';
			int res = left + right + carry;
			carry = res/10;
			res = res - carry*10;
			result = std::to_string(res)+result;
		}
		if(carry>0){
			result = '1' + result;
		}
		if(!isLP){
			result = '-' + result;
		}else{
            result = '+' + result;
        }
	}else{
        if(l==r)
            return "0";
		if(!bigger("+"+l,"+"+r)){
			std::string temp = r;
			r = l;
			l = temp;
			isLP = !(isLP);
			isRP = !(isRP);
		}
		for(int i=length-1;i>=0;i--){
			int left = l[i]-'0';
			int right = r[i]-'0';
			int res = left - right - carry;
			carry = res<0;
			res = res + carry*10;
			if(!(res==0 && i==0))
				result = std::to_string(res)+result;
		}
		
		
		if(!isLP)
			result = '-' + result;
        if(isLP)
            result = '+' + result;
        result = removeExtra0(result);
	}
	return result;
}*/

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

llvm::Value* division::OverFlowCheck(QValue* left,QValue* right){
    Function *TheFunction = Builder.GetInsertBlock()->getParent();

    // check div 0
    BasicBlock *div0BB = BasicBlock::Create(TheContext, "divsion0", TheFunction);
    BasicBlock *divnormalBB = BasicBlock::Create(TheContext, "divnormal",TheFunction);

    llvm::Value* zero = ConstantInt::get(right->getType()->getLLVMType(), 0);
    llvm::Value* ifDivZero = Builder.CreateICmpEQ(right->getValue(), zero, "cmptmp");
    Builder.CreateCondBr(ifDivZero, div0BB, divnormalBB);

    // div 0
    Builder.SetInsertPoint(div0BB);
    callError("division zero",line);
    div0BB = Builder.GetInsertBlock();  

    // normal
    Builder.SetInsertPoint(divnormalBB);
    //check overflow
    IntType* leftT = dynamic_cast<IntType*>(left->getType());
    if(leftT->getSigned()!=false){
    
        BasicBlock *overflowBB = BasicBlock::Create(TheContext, "overflow", TheFunction);
        BasicBlock *normalBB = BasicBlock::Create(TheContext, "normal",TheFunction);
        
        // check if right is -1
        llvm::Value* negOne = ConstantInt::get(right->getType()->getLLVMType(), -1);
        llvm::Value* ifDivOne = Builder.CreateICmpEQ(right->getValue(), negOne, "cmptmp");

        // check if left is the min
        llvm::Value* negMin = ConstantInt::get(TheContext,APInt::getSignedMinValue(leftT->getWidth()));
        llvm::Value* ifMin = Builder.CreateICmpEQ(left->getValue(), negMin, "cmptmp");

        llvm::Value* isOverFlow = Builder.CreateAnd(ifDivOne,ifMin);
        Builder.CreateCondBr(isOverFlow, overflowBB, normalBB);

        // overflow
        Builder.SetInsertPoint(overflowBB);
        callError("overflow",line);
        overflowBB = Builder.GetInsertBlock();  

        // normal
        Builder.SetInsertPoint(normalBB);
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
        ConstantType* resType = new ConstantType(result);
        llvm::Value* constInt =  ConstantInt::get(TheContext, result.getValue());
        return new QValue(resType,constInt);
    }
    IntType* intType = dynamic_cast<IntType*>(type);
    if(intType->getSigned()!=true){
        error("! with unsigned");
    }
    llvm::Value* rightV = operand->getValue();
    if(doCheck){
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
