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
//other functions are at the end of the file

extern std::map<int,std::string> maxIntSignedValue;
extern std::map<int,std::string> minIntSignedValue;
extern std::map<int,std::string> maxIntUnSignedValue;
extern std::map<int,std::string> minIntUnSignedValue;

extern LLVMContext TheContext;
extern IRBuilder<> Builder;
extern std::unique_ptr<Module> TheModule;
extern std::unique_ptr<TargetMachine> TM;
extern bool doCheck;
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

    llvm::Type* getLLVMType() const{
        if(isVoid){
            return llvm::Type::getVoidTy(TheContext);
        }else {
            return qType->getLLVMType();
        }
        
    }

    void printAST() const;

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
    StructType* stype;
    void initStruct(){
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
        //return llvm::PointerType::get(elementType->getLLVMType(),0); 
        return llvm::PointerType::get(stype,0);  
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
    bool getIsPointer() const{
        return true;
    }

};

/*
  Arbitrary precision signed integer type

  TODO: Move to own file
*/
class IntConst {
private:
  llvm::APInt apValue;

  IntConst(llvm::APInt _apValue) : apValue(_apValue) {}

  template<typename OP> llvm::APInt binop_adjust(OP f, llvm::APInt const &a, llvm::APInt const &b) const {
    unsigned width = std::max(a.getBitWidth(),b.getBitWidth());
    auto aa = a.sextOrTrunc(width);
    auto bb = b.sextOrTrunc(width);
    return f(aa,bb);
  }

  template<typename OP> bool cmpop_adjust(OP f, llvm::APInt const &a, llvm::APInt const &b) const {
    unsigned width = std::max(a.getBitWidth(),b.getBitWidth());
    auto aa = a.sextOrTrunc(width);
    auto bb = b.sextOrTrunc(width);
    return f(aa,bb);
  }

  template<typename OP> llvm::APInt binop_adjust_ovf(OP f, llvm::APInt const &a, llvm::APInt const &b) const {
    unsigned width = std::max(a.getBitWidth(),b.getBitWidth());

    llvm::APInt res;
    while (true) {
      auto aa = a.sextOrTrunc(width);
      auto bb = b.sextOrTrunc(width);

      bool ov;
      res = f(aa,bb,ov);
      if (!ov) break;

      if (width > std::numeric_limits<unsigned>::max() / 2) error("Constant arithmetic overflow"); // Maybe APInt will hit some limits before that. TODO: Check if it is undefined behaviour then!

      width*=2; // width++
    };

    res = res.zextOrTrunc(res.getMinSignedBits()+1);
    if(IntConst(res).gt(IntConst(APInt::getSignedMaxValue(256))) || IntConst(res).lt(IntConst(APInt::getSignedMinValue(128)))){
        error("invalid number because it is too big or too small");
    }
    return res;
  }

public:
  IntConst(std::string v) {
      
    assert(v!="");
    bool sgn = false;
    if(v[0]=='-'){
      sgn = true;
      v.erase(0,1);
    }
    StringRef vv = v;
    vv.getAsInteger(10,apValue);
    //if (vv.getAsInteger(10,apValue)) Bug("Invalid integer, but parser should have ensured a valid one here!",0);

    auto minw = apValue.getActiveBits() + 1;
    apValue = apValue.zextOrTrunc(minw);
    if(sgn){
        apValue = this->uminus().getValue();
    }
  }

  unsigned getWidth() const {return apValue.getBitWidth();}
  bool isNegative() const {return apValue.isNegative();}
  bool isZero() const {return apValue == 0;}

  llvm::APInt getValue() const {return apValue;}

  unsigned minSignedWidth() const {
    unsigned minw = apValue.getMinSignedBits();

    unsigned res = 8;
    while (res<minw) res*=2;

    return res;
  }

  unsigned minUnsignedWidth() const {
    if (isNegative()) Bug("Cannot interpret negative const as unsigned (should be detected further up, where proper lineno is available)",0);
    unsigned minw = apValue.getActiveBits();

    unsigned res = 8;
    while (res<minw) res*=2;

    return res;
  }

