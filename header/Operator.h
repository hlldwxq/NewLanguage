#pragma once
#include "AST.h"

bool bigger(std::string left,std::string right);
std::string getZero(int size);
std::string removeExtra0(std::string s);
std::string plus1(std::string l,std::string r);

class UOperator{
    protected:
    Operators opType;
    int line;
    public:
    virtual QValue* codegen(QValue* operand) = 0; 
    virtual void printAST() = 0;
    UOperator(Operators op,int l){
        opType = op;
        line = l;
    }
    virtual Operators getOpType(){
        return opType;
    }
};

class exclamation : public UOperator{
    public:
  
    exclamation(int l,Operators op = Operators::exclamation_point):UOperator(op,l){}
    void printAST();
    QValue* codegen(QValue* operand);
};

class negative : public UOperator{
    public:
  
    negative(int l,Operators op = Operators::minus):UOperator(op,l){}
    void printAST();
    QValue* codegen(QValue* operand);
};

class BOperator{
    protected:
    Operators opType;
    public:
    virtual Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right) = 0;
    virtual QValue* codegen(QValue* a, QValue* b) = 0;
    virtual QValue* constantCodegen(IntConst left, IntConst right) = 0;
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
    virtual bool gen_constant(IntConst left, IntConst right) = 0;
    virtual void printAST() =0;
    virtual Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right) = 0;
    
    CompareOperator(Operators op):BOperator(op){}
   
    virtual QValue* codegen(QValue* a, QValue* b) {
        IntType* getSign = dynamic_cast<IntType*>(a->getType());
        llvm::Value* llvm_result = gen_llvm(getSign->getSigned() , a->getValue() , b->getValue());
        return new QValue(new IntType(false,1) , llvm_result);
    }

    virtual QValue* constantCodegen(IntConst left, IntConst right){
        bool result = gen_constant(left,right);
        int i = result ? true : false;
        IntType* qtype = new IntType(false,1);
        llvm::Value* constInt =  ConstantInt::get(qtype->getLLVMType(), i);
        return new QValue(qtype,constInt);
    }
};

class equal_sign : public CompareOperator{
    public:
    bool gen_constant(IntConst left, IntConst right){
        return left.eq(right);
    }
    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    equal_sign():CompareOperator(Operators::equal_sign){}
    void printAST();
};  // ==

class not_equal : public CompareOperator{
    public:
    bool gen_constant(IntConst left, IntConst right){
        return left.ne(right);
    }

    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    not_equal():CompareOperator(Operators::not_equal){} 
    void printAST();
};  // !=

class less_equal : public CompareOperator{
    public:
    bool gen_constant(IntConst left, IntConst right){
        return left.le(right);
    }

    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    less_equal():CompareOperator(Operators::less_equal){}
    void printAST();
};  // <=

class greater_equal : public CompareOperator{
    public:
    bool gen_constant(IntConst left, IntConst right){
        return left.ge(right);
    }
    
    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    void printAST();
    greater_equal():CompareOperator(Operators::greater_equal){}
};  // >=

class greater_than : public CompareOperator{
    public:
    bool gen_constant(IntConst left, IntConst right){
        return left.gt(right);
    }

    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    void printAST();
    greater_than():CompareOperator(Operators::greater_than){}
};  // >

class less_than : public CompareOperator{
    public:
    bool gen_constant(IntConst left, IntConst right){
        return left.lt(right);
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
    virtual IntConst gen_constant(IntConst left, IntConst right) = 0;
    virtual void printAST() = 0;
    virtual Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right) = 0;

    virtual QValue* codegen(QValue* a, QValue* b) {
        assert(a->getType()->compare(b->getType()));

        if(opType!=Operators::andT && opType!=Operators::orT && a->getType()->compare(new IntType(false,1)) && b->getType()->compare(new IntType(false,1))){
            error("boolean (uint1) cannot be as the operand of arithmatic operand at Line: "+std::to_string(line));
        }

        if(doCheck[CheckLevel::check_arith]){
            llvm::Value* result = OverFlowCheck(a,b);
            return new QValue(a->getType(), result);
        }else{
            IntType* getSign = dynamic_cast<IntType*>(a->getType());
            llvm::Value* llvm_result = gen_llvm(getSign->getSigned() , a->getValue() , b->getValue());
            return new QValue(a->getType(), llvm_result);
        }
    }

