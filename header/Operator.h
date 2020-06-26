#pragma once
#include "AST.h"
class UOperator{
    protected:
    Operators opType;
    public:
    virtual QValue* codegen(QValue* operand) = 0; 
    virtual void printAST() = 0;
    UOperator(Operators op){
        opType = op;
    }
    virtual Operators getOpType(){
        return opType;
    }
};

class exclamation : public UOperator{
    public:
  
    exclamation(Operators op = Operators::exclamation_point):UOperator(op){}
    void printAST();
    QValue* codegen(QValue* operand);
};

class negative : public UOperator{
    public:
  
    negative(Operators op = Operators::minus):UOperator(op){}
    void printAST();
    QValue* codegen(QValue* operand);
};

class BOperator{
    protected:
    Operators opType;
    public:
    virtual Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right) = 0;
    virtual QValue* codegen(QValue* a, QValue* b) = 0;
    virtual QValue* constantCodegen(long long left, long long right) = 0;
    virtual void printAST() = 0;
    virtual Operators getOpType(){
        return opType;
    }
   // virtual bool isCompareOp() = 0;
    BOperator(Operators op){
        opType = op;
    }
};

class CompareOperator : public BOperator{
    public:
    virtual bool gen_constant(long long left, long long right) = 0;
    virtual void printAST() =0;
    CompareOperator(Operators op):BOperator(op){}
    virtual Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right) = 0;
   
    virtual QValue* codegen(QValue* a, QValue* b) {
        IntType* getSign = dynamic_cast<IntType*>(a->getType());
        llvm::Value* llvm_result = gen_llvm(getSign->getSigned() , a->getValue() , b->getValue());
        return new QValue(new IntType(false,1) , llvm_result);
    }

    virtual QValue* constantCodegen(long long left, long long right){
        bool result = gen_constant(left,right);
        int i = result ? true : false;
        IntType* qtype = new IntType(false,1);
        llvm::Value* constInt =  ConstantInt::get(qtype->getLLVMType(), i);
        return new QValue(qtype,constInt);
    }
};

class equal_sign : public CompareOperator{
    public:
    bool gen_constant(long long left, long long right){
        return left == right;
    }

    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    void printAST();
    equal_sign():CompareOperator(Operators::equal_sign){}
};  // ==

class not_equal : public CompareOperator{
    public:
    bool gen_constant(long long left, long long right){
        return left != right;
    }

    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    void printAST();
    not_equal():CompareOperator(Operators::not_equal){}
};  // !=

class less_equal : public CompareOperator{
    public:
    bool gen_constant(long long left, long long right){
        return left <= right;
    }

    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    void printAST();
    less_equal():CompareOperator(Operators::less_equal){}
};  // <=

class greater_equal : public CompareOperator{
    public:
    bool gen_constant(long long left, long long right){
        return left >= right;
    }
    
    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    void printAST();
    greater_equal():CompareOperator(Operators::greater_equal){}
};  // >=

class greater_than : public CompareOperator{
    public:
    bool gen_constant(long long left, long long right){
        return left > right;
    }

    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    void printAST();
    greater_than():CompareOperator(Operators::greater_than){}
};  // >

class less_than : public CompareOperator{
    public:
    bool gen_constant(long long left, long long right){
        return left < right;
    }

    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    void printAST();
    less_than():CompareOperator(Operators::less_than){}
};  // <

class ArithOperator : public BOperator {
protected:
    int line;
public:
    virtual bool isCompareOp(){
        return false;
    }
    virtual int getLine(){
        return line;
    }
    ArithOperator(Operators op,int l):BOperator(op){
        line = l;
    }