  unsigned minWidth(bool isSigned) const { return isSigned?minSignedWidth():minUnsignedWidth(); }

  llvm::APInt getAlignedValue(bool isSigned, unsigned width=0) const {
    unsigned mw = minWidth(isSigned);
    if (width==0) width=mw;
    else if (width < mw) Bug("width too small (should be checked further up where lineo is available)",0);

    return apValue.sextOrTrunc(width);
  }

  IntConst uminus() const {
    llvm::APInt z = llvm::APInt(getWidth(),"0",10);
    return binop_adjust_ovf([](APInt const &a, APInt const &b,bool &ov){return a.ssub_ov(b,ov);},z,apValue);
  }

  IntConst plus(IntConst const& b) const {  return binop_adjust_ovf([](APInt const &a, APInt const &b,bool &ov){return a.sadd_ov(b,ov);},apValue,b.getValue()); }
  IntConst minus(IntConst const& b) const { return binop_adjust_ovf([](APInt const &a, APInt const &b,bool &ov){return a.ssub_ov(b,ov);},apValue,b.getValue()); }
  IntConst mul(IntConst const& b) const {   return binop_adjust_ovf([](APInt const &a, APInt const &b,bool &ov){return a.smul_ov(b,ov);},apValue,b.getValue()); }
  IntConst div(IntConst const& b) const {   return binop_adjust_ovf([](APInt const &a, APInt const &b,bool &ov){return a.sdiv_ov(b,ov);},apValue,b.getValue()); }
  IntConst rem(IntConst const& b) const {   return binop_adjust([](APInt a, APInt b){return a.srem(b);},apValue,b.getValue()); }

  bool eq(IntConst const& b) const { return cmpop_adjust([](APInt const &a, APInt const &b){return a.eq(b);},apValue,b.getValue()); }
  bool ne(IntConst const& b) const { return cmpop_adjust([](APInt const &a, APInt const &b){return a.ne(b);},apValue,b.getValue()); }
  bool lt(IntConst const& b) const { return cmpop_adjust([](APInt const &a, APInt const &b){return a.slt(b);},apValue,b.getValue()); }
  bool gt(IntConst const& b) const { return cmpop_adjust([](APInt const &a, APInt const &b){return a.sgt(b);},apValue,b.getValue()); }
  bool le(IntConst const& b) const { return cmpop_adjust([](APInt const &a, APInt const &b){return a.sle(b);},apValue,b.getValue()); }
  bool ge(IntConst const& b) const { return cmpop_adjust([](APInt const &a, APInt const &b){return a.sge(b);},apValue,b.getValue()); }


  IntConst bit_and(IntConst const &b) const {
    unsigned width = std::max(apValue.getBitWidth(),b.getValue().getBitWidth());
    auto aa = apValue.sextOrTrunc(width);
    auto bb = b.getValue().sextOrTrunc(width);

    return IntConst(aa&=bb);
  }

  IntConst bit_or(IntConst const &b) const {
    unsigned width = std::max(apValue.getBitWidth(),b.getValue().getBitWidth());
    auto aa = apValue.sextOrTrunc(width);
    auto bb = b.getValue().sextOrTrunc(width);

    return IntConst(aa|=bb);
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
    ConstantType(std::string v) : QType(false), value(v) { }
    ConstantType(IntConst v) : QType(false), value(v) {
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

        return value.eq(ct->getValue()); //test to check

//         if(value!="" && ct->getValue()!="" && ct->getValue()==value)
//             return true;
//         else
//             return apValue == ct->getApValue(); //test to check
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


extern Scope<QAlloca,QFunction,QGlobalVariable,ReturnType> scope;
QValue* assignCast(QValue* varValue, QType* leftT);
QValue* constAdjustSign(QValue* num, bool isSigned);
QValue* upCast(QValue* qv, IntType* type);
