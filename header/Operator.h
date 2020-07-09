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
    virtual QValue* constantCodegen(llvm::APInt left, llvm::APInt right) = 0;
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
    virtual bool gen_constant(llvm::APInt left, llvm::APInt right) = 0;
    virtual void printAST() =0;
    virtual Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right) = 0;
    
    CompareOperator(Operators op):BOperator(op){}
   
    virtual QValue* codegen(QValue* a, QValue* b) {
        IntType* getSign = dynamic_cast<IntType*>(a->getType());
        llvm::Value* llvm_result = gen_llvm(getSign->getSigned() , a->getValue() , b->getValue());
        return new QValue(new IntType(false,1) , llvm_result);
    }

    virtual QValue* constantCodegen(llvm::APInt left, llvm::APInt right){
        if(left.getBitWidth()>right.getBitWidth()){
            right = right.sext(left.getBitWidth());
        }else if(left.getBitWidth()<right.getBitWidth()){
            left = left.sext(right.getBitWidth());
        }
        bool result = gen_constant(left,right);
        int i = result ? true : false;
        IntType* qtype = new IntType(false,1);
        llvm::Value* constInt =  ConstantInt::get(qtype->getLLVMType(), i);
        return new QValue(qtype,constInt);
    }
};

class equal_sign : public CompareOperator{
    public:
    bool gen_constant(llvm::APInt left,llvm::APInt right){
        return left.eq(right);
    }
    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    equal_sign():CompareOperator(Operators::equal_sign){}
    void printAST();
};  // ==

class not_equal : public CompareOperator{
    public:
    bool gen_constant(llvm::APInt left,llvm::APInt right){
        return left.ne(right);
    }

    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    not_equal():CompareOperator(Operators::not_equal){} 
    void printAST();
};  // !=

class less_equal : public CompareOperator{
    public:
    bool gen_constant(llvm::APInt left,llvm::APInt right){
        return left.sle(right);
    }

    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    less_equal():CompareOperator(Operators::less_equal){}
    void printAST();
};  // <=

class greater_equal : public CompareOperator{
    public:
    bool gen_constant(llvm::APInt left,llvm::APInt right){
        return left.sge(right);
    }
    
    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    void printAST();
    greater_equal():CompareOperator(Operators::greater_equal){}
};  // >=

class greater_than : public CompareOperator{
    public:
    bool gen_constant(llvm::APInt left,llvm::APInt right){
        return left.sgt(right);
    }

    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    void printAST();
    greater_than():CompareOperator(Operators::greater_than){}
};  // >

class less_than : public CompareOperator{
    public:
    bool gen_constant(llvm::APInt left,llvm::APInt right){
        return left.slt(right);
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
    virtual llvm::APInt gen_constant(llvm::APInt left, llvm::APInt right) = 0;
    virtual void printAST() = 0;
    virtual Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right) = 0;

    virtual QValue* codegen(QValue* a, QValue* b) {
        assert(a->getType()->compare(b->getType()));

        if(opType!=Operators::andT && opType!=Operators::orT && a->getType()->compare(new IntType(false,1)) && b->getType()->compare(new IntType(false,1))){
            error("boolean (uint1) cannot be as the operand of arithmatic operand at Line: "+std::to_string(line));
        }

        if(doCheck){
            llvm::Value* result = OverFlowCheck(a,b);
            return new QValue(a->getType(), result);
        }else{
            IntType* getSign = dynamic_cast<IntType*>(a->getType());
            llvm::Value* llvm_result = gen_llvm(getSign->getSigned() , a->getValue() , b->getValue());
            return new QValue(a->getType(), llvm_result);
        }
    }

    virtual QValue* constantCodegen(llvm::APInt left, llvm::APInt right){

        int leftBit = left.getBitWidth();
        int rightBit = right.getBitWidth();

        if(leftBit>rightBit){
            right = right.sext(leftBit);

        }else if(leftBit<rightBit){
            left = left.sext(rightBit);
        }

        llvm::APInt result = gen_constant(left,right);
        if(result.getBitWidth()>128){
            if((result.isNegative() && checkRange(result,true)) || (!result.isNegative()&&checkRange(result,false))){
                error("invalid number because it is too big or too small at line: "+std::to_string(line));
            }
        }

        ConstantType* qtype = new ConstantType(result);
        llvm::Value* constInt =  ConstantInt::get(TheContext, result);
        return new QValue(qtype,constInt);
    }

    virtual llvm::Value* OverFlowCheck(QValue* left, QValue* right){
        
        std::vector<Type*> args_type;
        args_type.push_back(left->getType()->getLLVMType()); 
        args_type.push_back(right->getType()->getLLVMType());
        
        Function* overFlow = overFlowDeclare(args_type,dynamic_cast<IntType*>(left->getType())->getSigned());
        if (!overFlow) return gen_llvm(dynamic_cast<IntType*>(left->getType())->getSigned(),left->getValue(),right->getValue());
        
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
        return Builder.CreateExtractValue(checkCall,0);
        
    }
};

