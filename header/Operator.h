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
    virtual QValue* constantCodegen(std::string left, std::string right) = 0;
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
    virtual bool gen_constant(std::string left, std::string right) = 0;
    virtual void printAST() =0;
    virtual Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right) = 0;
    
    CompareOperator(Operators op):BOperator(op){}
   
    virtual QValue* codegen(QValue* a, QValue* b) {
        IntType* getSign = dynamic_cast<IntType*>(a->getType());
        llvm::Value* llvm_result = gen_llvm(getSign->getSigned() , a->getValue() , b->getValue());
        return new QValue(new IntType(false,1) , llvm_result);
    }

    virtual QValue* constantCodegen(std::string left, std::string right){
        bool result = gen_constant(left,right);
        int i = result ? true : false;
        printf("%s  ",left.c_str());
        printAST();
        printf("  %s = %d\n",right.c_str(),i);
        IntType* qtype = new IntType(false,1);
        llvm::Value* constInt =  ConstantInt::get(qtype->getLLVMType(), i);
        return new QValue(qtype,constInt);
    }
};

class equal_sign : public CompareOperator{
    public:
    bool gen_constant(std::string left, std::string right){
        return left == right;
    }

    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    void printAST();
    equal_sign():CompareOperator(Operators::equal_sign){}
};  // ==

class not_equal : public CompareOperator{
    public:
    bool gen_constant(std::string left, std::string right){
        return left != right;
    }

    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    void printAST();
    not_equal():CompareOperator(Operators::not_equal){}
};  // !=

class less_equal : public CompareOperator{
    public:
    bool gen_constant(std::string left, std::string right){
        // <=
        if(left == right)
            return true;
        return bigger(right,left);
    }

    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    void printAST();
    less_equal():CompareOperator(Operators::less_equal){}
};  // <=

class greater_equal : public CompareOperator{
    public:
    bool gen_constant(std::string left, std::string right){
        // >=
        if(left == right)
            return true;
        return bigger(left,right);
    }
    
    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    void printAST();
    greater_equal():CompareOperator(Operators::greater_equal){}
};  // >=

class greater_than : public CompareOperator{
    public:
    bool gen_constant(std::string left, std::string right){
        // >
        if(left == right)
            return false;
        return bigger(left,right);
    }

    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    void printAST();
    greater_than():CompareOperator(Operators::greater_than){}
};  // >

class less_than : public CompareOperator{
    public:
    bool gen_constant(std::string left, std::string right){
        // <
        if(left == right)
            return false;
        return bigger(right,left);
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
    virtual std::string gen_constant(std::string left, std::string right) = 0;
    virtual void printAST() = 0;
    virtual Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right) = 0;

    virtual QValue* codegen(QValue* a, QValue* b) {
        assert(a->getType()->compare(b->getType()));
        if(doCheck)
            OverFlowCheck(a,b);
        IntType* getSign = dynamic_cast<IntType*>(a->getType());
        llvm::Value* llvm_result = gen_llvm(getSign->getSigned() , a->getValue() , b->getValue());
        return new QValue(a->getType(), llvm_result);
    }

    virtual QValue* constantCodegen(std::string left, std::string right){
        std::string result = gen_constant(left,right);

        ConstantType* qtype = new ConstantType(result,line);
        llvm::APInt resAP = llvm::APInt(getBitOfInt(result,true), result, 10);
        llvm::Value* constInt =  ConstantInt::get(TheContext, resAP);
        return new QValue(qtype,constInt);
    }

    virtual void OverFlowCheck(QValue* left, QValue* right){
        
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
};

class plus : public ArithOperator{
    public:
    std::string gen_constant(std::string left, std::string right){
        std::string res = plus1(left,right);
        if(res[0]!='0' && getBitOfInt(res,(res[0]=='-'))<=0){
            error("the calculation result of "+left+" and "+right+" is overflow at line: "+std::to_string(line));
        }
        return res;
    }

    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    void printAST();
    Function* overFlowDeclare(std::vector<Type*> args_type, bool isSigned);
    plus(int line):ArithOperator(Operators::plus,line){}
};  // +

class minus : public ArithOperator{
    public:
    std::string gen_constant(std::string left, std::string right){
        if(right[0]=='-'){
            right[0] = '+';
        }else if(right[0]=='+'){
            right[0] = '-';
        }
        std::string res = plus1(left,right);
        if(res[0]!='0' && getBitOfInt(res,(res[0]=='-'))<=0){
            error("the calculation result of "+left+" and "+right+" is overflow at line: "+std::to_string(line));
        }
        return res;
    }

    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    void printAST();
    Function* overFlowDeclare(std::vector<Type*> args_type, bool isSigned);
    minus(int line):ArithOperator(Operators::minus,line){}
};  // -

class star : public ArithOperator{
    public:
    std::string gen_constant(std::string l, std::string r){
        
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
    }

    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    void printAST();
    Function* overFlowDeclare(std::vector<Type*> args_type, bool isSigned);
    void divisionDyCheck(QValue* left,QValue* right){Bug("this is star, only division need the function",line);}
    star(int line):ArithOperator(Operators::star,line){}
};  // *

class division : public ArithOperator{
    public:
    std::string gen_constant(std::string l, std::string r){
        if(r=="0"){
            error("The divisor cannot be 0");
        }if(l=="0"){
            return "0";
        }
        bool isLP = (l[0]=='+');
        bool isRP = (r[0]=='+');

        assert(l[0]=='+'||l[0]=='-');
        l.erase(0,1);
        assert(r[0]=='+'||r[0]=='-');
        r.erase(0,1);
        bool resP = (isLP == isRP);
        if(l==r){
            if(resP)
                return "+1";
            else
                return "-1";
        }

        l = "+" + l;
        r = "+" + r;

        if(!bigger(l,r)){
            return "0";
        }
        std::string result="0";

        while( l==r || bigger(l,r) ){
            result = plus1(result, "+1");

            std::string tempR = r;
            tempR[0] = '-';
            l = plus1(l,tempR);
        //    printf("%s   %s   %s   %s\n",result.c_str(),l.c_str(),tempR.c_str(),r.c_str());
        }
        if(!resP)
            result[0]='-';

        if(getBitOfInt(result,!resP)<=0){
            error("the calculation result of is overflow at line: "+std::to_string(line));
        }

        return result;
    }

    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    void printAST();
    Function* overFlowDeclare(std::vector<Type*> args_type, bool isSigned);
    void OverFlowCheck(QValue* left,QValue* right);
    division(int line):ArithOperator(Operators::division,line){}
};  // /

class andT : public ArithOperator{
    public:
    std::string gen_constant(std::string left, std::string right){
        return "";
    }

    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    void printAST();
    Function* overFlowDeclare(std::vector<Type*> args_type, bool isSigned);
    void divisionDyCheck(QValue* left,QValue* right){Bug("this is and, only division need the function",line);}
    andT(int line):ArithOperator(Operators::andT,line){}
};  // &

class orT : public ArithOperator{
    public:
    std::string gen_constant(std::string left, std::string right){
        return "";
    }
    
    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    void printAST();
    Function* overFlowDeclare(std::vector<Type*> args_type, bool isSigned);
    void divisionDyCheck(QValue* left,QValue* right){Bug("this is or, only division need the function",line);}
    orT(int line):ArithOperator(Operators::orT,line){}
};  // |
