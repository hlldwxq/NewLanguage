#pragma once
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/GlobalVariable.h"
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
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Transforms/Utils/ModuleUtils.h"
#include <utility>
#include <utility>
#include <vector>
#include <map>
#include <string>
#include <climits> 
using namespace llvm;

extern std::unique_ptr<Module> TheModule;
static LLVMContext TheContext;
static IRBuilder<> Builder(TheContext);
void CallInitFunction();
extern void Bug(const char * info,int lineN);

enum class ASTType{
	var_or_pointer,
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
	division,   // /
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

// type class
class QType{
    bool isPointer;
public:
    QType(bool isP){
        isPointer = isP;
    }
    virtual ~QType(){}
    bool getIsPointer() const{
        return isPointer;
    }
    virtual llvm::Type* getLLVMType() const = 0; //{return NULL;};
    virtual void printAST() const = 0; //{}
    virtual const QType* getElementType() const= 0; //{ return NULL; }

    virtual bool compare(QType const* ty) const = 0;

};

class ReturnType{
protected:
    QType* qType;
    bool isVoid;

public:
    ReturnType(QType* q){
        qType = q;
    }
    ReturnType(bool isV = true){
        isVoid = isV;
    }
    virtual ~ReturnType(){}

    QType* getType() const{
        return qType;
    }

    bool getIsVoid() const{
        return isVoid;
    }

    llvm::Type* getLLVMType() const;

    virtual void printAST() const{
        if(isVoid)
            printf("void");
        else{
            qType->printAST();
        }
    }

};

class IntType : public QType{
    bool isSigned;
    unsigned long long width;
public:
    IntType(bool s,unsigned long long w):QType(false){
        isSigned = s;
        width = w;
        check_valid();
    }
    bool getSigned() const{
        return isSigned;
    }
    unsigned long long getWidth() const{
        return width;
    }
    void printAST() const{
        if(isSigned){
            printf("signed ");
        }else{
            printf("unsigned ");
        }

        printf("int%lld",width);
    }
    llvm::Type* getLLVMType() const;
    const QType* getElementType() const{ return NULL; }

    void check_valid() const {
      switch(width){
        case 1:
        case 8:
        case 16:
        case 32:
        case 64:
        case 128:
            break;
        default:
            Bug("invalid int width",0);
            exit(1);
      }
    }


    virtual bool compare(QType const* ty) const {
      if (ty->getIsPointer()) return false;
      IntType const*ity = dynamic_cast<IntType const*>(ty);
      return getSigned()==ity->getSigned() && getWidth()==ity->getWidth();
    }



};

class PointType : public QType{
    QType* elementType;
public:

    PointType(QType* elementType1) : QType(true){
        elementType = elementType1;
    }
    QType* getElementType() const{
        return elementType;
    }
    llvm::PointerType* getLLVMType() const;
    void printAST() const{
        const QType* p = elementType;
        printf("*");
        p->printAST();
    }

