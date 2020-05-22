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
#include <utility>

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
    voidType
};

class ReturnType{
    protected:
    TypeType typeOfType;

    public:
    ReturnType(TypeType t){
        typeOfType = t;
    }
    virtual ~ReturnType(){}

    TypeType getType(){
        return typeOfType;
    }
    virtual void printAST(){}
};

class VarAndPointType : public ReturnType{
    
    public:
        VarAndPointType(TypeType t):ReturnType(t){}
        virtual ~VarAndPointType(){}
        virtual void printAST(){}
};

class voidType : public ReturnType{
    public:
        voidType():ReturnType(TypeType::voidType){}
        void printAST(){printf("void");}
};

class IntType : public VarAndPointType{
    VarType type;
public:
    IntType(VarType type1) : VarAndPointType(TypeType::intType){
        type = type1;
    }
    VarType getType(){
        return type;
    }
    void printAST(){
        switch(type){
            case VarType::int1:  
                printf("int1");
                break;
            case VarType::int8:  
                printf("int8");
                break;
            case VarType::int16:  
                printf("int16");
                break;
            case VarType::int32:  
                printf("int32");
                break;
            case VarType::int64:  
                printf("int64");
                break;
            case VarType::int128:  
                printf("int128");
                break;
        }
       
    }
};

class PointType : public VarAndPointType{
    VarAndPointType* elementType;
public:
    PointType(VarAndPointType* elementType1) : VarAndPointType(TypeType::pointerType){
        elementType = elementType1;
    }
    VarAndPointType* getElementType(){
        return elementType;
    }
    void printAST(){
        VarAndPointType* p = elementType;
        printf("*");
        while(p->getType()==TypeType::pointerType){
            printf("*");
            p = ((dynamic_cast<PointType*>(p))->getElementType());
        }
        (static_cast<IntType*>(p))->printAST();
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
    virtual ~AST(){}
    int getType();

    virtual void printAST(int level=0){}
};

// Expression
class ExprAST : public AST{
public:
    ExprAST(ASTType type):AST(type){
    }
    virtual ~ExprAST(){}
    Value* codegen();
    virtual void printAST(int level=0){}
};

// Commands
class CommandAST : public AST{
public:
    CommandAST(ASTType type):AST(type){}
    virtual ~CommandAST(){}
    Value* codegen();
    virtual void printAST(int level=0){}
};

// structure
class StructureAST : public AST{
public:
    StructureAST(ASTType type):AST(type){}
    virtual ~StructureAST(){}
    GlobalObject* structureCodegen();
    virtual void printAST(int level=0){}
};

// LeftValue  is also expression
class LeftValueAST : public ExprAST{
public:
    LeftValueAST(ASTType type):ExprAST(type){}
    virtual ~LeftValueAST(){}
    virtual void printAST(int level=0){}
};

// Variable
class VarOrPointerAST : public LeftValueAST{
    std::string name;
    bool isPointer;
public:
    VarOrPointerAST(const std::string &Name, bool isP = false) : LeftValueAST(ASTType::variable) {
		name = Name;
        isPointer = isP;
	}
	const std::string &getName() const { return name; }

    void printAST(int level=0){
        for(int i=0;i<level;i++){
            printf(" ");
        }
        if(isPointer){
            printf("Pointer Name: ");
        }else{
            printf("Variable Name: ");
        }

        printf("%s",name.c_str());
    }
};

// arrayIndex
class ArrayIndexExprAST: public LeftValueAST{
    std::unique_ptr<ExprAST> pointer; 
    std::unique_ptr<ExprAST> index;
public:
    ArrayIndexExprAST(std::unique_ptr<ExprAST> p, std::unique_ptr<ExprAST> index1)
                     : LeftValueAST(ASTType::arrayIndex) {
        pointer = std::move(p);
        index = std::move(index1);
    }