class plus : public ArithOperator{
    public:
    llvm::APInt gen_constant(llvm::APInt left,llvm::APInt right){
        assert(left.getBitWidth()==right.getBitWidth());

        bool overflow; //check
        int currentWidth = left.getBitWidth();
        llvm::APInt result = left.sadd_ov(right,overflow);
        while(overflow){
            currentWidth*=2;
            left = left.sext(currentWidth);
            right = right.sext(currentWidth);
            assert(left.getBitWidth()==right.getBitWidth());
            result = left.sadd_ov(right,overflow);
        }
        return result;
    }

    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    void printAST();
    Function* overFlowDeclare(std::vector<Type*> args_type, bool isSigned);
    plus(int line):ArithOperator(Operators::plus,line){}
};  // +

class minus : public ArithOperator{
    public:
    llvm::APInt gen_constant(llvm::APInt left,llvm::APInt right){
        
        assert(left.getBitWidth()==right.getBitWidth());
        bool overflow; //check
        int currentWidth = left.getBitWidth();
        llvm::APInt result = left.ssub_ov(right,overflow);
        while(overflow){
            currentWidth*=2;
            left = left.sext(currentWidth);
            right = right.sext(currentWidth);
            assert(left.getBitWidth()==right.getBitWidth());
            result = left.ssub_ov(right,overflow);

        }

        return result;
    }

    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    void printAST();
    Function* overFlowDeclare(std::vector<Type*> args_type, bool isSigned);
    minus(int line):ArithOperator(Operators::minus,line){}
};  // -

class star : public ArithOperator{
    public:
    /*std::string gen_constant(std::string l, std::string r){
        
        if(l=="0" || r=="0"){
            return "0";
        }

        bool isLP = (l[0]=='+');
        bool isRP = (r[0]=='+');
        l.erase(0,1);
        r.erase(0,1);
    
        bool rp = (isLP == isRP);  //the result is pos or not
        std::string result = "0";  //care the symbol at the end, assume the result >0 when calculating
        int carry = 0;
        for(int i=r.size()-1;i>=0;i--){
            
            int left = r[i]-'0';
            std::string res1 = "";
            for(int j=l.size()-1;j>=0;j--){
                int right = l[j]-'0';
                int res = left * right + carry;
                carry = res/10;
                res = res - carry*10;
                res1 = std::to_string(res) + res1;
            }
            res1 = res1 + getZero(r.size()-1-i);
            res1 = removeExtra0(res1);
            res1 = "+"+res1;
            result = plus1(result,res1);
        }
        
        assert(result[0]=='+');
        result.erase(0,1);
        if(carry!=0){
            result = std::to_string(carry)+result;
        }
        if(rp){
            result = '+'+result;
        }else{
            result = '-'+result;
        }
        if(result[0]!='0' && getBitOfInt(result,!rp)<=0){
            error("the calculation result of " + l + " and " + r + " is overflow at line: "+std::to_string(line));
        }
        return result;
    }*/
    llvm::APInt gen_constant(llvm::APInt left,llvm::APInt right){
        assert(left.getBitWidth()==right.getBitWidth());
        bool overflow; //check
        int currentWidth = left.getBitWidth();
        llvm::APInt result = left.smul_ov(right,overflow);
        while(overflow){
            currentWidth*=2;
            left = left.sext(currentWidth);
            right = right.sext(currentWidth);
            assert(left.getBitWidth()==right.getBitWidth());
            result = left.smul_ov(right,overflow);
        }
        return result;
    }
    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    void printAST();
    Function* overFlowDeclare(std::vector<Type*> args_type, bool isSigned);
    void divisionDyCheck(QValue* left,QValue* right){Bug("this is star, only division need the function",line);}
    star(int line):ArithOperator(Operators::star,line){}
};  // *

class division : public ArithOperator{
    public:
    llvm::APInt gen_constant(llvm::APInt left,llvm::APInt right){
        if(right==(0)){
            error("div 0 at line: "+std::to_string(line));
        }
        assert(left.getBitWidth()==right.getBitWidth());
        bool overflow; //check
        int currentWidth = left.getBitWidth();
        llvm::APInt result = left.sdiv_ov(right,overflow);
        while(overflow){
            currentWidth*=2;
            left = left.sext(currentWidth);
            right = right.sext(currentWidth);
            assert(left.getBitWidth()==right.getBitWidth());
            result = left.sdiv_ov(right,overflow);
        }

        return result;
    }

    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    void printAST();
    Function* overFlowDeclare(std::vector<Type*> args_type, bool isSigned);
    llvm::Value* OverFlowCheck(QValue* left,QValue* right);
    division(int line):ArithOperator(Operators::division,line){}
};  // /

class andT : public ArithOperator{
    public:
    llvm::APInt gen_constant(llvm::APInt left,llvm::APInt right){
        llvm::APInt copyL(left);
        llvm::APInt result = left&=(right);
        //Performs a bitwise OR operation on this APInt and RHS. The result is assigned to *this
        left = copyL;
        return result;
    }

    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    void printAST();
    Function* overFlowDeclare(std::vector<Type*> args_type, bool isSigned);
    andT(int line):ArithOperator(Operators::andT,line){}
};  // &

class orT : public ArithOperator{
    public:
    llvm::APInt gen_constant(llvm::APInt left,llvm::APInt right){
        llvm::APInt copyL(left);
        llvm::APInt result = left|=(right);
        //Performs a bitwise OR operation on this APInt and RHS. The result is assigned to *this
        left = copyL;
        return result;
    }
    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    void printAST();
    Function* overFlowDeclare(std::vector<Type*> args_type, bool isSigned);
    orT(int line):ArithOperator(Operators::orT,line){}
};  // |
