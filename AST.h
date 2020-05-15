#pragma once
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"

using namespace llvm;
//using namespace llvm::orc;

enum ASTType{
	variable = -1,
	arrayIndex = -2,
	number = -3,
	call = -4,

	unary = -5,
	binary = -6,
	
	ifT = -7,
	forT = -8,
    whileT = -9,
    breakT = -10,
	varDef = -11,
	arrayDef = -12,
    assign = -13,
	function = -14,
	proto = -15,
	body = -16,
	returnT = -17,
    nullT = -18
};
//Base class
class AST{
protected:
    int astType;
public:
    AST(int type){
        astType = type;
    }
    int getType();
};

// Expression
class ExprAST : public AST{
protected:
    bool paren;
public:
    ExprAST(int type):AST(type){
        paren = false;
    }
    void hasParen(){
        paren = true;
    }
    bool wrapInParen(){
        return paren;
    }
    Value* codegen();
};

// Commands
class CommandAST : public AST{
public:
    CommandAST(int type):AST(type){}
    Value* codegen();
};

// LeftValue  is also expression
class LeftValueAST : public ExprAST{
public:
    LeftValueAST(int type):ExprAST(type){}
};

// Variable
class VariableExprAST : public LeftValueAST{
    std::string name;
public:
    VariableExprAST(const std::string &Name) : LeftValueAST(ASTType::variable) {
		name = Name;
	}
	const std::string &getName() const { return name; }
};

// arrayIndex
class ArrayIndexExprAST: public LeftValueAST{
    std::string arrayName;
    std::vector<std::unique_ptr<ExprAST>> indexs;
public:
    ArrayIndexExprAST(const std::string &arrayName1, std::vector<std::unique_ptr<ExprAST>> indexs1)
                     : LeftValueAST(ASTType::arrayIndex)
    {
        arrayName = arrayName1;
        indexs = std::move(indexs1);
    }
};

// Constant Number
class NumberExprAST : public ExprAST{
protected:
    long long value;
public:
    NumberExprAST(long long val):ExprAST(ASTType::number){
        value = val;
    }
};

// NullPointer
class NullExprAST : public ExprAST{
public:
    NullExprAST():ExprAST(ASTType::nullT){}
};

/// CallExprAST - Expression class for function calls.
class CallExprAST : public ExprAST, public CommandAST
{
	std::string functionName;
	std::vector<std::unique_ptr<ExprAST>> args;

public:
	CallExprAST(const std::string &functionName1, std::vector<std::unique_ptr<ExprAST>> args1)
		       : ExprAST(ASTType::call), CommandAST(ASTType::call)
    {
        functionName = functionName1;
        args = std::move(args1); 
    }
};

/// UnaryExprAST - Expression class for a unary operator.
class UnaryExprAST : public ExprAST
{
	int opCode;					  
	std::unique_ptr<ExprAST> Operand; 

public:
	UnaryExprAST(int opCode1, std::unique_ptr<ExprAST> Operand1) : ExprAST(ASTType::unary) {
        opCode = opCode1;
        Operand = move(Operand1);
    }
};

/// BinaryExprAST - Expression class for a binary operator.
class BinaryExprAST : public ExprAST
{
	int op;
	std::unique_ptr<ExprAST> LHS, RHS;

public:
	BinaryExprAST(int OpCode, std::unique_ptr<ExprAST> lhs, std::unique_ptr<ExprAST> rhs) : ExprAST(ASTType::binary){
        op = OpCode;
        LHS = std::move(lhs);
        RHS = std::move(rhs); 
    }
};

class DefAST:public CommandAST{
    public:
    DefAST(int type) : CommandAST(type){}
};

/// VarDefAST
class VarDefAST : public DefAST{
    int VarType;
    std::string name;
    std::unique_ptr<ExprAST> value;
public:
    VarDefAST(int type, std::string n, std::unique_ptr<ExprAST> v):DefAST(ASTType::varDef){
        VarType = type;
        name = n;
        value = std::move(v);
    }
};

///ArrayDefAST
class ArrayDefAST : public DefAST{
    int ArrayType;
    std::string name;
    std::vector<std::unique_ptr<ExprAST>> sizes;
public:
    ArrayDefAST(int type, std::string n, std::vector<std::unique_ptr<ExprAST>> s)
               :DefAST(ASTType::arrayDef)
    {
        ArrayType = type;
        name = n;
        sizes = std::move(s);
    }
};

///AssignAST
class AssignAST : public CommandAST{
    std::unique_ptr<LeftValueAST> left;
    std::unique_ptr<ExprAST> right;
    public:
    AssignAST(std::unique_ptr<LeftValueAST> l, std::unique_ptr<ExprAST> r):CommandAST(ASTType::assign){
        left = std::move(l);
        right = std::move(r);
    }
};

///ifAST
class IfAST : public CommandAST{
    std::unique_ptr<ExprAST> condition;
    std::unique_ptr<CommandAST> thenC;
    std::unique_ptr<CommandAST> elseC;
    public:
    IfAST( std::unique_ptr<ExprAST> cond, std::unique_ptr<CommandAST> t, std::unique_ptr<CommandAST> e)
              : CommandAST(ASTType::ifT)
    {
        condition = std::move(cond);
        thenC = std::move(t);
        elseC = std::move(e);
    }
};

///forAST
class ForAST : public CommandAST{
    std::unique_ptr<CommandAST> start;
    std::unique_ptr<ExprAST> condition;
    long long step;
    std::unique_ptr<CommandAST> body;
    public:
    ForAST(std::unique_ptr<CommandAST> start1,std::unique_ptr<ExprAST> condition1,long long step1,std::unique_ptr<CommandAST> body1)
          : CommandAST(ASTType::forT)
    {
        start = std::move(start1);
        condition = std::move(condition1);
        step = step1;
        body = std::move(body1);
    }

};

///whileAST
class WhileAST : public CommandAST{
    std::unique_ptr<ExprAST> condition;
    std::unique_ptr<CommandAST> body;
    public:
    WhileAST(std::unique_ptr<ExprAST> cond, std::unique_ptr<CommandAST> cmd)
            : CommandAST(ASTType::whileT)
    {
        condition = std::move(cond);
        body = std::move(cmd);
    }
};

///returnAST
class ReturnAST : public CommandAST{
    std::unique_ptr<ExprAST> value;
    public:
    ReturnAST(std::unique_ptr<ExprAST> v)
             : CommandAST(ASTType::returnT)
    {
        value = std::move(v);
    }
};

///breakAST
class BreakAST : public CommandAST{
  
    public:
    BreakAST():CommandAST(ASTType::breakT){}
};

///blockAST
class BlockAST : public CommandAST{
    std::vector<std::unique_ptr<CommandAST>> cmds;
public:
    BlockAST(std::vector<std::unique_ptr<CommandAST>> c):CommandAST(ASTType::breakT){
        cmds = std::move(c);
    }
};