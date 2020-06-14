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
#include "llvm/IR/DataLayout.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Transforms/Utils/ModuleUtils.h"
#include <utility>
#include <vector>
#include <map>
#include <string>
#include <climits> 
using namespace llvm;


class error_e : public std::exception {
  std::string msg;

public:
  error_e() : msg("") {}
  error_e(std::string _msg) : msg(_msg) {}

  void specify(std::string m) {
    if (msg != "") msg = m + " >> " + msg;
  }

  virtual const char *what() const throw() {
    return msg.c_str();
  };

};

[[noreturn]] void error(std::string msg);


extern std::unique_ptr<Module> TheModule;
static LLVMContext TheContext;
static IRBuilder<> Builder(TheContext);
void CallInitFunction();
[[noreturn]] extern void Bug(const char * info,int lineN);
[[noreturn]] extern void ErrorQ(const char* info, int lineN);
[[noreturn]] extern void ErrorD(const char* info, int lineN);

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
	greater_equal,  //>=
	greater_than,   // >
	less_than,   // <
	plus,   //+
	minus,       //-
	star,        //*
	division,    // /
	exclamation_point,    // !
	andT,                 // &
	orT                   // |

};

// type class
class QType{
    bool isPointer;
public:
    QType(bool isP){
        isPointer = isP;
    }
    virtual ~QType(){}
    virtual bool getIsPointer() const = 0;
    virtual llvm::Type* getLLVMType() const = 0; 
    virtual void printAST() const = 0; 
    virtual bool isConstant() const = 0;
    virtual bool compare(QType const* ty) const = 0;
};

