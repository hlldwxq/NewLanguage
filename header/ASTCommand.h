#pragma once
#include "AST.h"
#include "ASTExpr.h"

class FreeAST : public CommandAST{
    std::unique_ptr<ExprAST> ptr;
    public:
    FreeAST(std::unique_ptr<ExprAST> p,int line):CommandAST(ASTType::freeT,line,false){
        ptr = std::move(p);
    }
    void codegenCommand();
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
    std::string getName(){return name;}
    const QType* getType(){return type;}
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
    void printAST(int level=0);
};

/// StrDefAST
class StrDefAST : public DefAST{
public:
    StrDefAST(StringType* type1, std::string n, std::unique_ptr<ExprAST> v, int line1, bool global)
             :DefAST(type1,n,std::move(v),line1,global,ASTType::strDef){
    }
    void codegenStructure();
    void printAST(int level=0);
};

///ArrayDefAST
class ArrayDefAST : public DefAST{
public:
    ArrayDefAST(PointType* type1, std::string n, std::unique_ptr<ExprAST> v,int line1,bool global)
               :DefAST(type1,n,std::move(v),line1,global,ASTType::arrayDef)
    {}
    void codegenStructure();
    void printAST(int level=0);
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

    void printAST(int level=0);
    void codegenCommand();
};

///ifAST
class IfAST : public CommandAST{
    std::unique_ptr<ExprAST> condition;
    std::unique_ptr<CommandAST> thenC;
    std::unique_ptr<CommandAST> elseC;
    bool retOrBreak;
public:
    IfAST( std::unique_ptr<ExprAST> cond, std::unique_ptr<CommandAST> t, std::unique_ptr<CommandAST> e, int line1,bool isRet, bool isBreak, bool retOrBreak1)
              : CommandAST(ASTType::ifT,line1,isRet,isBreak)
    {
        condition = std::move(cond);
        thenC = std::move(t);
        elseC = std::move(e);
        retOrBreak = retOrBreak1;
    }
    void codegenCommand();
    void printAST(int level=0);
    bool isRetOrBreak(){
        return retOrBreak;
    }

    bool isBreak(){
        return retOrBreak;
    }
};

///forAST
class ForAST : public CommandAST{
    std::unique_ptr<DefAST> start;
    std::unique_ptr<ExprAST> condition;
    std::unique_ptr<ExprAST> step;
    std::unique_ptr<CommandAST> body;
public:
    ForAST(std::unique_ptr<DefAST> start1,std::unique_ptr<ExprAST> condition1,std::unique_ptr<ExprAST> step1,std::unique_ptr<CommandAST> body1, int line1, bool isRet)
          : CommandAST(ASTType::forT,line1,isRet)
    {
        start = std::move(start1);
        condition = std::move(condition1);
        step = std::move(step1);
        body = std::move(body1);
    }
    void printAST(int level=0);
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
    void printAST(int level=0);
    void codegenCommand();
};

///returnAST
class ReturnAST : public CommandAST{
    std::unique_ptr<ExprAST> value;
public:
    ReturnAST(std::unique_ptr<ExprAST> v, int line1) : CommandAST(ASTType::returnT,line1,true){
        value = std::move(v);
    }
    void printAST(int level=0);
    void codegenCommand();
};

///breakAST
class BreakAST : public CommandAST{
    public:
    BreakAST(int line1):CommandAST(ASTType::breakT,line1,false,true){
    }
    void printAST(int level=0);
    void codegenCommand();
};

///blockAST
class BlockAST : public CommandAST{
    std::vector<std::unique_ptr<CommandAST>> cmds;
    public:
    BlockAST(std::vector<std::unique_ptr<CommandAST>> c,int line1,bool isRet,bool isBreak):CommandAST(ASTType::body,line1,isRet,isBreak){
        cmds = std::move(c);
    }
    BlockAST(int line1):CommandAST(ASTType::body,line1,false){}

    void printAST(int level=0);
    void codegenCommand();
};