    void printAST(int level=0){
        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("array index: \n");
        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("  left: \n");
        pointer->printAST(level+4);
        printf("\n");

        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("  index: \n");
        index->printAST(level+4);
    }
};

// Constant Number
class NumberExprAST : public ExprAST{

    long long value;
public:
    NumberExprAST(long long val):ExprAST(ASTType::number){
        value = val;
    }
    void printAST(int level=0){
        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("number: %lld",value);
    }
};

// NullPointer
class NullExprAST : public ExprAST{
public:
    NullExprAST():ExprAST(ASTType::nullT){}

    void printAST(int level=0){
        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("null pointer");
    }
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

    void printAST(int level=0){

        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("function call:\n");
       
        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("  name: %s\n",functionName.c_str());
        
        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("  args: \n");
       
        for(int i=0;i<args.size();i++){
            for(int j=0;j<level;j++){
                printf(" ");
            }
            printf("  arg%d: \n",i+1);
            args[i]->printAST(level+4);
            printf("\n");
        }

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

    void printAST(int level=0){

        printf("unary expr:\n"); 
        
        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("  operator: ");
        switch(opCode){
            case Operators::minus:
            printf("-");
            break;
            case Operators::exclamation_point:
            printf("!");
            break;
        }
        printf("\n");
        
        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("  operand: \n");
        Operand->printAST(level+4);
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

    void printAST(int level=0){
        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("binary expr: \n");

        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("  left:\n");
        LHS->printAST(level+4);
        printf("\n");

        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("  operator: ");
        switch(op){
            case Operators::equal_sign:  //==
                printf("==");
                break;
	        case Operators::not_equal:   //!=
                printf("!=");
                break;
	        case Operators::less_equal:  //<=
                printf("<=");
                break;
	        case Operators::more_equal:  //>=
                printf(">=");
                break;
	        case Operators::plus_sign:   //+
                printf("+");
                break;
            case Operators::minus:       //-
                printf("-");
                break;
            case Operators::star:        //*
                printf("*");
                break;
	        case Operators::disvision:   // /
                printf("/");
                break;
        	case Operators::more_sign:   // >
                printf(">");
                break;
	        case Operators::less_sign:   // <
                printf("<");
                break;
	        case Operators::assignment:  // =
                printf("=");
                break;
            case Operators::andT:        // &
                printf("&");
                break;
	        case Operators::orT:         // |
                printf("|");
                break;
            default:
                printf("error binary op");
                break;
        }
        printf("\n");

        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("  right:\n");
        RHS->printAST(level+4);
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
    void printAST(int level=0){
        for(int i=0;i<level;i++){
                printf(" ");
            }
        printf("New expr:\n");
        for(int i=0;i<level;i++){
                printf(" ");
            }
        printf("  type: ");
        switch(type){
            case VarType::int1:  
                printf("int1");
                break;
            case VarType::int8:  
                printf("int8");
                break;
            case VarType::int16:  
                printf("int16");
                break;
            case VarType::int32:  
                printf("int32");
                break;
            case VarType::int64:  
                printf("int64");
                break;
            case VarType::int128:  
                printf("int128");
                break;
        }
        printf("\n");
        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("sizes:\n");

        for(int i=0;i<args.size();i++){
            for(int i=0;i<level;i++){
                printf(" ");
            }
            printf(" size%d\n",i+1);
            args[i]->printAST(level+4);
            printf("\n");
        }
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
    IntType* type;
    std::string name;
    std::unique_ptr<ExprAST> value;
public:
    VarDefAST(IntType* type1, std::string n, std::unique_ptr<ExprAST> v, bool global)
             :DefAST(ASTType::varDef,global){
        type = type1;
        name = n;
        value = std::move(v);
    }
    
    void printAST(int level=0){
        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("Variable def:\n");
        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("  type: ");
        type->printAST();
        printf("\n");
        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("  name: %s \n",name.c_str());
        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("  value: \n");
        value->printAST(level+4);

    }
};

///ArrayDefAST
class ArrayDefAST : public DefAST{
    PointType* type;
    std::string name;
    std::unique_ptr<ExprAST> value;
public:
    ArrayDefAST(PointType* type1, std::string n, std::unique_ptr<ExprAST> v,bool global)
               :DefAST(ASTType::arrayDef,global)
    {
        type = type1;
        name = n;
        value = std::move(v);
    }
    void printAST(int level=0){
        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("Array def:\n");
        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("  type: ");
        type->printAST();
        printf("\n");
        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("  name: %s\n",name.c_str());
        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("  value: \n");
        value->printAST(level+4);
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

    void printAST(int level=0){
        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("Assign: \n");
        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("  left:\n");
        left->printAST(level+4);
        printf("\n");
        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("  right:\n");
        right->printAST(level+4);
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

    void printAST(int level=0){
        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("If:\n");

        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("  condition:\n");
        condition->printAST(level+4);
        printf("\n");

        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("  then:\n");
        thenC->printAST(level+4);
        printf("\n");

        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("  else\n");
        elseC->printAST(level+4);
        printf("\n");
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
    void printAST(int level=0){
        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("For:\n");

        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("  start:\n");
        start->printAST(level+4);
        printf("\n");

        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("  condition:\n");
        condition->printAST(level+4);
        printf("\n");
        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("  step: %lld\n",step);
        printf("\n");
        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("  for body:\n");
        body->printAST(level+4);
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
    void printAST(int level=0){
        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("While:\n");

        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("  condition:");
        condition->printAST(level+4);
        printf("\n");

        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("  body:");
        body->printAST(level+4);
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
    void printAST(int level=0){
        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("Return: \n");
        value->printAST(level+4);
    }
};

///breakAST
class BreakAST : public CommandAST{
  
    public:
    BreakAST():CommandAST(ASTType::breakT){}
    void printAST(int level=0){
        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("Break");
    }
};

///blockAST
class BlockAST : public CommandAST{
    std::vector<std::unique_ptr<CommandAST>> cmds;
public:
    BlockAST(std::vector<std::unique_ptr<CommandAST>> c):CommandAST(ASTType::body){
        cmds = std::move(c);
    }
    BlockAST():CommandAST(ASTType::body){
        //std::vector<std::unique_ptr<CommandAST>> c;
        //cmds = c;
    }
    void printAST(int level=0){
        for(int i=0;i<level;i++){
            printf(" ");
        }
        if(cmds.size()==0){
            printf("Block: empty\n");
        }else{
            printf("Block: \n");
            for(int i =0;i<cmds.size();i++){
                cmds[i]->printAST(level+4);
                printf("\n");
            }
        }
    }
};

class PrototypeAST : public StructureAST
{
	std::string Name;
	std::vector<std::pair<std::string,VarAndPointType*>> Args;
    ReturnType* returnType;

public:
	PrototypeAST(const std::string &Name1, std::vector<std::pair<std::string,VarAndPointType*>> Args1, ReturnType* returnType1)
		: Name(Name1),StructureAST(ASTType::proto){
        Args = Args1;
        returnType = returnType1;
	}

    void printAST(int level=0){
        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("Prototype:\n");
        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("  returnType: ");  
        returnType->printAST();
        printf("\n");
        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("  name: %s\n",Name.c_str());
        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("  args:\n");
        for(int i=0;i<Args.size();i++){
            for(int i=0;i<level;i++){
                printf(" ");
            }
            printf("    arg%d :    name: %s\n",i,Args[i].first.c_str());
            for(int i=0;i<level;i++){
                printf(" ");
            }
            printf("              type: ");
            Args[i].second->printAST();
            printf("\n");
        }
    }
	const std::string &getName() const { return Name; }

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

    void printAST(int level=0){
        printf("function: \n");
        Proto->printAST(level+4);
        Body->printAST(level+4);
    }
};