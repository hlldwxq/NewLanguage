#pragma once
#include "AST.h"
#include "Operator.h"

// LeftValue  is also expression
class LeftValueAST : public ExprAST{
public:
    LeftValueAST(ASTType type, int l):ExprAST(type,l){}
    virtual ~LeftValueAST(){}
    virtual void printAST(int level=0){}
    virtual QValue* codegen()=0; //{ return NULL; }
    virtual const QAlloca* codegenLeft()=0;
};

// Variable
class VariableAST : public LeftValueAST{
    std::string name;
    bool isPointer;
public:
    VariableAST(std::string Name, int line1, bool isP = false) 
                   :LeftValueAST(ASTType::var_or_pointer,line1) {
		name = Name;
        isPointer = isP;
	}
	std::string getName() { return name; }
    const QAlloca* codegenLeft();
    QValue* codegen();

    void printAST(int level=0);
};

// arrayIndex
class ArrayIndexExprAST: public LeftValueAST{
    std::unique_ptr<ExprAST> pointer; 
    std::unique_ptr<ExprAST> index;

public:
    ArrayIndexExprAST(std::unique_ptr<ExprAST> p, std::unique_ptr<ExprAST> index1,int line1)
                     : LeftValueAST(ASTType::arrayIndex,line1) {
        pointer = std::move(p);
        index = std::move(index1);
    }
    const QAlloca* codegenLeft();
    QValue* codegen();

    void printAST(int level=0);
};

// Constant Number
class NumberExprAST : public ExprAST{
    std::string value;

public:
    NumberExprAST(std::string val, int line1):ExprAST(ASTType::number,line1){
        value = val;
    }
    std::string getValue(){
        return value;
    }

    void printAST(int level=0);
    QValue* codegen();
};

// constant string
class StringExprAST : public ExprAST{
    std::string str;
public:
    StringExprAST(std::string str1, int line1):ExprAST(ASTType::stringT,line1){
        str = str1;
    }
    std::string getString(){
        return str;
    }

    void printAST(int level=0);
    QValue* codegen();

};

//true or false
class ConstantBoolAST : public ExprAST{
    bool b;
public:
    ConstantBoolAST(bool b1,int line1):ExprAST(ASTType::boolT,line1){
        b = b1;
    }
    void printAST(int level=0);
    QValue* codegen();
};

// NullPointer
class NullExprAST : public ExprAST{
public:
    NullExprAST(int line1):ExprAST(ASTType::nullT,line1){
    }

    void printAST(int level=0);
    QValue* codegen();
};

/// CallExprAST - Expression class for function calls.
class CallExprAST : public ExprAST, public CommandAST
{
	std::string functionName;
	std::vector<std::unique_ptr<ExprAST>> args;
public:
	CallExprAST(std::string functionName1, std::vector<std::unique_ptr<ExprAST>> args1,int line1)
		       : ExprAST(ASTType::call,line1), CommandAST(ASTType::call,line1,false), args(std::move(args1))
    {
        functionName = functionName1;
    }

    void printAST(int level=0);
    void codegenCommand();
    QValue* codegen();

private:
    QValue* codegen_internal(bool is_cmd);

};

/// UnaryExprAST - Expression class for a unary operator.
class UnaryExprAST : public ExprAST
{
	UOperator* opCode;					  
	std::unique_ptr<ExprAST> Operand; 
public:
	UnaryExprAST(UOperator* opCode1, std::unique_ptr<ExprAST> Operand1, int line1) : ExprAST(ASTType::unary,line1) {
        opCode = opCode1;
        Operand = move(Operand1);
    }

    QValue* codegen();
    Operators getOperatorType(){
        return opCode->getOpType();
    }
    void printAST(int level=0);
};

/// BinaryExprAST - Expression class for a binary operator.
class BinaryExprAST : public ExprAST
{
	BOperator* op;
	std::unique_ptr<ExprAST> LHS, RHS;

public:
	BinaryExprAST(BOperator* OpCode, std::unique_ptr<ExprAST> lhs, std::unique_ptr<ExprAST> rhs,int line1) 
                 : ExprAST(ASTType::binary,line1){
        op = OpCode;
        LHS = std::move(lhs);
        RHS = std::move(rhs); 
    }

    void printAST(int level=0);

    Operators getOperatorType(){
        return op->getOpType();
    }
   /* bool isCompareOp(){
        return op->isCompareOp();
    }*/
    QValue* codegen();

};

/// newExprAST
class NewExprAST : public ExprAST{
    PointType* type; 
    std::unique_ptr<ExprAST> size;
public:
    NewExprAST(PointType* type1, std::unique_ptr<ExprAST> size1, int line1) 
              :ExprAST(ASTType::newT,line1)
    {
        type = type1;
        size = std::move(size1);
    }
    QType* getType(){
        return type;
    }
    ExprAST* getSizePtr(){
        return size.get();
    }
    void printAST(int level=0);
    QValue* codegen();
};