    virtual bool compare(QType const* ty) const {
      if (!ty->getIsPointer()) return false;
      PointType const*pty = dynamic_cast<PointType const*>(ty);
      return getElementType()->compare(pty->getElementType());
    }

};

bool ComparePointType(PointType* p1,PointType* p2);

//Value
class QValue{
    QType* qtype;
    llvm::Value* value;
public:
    QValue(QType* q, llvm::Value* v){
        qtype = q;
        value = v;
    }
    QType* getType() const{
        return qtype;
    }
    llvm::Value* getValue() const{
        return value;
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
    ASTType getType(){
        return astType;
    }
    virtual void printAST(int level=0){}
};

// Expression
class ExprAST : public AST{

public:
    ExprAST(ASTType type):AST(type){
       
    }
    virtual ~ExprAST(){}
    virtual QValue* codegen()=0;
    virtual void printAST(int level=0){}
};

// Commands
class CommandAST : public AST{

public:
    CommandAST(ASTType type):AST(type){
    }
    virtual ~CommandAST(){}
    virtual bool codegenCommand()=0;
    virtual void printAST(int level=0){}
};

// structure
class StructureAST : public AST{

public:
    StructureAST(ASTType type):AST(type){
    }
    virtual ~StructureAST(){}
    virtual bool codegenStructure()=0;
    virtual void printAST(int level=0) {}
};

// LeftValue  is also expression
class LeftValueAST : public ExprAST{

public:
    LeftValueAST(ASTType type):ExprAST(type){
    }
    virtual ~LeftValueAST(){}
    virtual void printAST(int level=0){}
    virtual QValue* codegen(); //{ return NULL; }
    virtual QValue* codegenLeft(){ return NULL;}
};

// Variable
class VariableAST : public LeftValueAST{
    std::string name;
    bool isPointer;
    int line;
public:
    VariableAST(std::string Name, int line1, bool isP = false) 
                   :LeftValueAST(ASTType::var_or_pointer) {
		name = Name;
        isPointer = isP;
        line = line1;
	}
	std::string getName() { return name; }

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
//     QValue* codegen();
    QValue* codegenLeft();
};

// arrayIndex
class ArrayIndexExprAST: public LeftValueAST{
    std::unique_ptr<ExprAST> pointer; 
    std::unique_ptr<ExprAST> index;
    int line;
public:
    ArrayIndexExprAST(std::unique_ptr<ExprAST> p, std::unique_ptr<ExprAST> index1,int line1)
                     : LeftValueAST(ASTType::arrayIndex) {
        pointer = std::move(p);
        index = std::move(index1);
        line = line1;
    }
    QValue* codegenLeft();
//     QValue* codegen();
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
    int line;
    // IntegerType ty  --> 7384213U64  7384213S64
public:
    NumberExprAST(long long val,int line1):ExprAST(ASTType::number){
        value = val;
        line = line1;
    }
    long long getValue(){
        return value;
    }
    void printAST(int level=0){
        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("number: %lld",value);
    }
    QValue* codegen();
};

// NullPointer
class NullExprAST : public ExprAST{
    int line;
public:
    NullExprAST(int line1):ExprAST(ASTType::nullT){
        line = line1;
    }

    void printAST(int level=0) {
        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("null pointer");
    }
    QValue* codegen();
};

/// CallExprAST - Expression class for function calls.
class CallExprAST : public ExprAST, public CommandAST
{
	std::string functionName;
	std::vector<std::unique_ptr<ExprAST>> args;
    int line;
public:
	CallExprAST(std::string functionName1, std::vector<std::unique_ptr<ExprAST>> args1,int line1)
		       : ExprAST(ASTType::call), CommandAST(ASTType::call), args(std::move(args1))
    {
        functionName = functionName1;
        line = line1;
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
    bool codegenCommand();
    QValue* codegen();
};

/// UnaryExprAST - Expression class for a unary operator.
class UnaryExprAST : public ExprAST
{
	Operators opCode;					  
	std::unique_ptr<ExprAST> Operand; 
    int line;
public:
	UnaryExprAST(Operators opCode1, std::unique_ptr<ExprAST> Operand1, int line1) : ExprAST(ASTType::unary) {
        opCode = opCode1;
        Operand = move(Operand1);
        line = line1;
    }