    virtual QValue* constantCodegen(IntConst left, IntConst right){
        IntConst result = gen_constant(left,right);

        ConstantType* qtype = new ConstantType(result,line);
        llvm::Value* constInt =  ConstantInt::get(TheContext, result.getValue());
        return new QValue(qtype,constInt);
    }

    virtual llvm::Value* OverFlowCheck(QValue* left, QValue* right, std::string bbName = "", std::string normalBBname = ""){
        
        std::vector<Type*> args_type;
        args_type.push_back(left->getType()->getLLVMType()); 
        args_type.push_back(right->getType()->getLLVMType());
        
        Function* overFlow = overFlowDeclare(args_type,dynamic_cast<IntType*>(left->getType())->getSigned());
        if (!overFlow) return gen_llvm(dynamic_cast<IntType*>(left->getType())->getSigned(),left->getValue(),right->getValue());
        
        // Function *TheFunction = Builder.GetInsertBlock()->getParent();

        std::vector<llvm::Value*> fun_arguments;
        fun_arguments.push_back(left->getValue()); 
        fun_arguments.push_back(right->getValue());

        Value* checkCall = Builder.CreateCall(overFlow, fun_arguments, "overflowtmp");
        Value* extractV = Builder.CreateExtractValue(checkCall,1);
        if(bbName == "")
            createBr("overflow when doing arithmatic calculation",extractV, line, "arithOverFlow", "arithNormal");
        else
        {
            createBr("overflow when doing arithmatic calculation",extractV, line, bbName, normalBBname);
        }
        
        return Builder.CreateExtractValue(checkCall,0);
        
    }
};

class plus : public ArithOperator{
    public:
    IntConst gen_constant(IntConst left, IntConst right){ return left.plus(right); }

    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    void printAST();
    Function* overFlowDeclare(std::vector<Type*> args_type, bool isSigned);
    plus(int line):ArithOperator(Operators::plus,line){}
};  // +

class minus : public ArithOperator{
    public:

    IntConst gen_constant(IntConst left, IntConst right){ return left.minus(right); }

    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    void printAST();
    Function* overFlowDeclare(std::vector<Type*> args_type, bool isSigned);
    minus(int line):ArithOperator(Operators::minus,line){}
};  // -

class star : public ArithOperator{
    public:

    IntConst gen_constant(IntConst left, IntConst right){ return left.mul(right); }

    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    void printAST();
    Function* overFlowDeclare(std::vector<Type*> args_type, bool isSigned);
    void divisionDyCheck(QValue* left,QValue* right){Bug("this is star, only division need the function",line);}
    star(int line):ArithOperator(Operators::star,line){}
};  // *

class division : public ArithOperator{
    public:

    IntConst gen_constant(IntConst left, IntConst right){ return left.div(right); }

    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    void printAST();
    Function* overFlowDeclare(std::vector<Type*> args_type, bool isSigned);
    llvm::Value* OverFlowCheck(QValue* left,QValue* right,std::string bbName, std::string normalBBName);
    division(int line):ArithOperator(Operators::division,line){}
};  // /

class andT : public ArithOperator{
    public:
    IntConst gen_constant(IntConst left,IntConst right) { return left.bit_and(right); }

    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    void printAST();
    Function* overFlowDeclare(std::vector<Type*> args_type, bool isSigned);
    andT(int line):ArithOperator(Operators::andT,line){}
};  // &

class orT : public ArithOperator{
    public:
    IntConst gen_constant(IntConst left,IntConst right) { return left.bit_or(right); }
    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    void printAST();
    Function* overFlowDeclare(std::vector<Type*> args_type, bool isSigned);
    orT(int line):ArithOperator(Operators::orT,line){}
};  // |