class ReturnType{
protected:
    QType* qType;
    bool isVoid;

public:
    ReturnType(QType* q){
        qType = q;
        isVoid = false;
    }
    ReturnType(bool isV=false){
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
private:
    bool isSigned;
    unsigned long long width;
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
    bool isConstant() const{
        return false;
    }
    bool getIsPointer() const{
        return false;
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

    virtual bool compare(QType const* ty) const {
      if (ty->getIsPointer()) return false;
      IntType const*ity = dynamic_cast<IntType const*>(ty);
      return getSigned()==ity->getSigned() && getWidth()==ity->getWidth();
    }

};

class PointType : public QType{
    QType* elementType;
    bool isnull;
public:

    PointType(QType* elementType1,bool isn = false) : QType(true){
        elementType = elementType1;
        isnull = isn;
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

    bool isNull() const{
        return isnull;
    }

    bool isConstant() const{
        return false;
    }
    bool getIsPointer() const{
        return true;
    }
};

//The reason why I need the type is becasue,like in the cast:
//uint8 u2 = 9     uint8 u1 = 3 + 8 - u2
//if there is no constantNumber, type of 3+8 will be sint8
//but for minusAST, the ASTType of left operand is BinaryAST, instead of NumberAST
//so it believes that singed cannot plus unsigned 
class ConstantType : public QType{
private:
    long long value;
public:
    ConstantType(long long v):QType(false){
        value = v;
    }
    long long getValue() const{
        return value;
    }
    bool isConstant() const{
        return true;
    }
    bool getIsPointer() const{
        return false;
    }
    llvm::Type* getLLVMType() const;
    void printAST() const{}
    bool compare(QType const* ty) const{
        if(!ty->isConstant())
            return false;
        ConstantType const* ct = dynamic_cast<ConstantType const*>(ty);
        if(ct->getValue()==value)
            return true;
        return false;
    }
};

class QAlloca{
    QType* qtype;
    llvm::Value* allocaI;
public:
    QAlloca(QType* qt,llvm::Value* a){
        qtype = qt;
        allocaI = a;
    }
    QType* getType() const{
        return qtype;
    }
    llvm::Value* getAlloca() const{
        return allocaI;
    }
};

class QFunction{
    ReturnType* returnType;
    std::vector<QType*> argsType;
    Function* function;
public:
    QFunction(ReturnType* returnT, std::vector<QType*> argsT, Function* func){
        returnType = returnT;
        argsType = argsT;
        function = func;
    }
    ReturnType* getReturnType() const{
        return returnType;
    }
    std::vector<QType*> getArgsType() const{
        return argsType;
    }
    Function* getFunction() const{
        return function;
    }
};

class QGlobalVariable{
    QType* type;
    GlobalVariable* globalVar;
public:
    QGlobalVariable(QType* type1, GlobalVariable* globalVar1){
        type = type1;
        globalVar = globalVar1;
    }

    QType* getType() const{
        return type;
    }

    GlobalVariable* getGlobalVariable() const{
        return globalVar;
    }
};

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

class UOperator{
    protected:
    Operators opType;
    public:
    virtual QValue* codegen(QValue* operand) = 0; // TODO: Do never return null, but throw exception
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
    QValue* codegen(QValue* operand){
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
    exclamation(Operators op = Operators::exclamation_point):UOperator(op){}
    void printAST(){
        printf("-");
    }
};

class negative : public UOperator{
    public:
    QValue* codegen(QValue* operand){
        QType* type = operand->getType();
        if(type->getIsPointer()){
            error("- with pointer");
        }

        IntType* intType = dynamic_cast<IntType*>(type);
        if(intType->getSigned()!=true){
            error("! with unsigned");
        }
        llvm::Value* minu = Builder.CreateNeg(operand->getValue());
        return new QValue(type,minu);
    }
    negative(Operators op = Operators::minus):UOperator(op){}
    void printAST(){
        printf("!");
    }
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
    virtual bool isCompareOp() = 0;
    BOperator(Operators op){
        opType = op;
    }
};

class CompareOperator : public BOperator{
    public:
    virtual QValue* codegen(QValue* a, QValue* b) {
        IntType* getSign = dynamic_cast<IntType*>(a->getType());
        llvm::Value* llvm_result = gen_llvm(getSign->getSigned() , a->getValue() , b->getValue());
        return new QValue(new IntType(false,1) , llvm_result);
    }
    virtual bool gen_constant(long long left, long long right) = 0;
    virtual QValue* constantCodegen(long long left, long long right){
        bool result = gen_constant(left,right);
        int i = result ? true : false;
        IntType* qtype = new IntType(false,1);
        llvm::Value* constInt =  ConstantInt::get(qtype->getLLVMType(), i);
        return new QValue(qtype,constInt);
    }
    virtual void printAST() =0;
    CompareOperator(Operators op):BOperator(op){
    }
    virtual bool isCompareOp(){
        return true;
    }
};

class ArithOperator : public BOperator {
    public:
    virtual QValue* codegen(QValue* a, QValue* b) {
        IntType* getSign = dynamic_cast<IntType*>(a->getType());
        llvm::Value* llvm_result = gen_llvm(getSign->getSigned() , a->getValue() , b->getValue());
        return new QValue(a->getType(), llvm_result);
    }
    virtual long long gen_constant(long long left, long long right) = 0;
    virtual void printAST() = 0;
    virtual QValue* constantCodegen(long long left, long long right){
        long long result = gen_constant(left,right);
        ConstantType* qtype = new ConstantType(result);
        llvm::Value* constInt =  ConstantInt::get(qtype->getLLVMType(), result);
        return new QValue(qtype,constInt);
    }
    ArithOperator(Operators op):BOperator(op){}
    virtual bool isCompareOp(){
        return false;
    }
};

class equal_sign : public CompareOperator{
    public:
    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    bool gen_constant(long long left, long long right){
        return left == right;
    }
    void printAST(){
        printf("==");
    }
    equal_sign():CompareOperator(Operators::equal_sign){}
};  // ==

class not_equal : public CompareOperator{
    public:
    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    void printAST(){
        printf("!=");
    }
    bool gen_constant(long long left, long long right){
        return left != right;
    }
    not_equal():CompareOperator(Operators::not_equal){}
};  // !=

class less_equal : public CompareOperator{
    public:
    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    void printAST(){
        printf("<=");
    }
    bool gen_constant(long long left, long long right){
        return left <= right;
    }
    less_equal():CompareOperator(Operators::less_equal){}
};  // <=

class greater_equal : public CompareOperator{
    public:
    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    void printAST(){
        printf(">=");
    }
    bool gen_constant(long long left, long long right){
        return left >= right;
    }
    greater_equal():CompareOperator(Operators::greater_equal){}
};  // >=

class greater_than : public CompareOperator{
    public:
    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    void printAST(){
        printf(">");
    }
    bool gen_constant(long long left, long long right){
        return left > right;
    }
    greater_than():CompareOperator(Operators::greater_than){}
};  // >

class less_than : public CompareOperator{
    public:
    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    void printAST(){
        printf("<");
    }
    bool gen_constant(long long left, long long right){
        return left < right;
    }
    less_than():CompareOperator(Operators::less_than){}
};  // <

class plus : public ArithOperator{
    public:
    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    void printAST(){
        printf("+");
    }
    long long gen_constant(long long left, long long right){
        return left + right;
    }
    plus():ArithOperator(Operators::plus){}
};  // +

class minus : public ArithOperator{
    public:
    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    void printAST(){
        printf("-");
    }
    long long gen_constant(long long left, long long right){
        return left - right;
    }
    minus():ArithOperator(Operators::minus){}
};  // -

class star : public ArithOperator{
    public:
    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    void printAST(){
        printf("*");
    }
    long long gen_constant(long long left, long long right){
        return left * right;
    }
    star():ArithOperator(Operators::star){}
};  // *

class division : public ArithOperator{
    public:
    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    void printAST(){
        printf("/");
    }
    long long gen_constant(long long left, long long right){
        if(right==0){
            error("The divisor cannot be 0");
        }
        return left / right;
    }
    division():ArithOperator(Operators::division){}
};  // /

class andT : public ArithOperator{
    public:
    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    void printAST(){
        printf("&");
    }
    long long gen_constant(long long left, long long right){
        return left & right;
    }
    andT():ArithOperator(Operators::andT){}
};  // &

class orT : public ArithOperator{
    public:
    Value* gen_llvm(bool isSigned, llvm::Value* left, llvm::Value* right);
    void printAST(){
        printf("|");
    }
    long long gen_constant(long long left, long long right){
        return left | right;
    }
    orT():ArithOperator(Operators::orT){}
};  // |

//Base class
class AST{
protected:
    ASTType astType;
    int line = 0;

    [[noreturn]] virtual void lerror(std::string msg) { error("Line " + std::to_string(line) + ": " + msg); }

public:
    AST(ASTType type, int _line) : astType(type), line(_line) { }
    virtual ~AST(){}
    ASTType getType(){
        return astType;
    }
    virtual void printAST(int level=0){}
};

// Expression
class ExprAST : public AST{
public:
    ExprAST(ASTType type, int l):AST(type,l){
       
    }
    virtual ~ExprAST(){}
    virtual QValue* codegen()=0;
    virtual void printAST(int level=0){}
};

// Commands
class CommandAST : public AST{
protected:
    bool totallyRet; 
public:
    CommandAST(ASTType type,int l,bool ret):AST(type,l){
        totallyRet = ret;
    }

    virtual ~CommandAST(){}
    virtual void codegenCommand()=0; // Throws error
    virtual void printAST(int level=0){}
    virtual bool isRet(){
        return totallyRet;
    }
};

// structure
class StructureAST : public AST{
public:
    StructureAST(ASTType type, int l):AST(type,l){
    }
    virtual ~StructureAST(){}
    virtual void codegenStructure()=0;
    virtual void printAST(int level=0) {}
};

// LeftValue  is also expression
class LeftValueAST : public ExprAST{
public:
    LeftValueAST(ASTType type, int l):ExprAST(type,l){
    }
    virtual ~LeftValueAST(){}
    virtual void printAST(int level=0){}
    virtual QValue* codegen(); //{ return NULL; }
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

    const QAlloca* codegenLeft();
};

// arrayIndex
class ArrayIndexExprAST: public LeftValueAST{
    std::unique_ptr<ExprAST> pointer; 
    std::unique_ptr<ExprAST> index;
    int line;
public:
    ArrayIndexExprAST(std::unique_ptr<ExprAST> p, std::unique_ptr<ExprAST> index1,int line1)
                     : LeftValueAST(ASTType::arrayIndex,line1) {
        pointer = std::move(p);
        index = std::move(index1);
    }
    const QAlloca* codegenLeft();

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
    // IntegerType ty  --> 7384213U64  7384213S64
public:
    NumberExprAST(long long val,int line1):ExprAST(ASTType::number,line1){
        value = val;
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
public:
    NullExprAST(int line1):ExprAST(ASTType::nullT,line1){
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
public:
	CallExprAST(std::string functionName1, std::vector<std::unique_ptr<ExprAST>> args1,int line1)
		       : ExprAST(ASTType::call,line1), CommandAST(ASTType::call,line1,false), args(std::move(args1))
    {
        functionName = functionName1;
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
    void codegenCommand();
    QValue* codegen();
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
    void printAST(int level=0){

        printf("unary expr:\n"); 
        
        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("  operator: ");
        opCode->printAST();
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
	BOperator* op;
	std::unique_ptr<ExprAST> LHS, RHS;
    int line;
public:
	BinaryExprAST(BOperator* OpCode, std::unique_ptr<ExprAST> lhs, std::unique_ptr<ExprAST> rhs,int line1) 
                 : ExprAST(ASTType::binary,line1){
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
        op->printAST();
        printf("\n");

        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("  right:\n");
        RHS->printAST(level+4);
    }
    Operators getOperatorType(){
        return op->getOpType();
    }
    bool isCompareOp(){
        return op->isCompareOp();
    }
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
protected:
    bool global;
    QType* type;
    std::string name;
    std::unique_ptr<ExprAST> value;
public:
    DefAST(QType* type1, std::string name1, std::unique_ptr<ExprAST> v, int line1, bool g, ASTType asttype) : CommandAST(asttype,line1,false),StructureAST(asttype,line1){
        type = type1;
        name = name1;
        value = std::move(v);
        global = g;
    }
    virtual void codegenCommand();
    virtual GlobalVariable* globalInit();
    virtual Function* globalInitFunc();
    virtual void codegenStructure() = 0;
    virtual void printAST(int level=0) = 0;
};

/// VarDefAST
class VarDefAST : public DefAST{
public:
    VarDefAST(IntType* type1, std::string n, std::unique_ptr<ExprAST> v, int line1, bool global)
             :DefAST(type1,n,std::move(v),line1,global,ASTType::varDef){
    }
    void codegenStructure();
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
public:
    ArrayDefAST(PointType* type1, std::string n, std::unique_ptr<ExprAST> v,int line1,bool global)
               :DefAST(type1,n,std::move(v),line1,global,ASTType::arrayDef)
    {}
    void codegenStructure();
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
    AssignAST(std::unique_ptr<LeftValueAST> l, std::unique_ptr<ExprAST> r, int line1)
             :CommandAST(ASTType::assign,line1,false){
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
    void codegenCommand();
};

///ifAST
class IfAST : public CommandAST{
    std::unique_ptr<ExprAST> condition;
    std::unique_ptr<CommandAST> thenC;
    std::unique_ptr<CommandAST> elseC;
public:
    IfAST( std::unique_ptr<ExprAST> cond, std::unique_ptr<CommandAST> t, std::unique_ptr<CommandAST> e, int line1,bool isRet)
              : CommandAST(ASTType::ifT,line1,isRet)
    {
        condition = std::move(cond);
        thenC = std::move(t);
        elseC = std::move(e);
    }
    void codegenCommand();
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
    ForAST(std::unique_ptr<CommandAST> start1,std::unique_ptr<ExprAST> condition1,long long step1,std::unique_ptr<CommandAST> body1, int line1)
          : CommandAST(ASTType::forT,line1,false)
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
    void codegenCommand();
};

///whileAST
class WhileAST : public CommandAST{
    std::unique_ptr<ExprAST> condition;
    std::unique_ptr<CommandAST> body;
public:
    WhileAST(std::unique_ptr<ExprAST> cond, std::unique_ptr<CommandAST> cmd, int line1, bool isRet)
            : CommandAST(ASTType::whileT,line1,isRet){
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
    void codegenCommand();
};

///returnAST
class ReturnAST : public CommandAST{
    std::unique_ptr<ExprAST> value;
public:
    ReturnAST(std::unique_ptr<ExprAST> v, int line1) : CommandAST(ASTType::returnT,line1,true){
        value = std::move(v);
    }
    void printAST(int level=0){
        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("Return: \n");
        value->printAST(level+4);
    }
    void codegenCommand();
};

///breakAST
class BreakAST : public CommandAST{
    public:
    BreakAST(int line1):CommandAST(ASTType::breakT,line1,false){
    }
    void printAST(int level=0){
        for(int i=0;i<level;i++){
            printf(" ");
        }
        printf("Break");
    }
    void codegenCommand();
};

///blockAST
class BlockAST : public CommandAST{
    std::vector<std::unique_ptr<CommandAST>> cmds;
    public:
    BlockAST(std::vector<std::unique_ptr<CommandAST>> c,int line1,bool isRet):CommandAST(ASTType::body,line1,isRet){
        cmds = std::move(c);
    }
    BlockAST(int line1):CommandAST(ASTType::body,line1,false){}

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
    void codegenCommand();
};

class PrototypeAST : public StructureAST
{
	std::string Name;
	std::vector<std::pair<std::string,QType*>> Args;
    ReturnType* returnType;

public:
	PrototypeAST(const std::string &Name1, std::vector<std::pair<std::string,QType*>> Args1, ReturnType* returnType1,int line1)
		: StructureAST(ASTType::proto,line1),Name(Name1){
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

    std::string getFunctionName(){
        return Name;
    }
    std::vector<std::pair<std::string,QType*>> getArgs(){
        return Args;
    }
    ReturnType* getReturnType(){
        return returnType;
    }
    void codegenStructure();
};

/// FunctionAST - This class represents a function definition itself.
class FunctionAST : public StructureAST
{
	std::unique_ptr<PrototypeAST> Proto;
	std::unique_ptr<CommandAST> Body; 

public:
	FunctionAST(std::unique_ptr<PrototypeAST> Proto1, std::unique_ptr<CommandAST> Body1,int line1)
	           :StructureAST(ASTType::function,line1){
		Proto = std::move(Proto1);
		Body = std::move(Body1);
	}

    void printAST(int level=0){
        printf("function: \n");
        Proto->printAST(level+4);
        Body->printAST(level+4);
    }
    void codegenStructure();
};