    QValue* codegen();

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
            default:
            printf("wrong unary op");
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
    int line;
public:
	BinaryExprAST(Operators OpCode, std::unique_ptr<ExprAST> lhs, std::unique_ptr<ExprAST> rhs,int line1) 
                 : ExprAST(ASTType::binary){
        op = OpCode;
        LHS = std::move(lhs);
        RHS = std::move(rhs); 
        line = line1;
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
	        case Operators::division:   // /
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

    QValue* codegen();

};

/// newExprAST
class NewExprAST : public ExprAST{
    QType* type; 
    std::unique_ptr<ExprAST> size;
    int line;
public:
    NewExprAST(QType* type1, std::unique_ptr<ExprAST> size1, int line1) 
              :ExprAST(ASTType::newT)
    {
        type = type1;
        size = std::move(size1);
        line = line1;
    }
    QType* getType(){
        return type;
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
        type->printAST();
        printf("\n");
        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("sizes:\n");
        size->printAST(level+4);
        printf("\n");
    }
    QValue* codegen();
};

class DefAST : public CommandAST, public StructureAST{
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
    int line;
public:
    VarDefAST(IntType* type1, std::string n, std::unique_ptr<ExprAST> v, bool global,int line1)
             :DefAST(ASTType::varDef,global){
        type = type1;
        name = n;
        value = std::move(v);
        line = line1;
    }
    bool codegenCommand();
    bool codegenStructure();
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
    int line;
public:
    ArrayDefAST(PointType* type1, std::string n, std::unique_ptr<ExprAST> v,bool global,int line1)
               :DefAST(ASTType::arrayDef,global)
    {
        type = type1;
        name = n;
        value = std::move(v);
        line = line1;
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
    bool codegenCommand();
    bool codegenStructure();
};

///AssignAST
class AssignAST : public CommandAST{
    std::unique_ptr<LeftValueAST> left;
    std::unique_ptr<ExprAST> right;
    int line;
public:
    AssignAST(std::unique_ptr<LeftValueAST> l, std::unique_ptr<ExprAST> r, int line1)
             :CommandAST(ASTType::assign){
        left = std::move(l);
        right = std::move(r);
        line = line1;
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
    bool codegenCommand();
};

///ifAST
class IfAST : public CommandAST{
    std::unique_ptr<ExprAST> condition;
    std::unique_ptr<CommandAST> thenC;
    std::unique_ptr<CommandAST> elseC;
    int line;
public:
    IfAST( std::unique_ptr<ExprAST> cond, std::unique_ptr<CommandAST> t, std::unique_ptr<CommandAST> e, int line1)
              : CommandAST(ASTType::ifT)
    {
        condition = std::move(cond);
        thenC = std::move(t);
        elseC = std::move(e);
        line = line1;
    }
    bool codegenCommand();
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
    int line;
public:
    ForAST(std::unique_ptr<CommandAST> start1,std::unique_ptr<ExprAST> condition1,long long step1,std::unique_ptr<CommandAST> body1, int line1)
          : CommandAST(ASTType::forT)
    {
        start = std::move(start1);
        condition = std::move(condition1);
        step = step1;
        body = std::move(body1);
        line = line1;
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
    bool codegenCommand();
};

///whileAST
class WhileAST : public CommandAST{
    std::unique_ptr<ExprAST> condition;
    std::unique_ptr<CommandAST> body;
    int line;
public:
    WhileAST(std::unique_ptr<ExprAST> cond, std::unique_ptr<CommandAST> cmd, int line1)
            : CommandAST(ASTType::whileT){
        condition = std::move(cond);
        body = std::move(cmd);
        line = line1;
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
    bool codegenCommand();
};

///returnAST
class ReturnAST : public CommandAST{
    std::unique_ptr<ExprAST> value;
    int line;
public:
    ReturnAST(std::unique_ptr<ExprAST> v, int line1) : CommandAST(ASTType::returnT){
        value = std::move(v);
        line = line1;
    }
    void printAST(int level=0){
        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("Return: \n");
        value->printAST(level+4);
    }
    bool codegenCommand();
};

///breakAST
class BreakAST : public CommandAST{
    int line;
    public:
    BreakAST(int line1):CommandAST(ASTType::breakT){
        line = line1;
    }
    void printAST(int level=0){
        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("Break");
    }
    bool codegenCommand();
};

///blockAST
class BlockAST : public CommandAST{
    std::vector<std::unique_ptr<CommandAST>> cmds;
    int line;
    public:
    BlockAST(std::vector<std::unique_ptr<CommandAST>> c,int line1):CommandAST(ASTType::body){
        cmds = std::move(c);
        line = line1;
    }
    BlockAST(int line1):CommandAST(ASTType::body){
        //std::vector<std::unique_ptr<CommandAST>> c;
        //cmds = c;
        line = line1;
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
    bool codegenCommand();
};

class PrototypeAST : public StructureAST
{
	std::string Name;
	std::vector<std::pair<std::string,QType*>> Args;
    ReturnType* returnType;
    int line;

public:
	PrototypeAST(const std::string &Name1, std::vector<std::pair<std::string,QType*>> Args1, ReturnType* returnType1,int line1)
		: StructureAST(ASTType::proto),Name(Name1){
        Args = Args1;
        returnType = returnType1;
        line = line1;
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

    std::string getFunctionName(){
        return Name;
    }
    std::vector<std::pair<std::string,QType*>> getArgs(){
        return Args;
    }
    ReturnType* getReturnType(){
        return returnType;
    }
    bool codegenStructure();
};

/// FunctionAST - This class represents a function definition itself.
class FunctionAST : public StructureAST
{
	std::unique_ptr<PrototypeAST> Proto;
	std::unique_ptr<CommandAST> Body; 
    int line;

public:
	FunctionAST(std::unique_ptr<PrototypeAST> Proto1, std::unique_ptr<CommandAST> Body1,int line1)
	           :StructureAST(ASTType::function){
		Proto = std::move(Proto1);
		Body = std::move(Body1);
        line = line1;
	}

    void printAST(int level=0){
        printf("function: \n");
        Proto->printAST(level+4);
        Body->printAST(level+4);
    }
    bool codegenStructure();
};
