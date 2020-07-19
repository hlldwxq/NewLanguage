#pragma once
#include "AST.h"
using namespace llvm;
using namespace llvm::Intrinsic;

[[noreturn]] extern void error(std::string msg);
[[noreturn]] extern void Bug(const char * info,int lineN);
extern bool checkRange(std::string str, bool isSignd);
extern bool checkRange(llvm::APInt apint, bool isSigend);

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

      if (width > std::numeric_limits<unsigned>::max() / 2) error("Constant arithmetic overflow"); 
      // Maybe APInt will hit some limits before that. TODO: Check if it is undefined behaviour then!
      // I checked it, it is not a undefined behaviour

      width*=2; // width++
    }

    if(res.getBitWidth()>128){
        if(checkRange(res,res.isNegative())){
            error("invalid number because it is too big or too small ("+ std::to_string(res.getBitWidth()) +" bits)");
        }
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