    virtual Function* overFlowDeclare(std::vector<Type*> args_type, bool isSigned) = 0; 
    virtual long long gen_constant(long long left, long long right) = 0;
    virtual void printAST() = 0;
    virtual Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right) = 0;
    virtual void divisionDyCheck(QValue* left,QValue* right) = 0; //overflow and division 0

    virtual QValue* codegen(QValue* a, QValue* b) {
        if(doCheck)
            OverFlowCheck(a,b);
        IntType* getSign = dynamic_cast<IntType*>(a->getType());
        llvm::Value* llvm_result = gen_llvm(getSign->getSigned() , a->getValue() , b->getValue());
        return new QValue(a->getType(), llvm_result);
    }

    virtual QValue* constantCodegen(long long left, long long right){
        long long result = gen_constant(left,right);
        ConstantType* qtype = new ConstantType(result);
        llvm::Value* constInt =  ConstantInt::get(qtype->getLLVMType(), result);
        return new QValue(qtype,constInt);
    }

    virtual void OverFlowCheck(QValue* left, QValue* right){
        if(opType == Operators::division){
            divisionDyCheck(left,right);
        }else{
            std::vector<Type*> args_type;
            args_type.push_back(left->getType()->getLLVMType()); 
            args_type.push_back(right->getType()->getLLVMType());
            
            Function* overFlow = overFlowDeclare(args_type,dynamic_cast<IntType*>(left->getType())->getSigned());
            if (!overFlow) return;
            
            Function *TheFunction = Builder.GetInsertBlock()->getParent();

            std::vector<llvm::Value*> fun_arguments;
            fun_arguments.push_back(left->getValue()); 
            fun_arguments.push_back(right->getValue());

            Value* checkCall = Builder.CreateCall(overFlow, fun_arguments, "overflowtmp");
            Value* extractV = Builder.CreateExtractValue(checkCall,1);

            BasicBlock *overflowBB = BasicBlock::Create(TheContext, "overflow", TheFunction);
            BasicBlock *normalBB = BasicBlock::Create(TheContext, "normal",TheFunction);
            Builder.CreateCondBr(extractV, overflowBB, normalBB);

            // overflow
            Builder.SetInsertPoint(overflowBB);
            callError("overflow",line);
            overflowBB = Builder.GetInsertBlock();  

            // normal
            Builder.SetInsertPoint(normalBB);
        }
    }
};

class plus : public ArithOperator{
    public:
    long long gen_constant(long long left, long long right){
        return left + right;
    }

    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    void printAST();
    Function* overFlowDeclare(std::vector<Type*> args_type, bool isSigned);
    void divisionDyCheck(QValue* left,QValue* right){Bug("this is plus, only division need the function",line);}
    plus(int line):ArithOperator(Operators::plus,line){}
};  // +

class minus : public ArithOperator{
    public:
    long long gen_constant(long long left, long long right){
        return left - right;
    }

    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    void printAST();
    Function* overFlowDeclare(std::vector<Type*> args_type, bool isSigned);
    void divisionDyCheck(QValue* left,QValue* right){Bug("this is minus, only division need the function",line);}
    minus(int line):ArithOperator(Operators::minus,line){}
};  // -

class star : public ArithOperator{
    public:
    long long gen_constant(long long left, long long right){
        return left * right;
    }

    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    void printAST();
    Function* overFlowDeclare(std::vector<Type*> args_type, bool isSigned);
    void divisionDyCheck(QValue* left,QValue* right){Bug("this is star, only division need the function",line);}
    star(int line):ArithOperator(Operators::star,line){}
};  // *

class division : public ArithOperator{
    public:
    long long gen_constant(long long left, long long right){
        if(right==0){
            error("The divisor cannot be 0");
        }
        return left / right;
    }

    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    void printAST();
    Function* overFlowDeclare(std::vector<Type*> args_type, bool isSigned);
    void divisionDyCheck(QValue* left,QValue* right);
    division(int line):ArithOperator(Operators::division,line){}
};  // /

class andT : public ArithOperator{
    public:
    long long gen_constant(long long left, long long right){
        return left & right;
    }

    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    void printAST();
    Function* overFlowDeclare(std::vector<Type*> args_type, bool isSigned);
    void divisionDyCheck(QValue* left,QValue* right){Bug("this is and, only division need the function",line);}
    andT(int line):ArithOperator(Operators::andT,line){}
};  // &

class orT : public ArithOperator{
    public:
    long long gen_constant(long long left, long long right){
        return left | right;
    }
    
    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    void printAST();
    Function* overFlowDeclare(std::vector<Type*> args_type, bool isSigned);
    void divisionDyCheck(QValue* left,QValue* right){Bug("this is or, only division need the function",line);}
    orT(int line):ArithOperator(Operators::orT,line){}
};  // |