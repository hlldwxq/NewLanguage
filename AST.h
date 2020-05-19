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

enum class ASTType{
	variable,
	arrayIndex,
	number,
	call,

	unary,
	binary,
	
	ifT,
	forT,
    whileT,
    breakT,
    newT,
	varDef,
	arrayDef,
    assign,
	function,
	proto,
	body,
	returnT,
    nullT
};

enum class Operators{
    //symbol
	equal_sign,  //==
	not_equal,   //!=
	less_equal,  //<=
	more_equal,  //>=
	plus_sign,   //+
	minus,       //-
	star,        //*
	disvision,   // /
	left_paren,  // (
	right_paren, // )
	left_square_bracket,  // [
	right_square_bracket, // ]
	left_brace,           // {
	right_brace,          // }
	comma,                // ,
	exclamation_point,    // !
	andT,                  // &
	orT,                   // |
	more_sign,            // >
	less_sign,            // <
	assignment,           // =
};

enum class VarType{
    //type
	int1,
	int8,
	int16,
	int32,
	int64,
	int128
};

enum class TypeType{
    intType,
    pointerType,
    voidType,
    errorType
};
class ReturnType{
    protected:
    TypeType typeOfType;
    public:
    ReturnType(TypeType t){
        typeOfType = t;
    }
    TypeType getType(){
        return typeOfType;
    }
};

class VarAndPointType : public ReturnType{
    public:
        VarAndPointType(TypeType t):ReturnType(t){}
};

class voidType : public ReturnType{
    public:
        voidType():ReturnType(TypeType::voidType){}
};

class IntType : public VarAndPointType{
    VarType type;
public:
    IntType(VarType type1) : VarAndPointType(TypeType::intType){
        type = type1;
    }
};

class PointType : public VarAndPointType{
    VarAndPointType elementType;
public:
    PointType(const VarAndPointType& elementType1) : VarAndPointType(TypeType::pointerType),elementType(elementType1){
    }
};

//Base class
class AST{
protected:
    ASTType astType;
public:
    AST(ASTType type){
        astType = type;
    }
    int getType();
};

// Expression
class ExprAST : public AST{
public:
    ExprAST(ASTType type):AST(type){
    }
    
    Value* codegen();
};

// Commands
class CommandAST : public AST{
public:
    CommandAST(ASTType type):AST(type){}
    Value* codegen();
};

// structure
class StructureAST : public AST{
public:
    StructureAST(ASTType type):AST(type){}

    GlobalObject* structureCodegen();
};

// LeftValue  is also expression
class LeftValueAST : public ExprAST{
public:
    LeftValueAST(ASTType type):ExprAST(type){}
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
    std::unique_ptr<ExprAST> pointer; 
    std::unique_ptr<ExprAST> index;
public:
    ArrayIndexExprAST(std::unique_ptr<ExprAST> p, std::unique_ptr<ExprAST> index1)
                     : LeftValueAST(ASTType::arrayIndex)
    {
        pointer = std::move(p);
        index = std::move(index1);
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
	Operators opCode;					  
	std::unique_ptr<ExprAST> Operand; 

public:
	UnaryExprAST(Operators opCode1, std::unique_ptr<ExprAST> Operand1) : ExprAST(ASTType::unary) {
        opCode = opCode1;
        Operand = move(Operand1);
    }
};

/// BinaryExprAST - Expression class for a binary operator.
class BinaryExprAST : public ExprAST
{
	Operators op;
	std::unique_ptr<ExprAST> LHS, RHS;

public:
	BinaryExprAST(Operators OpCode, std::unique_ptr<ExprAST> lhs, std::unique_ptr<ExprAST> rhs) : ExprAST(ASTType::binary){
        op = OpCode;
        LHS = std::move(lhs);
        RHS = std::move(rhs); 
    }
};

/// newExprAST
class NewExprAST : public ExprAST{
    VarType type; 
    std::vector<std::unique_ptr<ExprAST>> args;
public:
    NewExprAST(VarType type1, std::vector<std::unique_ptr<ExprAST>> args1) 
              :ExprAST(ASTType::newT)
    {
        type = type1;
        args = std::move(args1);
    }
};

class DefAST:public CommandAST, public StructureAST{
    bool global;
    public:
    DefAST(ASTType type,bool g) : CommandAST(type),StructureAST(type){
        global = g;
    }
};

/// VarDefAST
class VarDefAST : public DefAST{
    IntType type;
    std::string name;
    std::unique_ptr<ExprAST> value;
public:
    VarDefAST(const IntType& type1, std::string n, std::unique_ptr<ExprAST> v, bool global)
             :type(type1),DefAST(ASTType::varDef,global){
        name = n;
        value = std::move(v);
    }
};

///ArrayDefAST
class ArrayDefAST : public DefAST{
    PointType type;
    std::string name;
    std::unique_ptr<ExprAST> value;
public:
    ArrayDefAST(PointType &type1, std::string n, std::unique_ptr<ExprAST> v,bool global)
               :type(type1),DefAST(ASTType::arrayDef,global)
    {
        type = type1;
        name = n;
        value = std::move(v);
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
    BlockAST():CommandAST(ASTType::breakT){
        //std::vector<std::unique_ptr<CommandAST>> c;
        //cmds = c;
    }
};

class PrototypeAST : public StructureAST
{
	std::string Name;
	std::map<std::string,VarAndPointType*> Args;
    ReturnType returnType;

public:
	PrototypeAST(const std::string &Name1, std::map<std::string,VarAndPointType*> Args1, ReturnType& returnType1)
		: Name(Name1),returnType(returnType1),StructureAST(ASTType::proto){
        Args = Args1;
	}

	const std::string &getName() const { return Name; }
    const ReturnType &getReturnType() const{ return returnType;}
};

/// FunctionAST - This class represents a function definition itself.
class FunctionAST : public StructureAST
{
	std::unique_ptr<PrototypeAST> Proto;
	std::unique_ptr<CommandAST> Body; 

public:
	FunctionAST(std::unique_ptr<PrototypeAST> Proto1, std::unique_ptr<CommandAST> Body1)
	           :StructureAST(ASTType::function){
		Proto = std::move(Proto1);
		Body = std::move(Body1);
	}

};