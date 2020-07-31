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
#include "llvm/IR/Instructions.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/Support/Error.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Transforms/Utils/ModuleUtils.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/Support/ErrorHandling.h" 
#include <utility>
#include <vector>
#include <map>
#include <string>
#include <climits> 
#include "Scope.h"
#include "IntConst.h"

using namespace llvm;
using namespace llvm::Intrinsic;

std::unique_ptr<Module>& getModule();
void callError(std::string str,int line);
void initCheck(std::string check);
void initModule(std::string fileName);
void CallInitFunction();
int getBitOfInt(std::string value, bool isSigned);
int getBitOfAPInt(llvm::APInt value, bool isSigned);
bool checkRange(std::string str, bool isSignd);
bool checkRange(llvm::APInt apint, bool isSigend);
void createBr(std::string errorMessage,Value* cmp,int line,std::string bbNameE, std::string bbNameN);
void isCheckLevelValid();
//other functions are at the end of the file

extern std::map<int,std::string> maxIntSignedValue;
extern std::map<int,std::string> minIntSignedValue;
extern std::map<int,std::string> maxIntUnSignedValue;
extern std::map<int,std::string> minIntUnSignedValue;

extern LLVMContext TheContext;
extern IRBuilder<> Builder;
extern std::unique_ptr<Module> TheModule;
extern std::unique_ptr<TargetMachine> TM;
extern bool* doCheck;
extern llvm::Type* sizet;

//the scope is at the end of the file

[[noreturn]] void error(std::string msg);
[[noreturn]] extern void Bug(const char * info,int lineN);
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

enum class ASTType{
	var_or_pointer,
	arrayIndex,
	number,
    stringT,
	call,
    boolT,

	unary,
	binary,
	
	ifT,
	forT,
    whileT,
    breakT,
    newT,
    freeT,
	varDef,
    strDef,
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

enum CheckLevel{
	check_arith,
	check_free,
	check_array_bound
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
    virtual bool isString() const = 0;
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

    llvm::Type* getLLVMType() const{
        if(isVoid){
            return llvm::Type::getVoidTy(TheContext);
        }else {
            return qType->getLLVMType();
        }
        
    }

    void printAST() const;

};

class StringType : public QType{
public:
    StringType():QType(false){

    }
    bool getIsPointer() const{return false;}
    llvm::Type* getLLVMType() const{
        return PointerType::get(Type::getIntNTy(TheContext,8), 0);
    }
    void printAST() const; 
    bool isConstant() const{return false;}
    bool isString() const{return true;}
    
    bool compare(QType const* ty) const{
        if(ty->isString()) return true;
        return false;
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
        case 256: //for contant num
            break;
        default:
            Bug("invalid int width",width);
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
    bool isString() const{
        return false;
    }
    bool getIsPointer() const{
        return false;
    }

    void printAST() const;
    llvm::Type* getLLVMType() const{
        check_valid();
        return Type::getIntNTy(TheContext,width);
    }

    virtual bool compare(QType const* ty) const {
      if (ty->getIsPointer()) return false;
      IntType const*ity = dynamic_cast<IntType const*>(ty);
      return getSigned()==ity->getSigned() && getWidth()==ity->getWidth();
    }
    static IntType bool_type;

};


class PointType : public QType{
    QType* elementType;
    bool isnull;
    StructType* stype = nullptr;
    void initStruct(){

        if(doCheck[CheckLevel::check_array_bound] || doCheck[CheckLevel::check_free])
            stype = StructType::get(TheContext,{sizet, llvm::PointerType::get(elementType->getLLVMType(),0)});

    }
public:
    PointType(QType* elementType1,bool isn = false) : QType(true){
        elementType = elementType1;
        isnull = isn;
        initStruct();
    }

    QType* const getElementType() const{
        return elementType;
    }

    StructType* getStructType() const{
        return stype;
    }

    llvm::PointerType* getLLVMType() const{
        
        if(stype != nullptr)
            return llvm::PointerType::get(stype,0);  
        else
            return llvm::PointerType::get(elementType->getLLVMType(),0); 
        
    }

    void setStructType(StructType* s){
        stype = s;
    }

    void printAST() const;

    virtual bool compare(QType const* ty) const {
      if (!ty->getIsPointer()) return false;
      PointType const*pty = dynamic_cast<PointType const*>(ty);
      return getElementType()->compare(pty->getElementType());
    }

    bool isNull() const{
        return isnull;
    }
    void setNull(bool isN){
        isnull = isN;
    }

    bool isConstant() const{
        return false;
    }
    bool isString() const{
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
    IntConst value;

public:
    ConstantType(std::string v, int line) : QType(false), value(v,line) { }
    ConstantType(IntConst v, int line) : QType(false), value(v) {
    }
    IntConst getValue() const {
        return value;
    }
    unsigned  getWidth() const{
        return value.getWidth();
    }
    bool isNegative() const{
        return value.isNegative();
    }
    bool isConstant() const{
        return true;
    }
    bool isString() const{
        return false;
    }
    bool getIsPointer() const{
        return false;
    }
    llvm::Type* getLLVMType() const{
        int width = getWidth();
        if(width<8) width = 8;
        else if(width<16) width = 16;
        else if(width<32) width = 32;
        else if(width<64) width = 64;
        else if(width<128) width = 128;
        else if(width<256) width = 256;

        return IntType(isNegative(),width).getLLVMType();
    }

    void printAST() const;

    bool compare(QType const* ty) const{
        if(!ty->isConstant())
            return false;
        ConstantType const* ct = dynamic_cast<ConstantType const*>(ty);

        return value.eq(ct->getValue()); 
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


//Base class
class AST{
protected:
    ASTType astType;
    int line = 0;

    [[noreturn]] virtual void lerror(std::string msg) { error("Line " + std::to_string(line) + ": " + msg); }

public:
    AST(ASTType type, int _line) : astType(type), line(_line) {}
    virtual ~AST(){}
    ASTType getType(){
        return astType;
    }
    virtual void printAST(int level=0){}
};

// Expression
class ExprAST : public AST{
public:
    ExprAST(ASTType type, int l):AST(type,l){}
    virtual ~ExprAST(){}
    virtual QValue* codegen()=0;
    virtual void printAST(int level=0){}
};

// Commands
class CommandAST : public AST{
protected:
    bool totallyRet; 
    bool hasBreak;
public:
    CommandAST(ASTType type,int l,bool ret,bool isbreak1=false):AST(type,l){
        totallyRet = ret;
        hasBreak = isbreak1;
    }

    virtual ~CommandAST(){}
    virtual void codegenCommand()=0; // Throws error
    virtual void printAST(int level=0){}
    virtual bool isRet(){
        return totallyRet;
    }
    virtual bool isBreak(){
        return hasBreak;
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


extern Scope<QAlloca,QFunction,QGlobalVariable,ReturnType,QValue> scope;
QValue* assignCast(QValue* varValue, QType* leftT);
QValue* constAdjustSign(QValue* num, bool isSigned);
QValue* upCast(QValue* qv, IntType* type);
