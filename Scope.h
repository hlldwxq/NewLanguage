#include "AST.h"
#include <vector>
#include <map>
#include <string>

extern void ErrorQ(const char* info, int lineN);

class QAlloca{
    QType* qtype;
    llvm::AllocaInst* allocaI;
public:
    QAlloca(QType* qt,llvm::AllocaInst* a){
        qtype = qt;
        allocaI = a;
    }
    QType* getType() const{
        return qtype;
    }
    llvm::AllocaInst* getAlloca() const{
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

std::vector<std::map<std::string,const QAlloca*>> symbolTable;
std::map<std::string,const QFunction*> functionTable;

bool addFunction(std::string name, QFunction* qf){
    std::string fn = name;
    if(functionTable.find(fn)!=functionTable.end()){
        return false; //cannot get the line number, thus ask codegen to emit error info
    }
    functionTable[fn] = qf;
    return true;
}

const QFunction* getFunction(std::string fname){
    if(functionTable.find(fname)!=functionTable.end()){
        return functionTable[fname]; //cannot get the line number, thus ask codegen to emit error info
    }
    return NULL;
}

bool addSymbol(std::string name , QAlloca* Alloca){
    if(symbolTable.size()==0) {Bug("No scopes",0); exit(1);}
    std::map<std::string,const QAlloca*> &scope = symbolTable.back();
    if(scope.find(name) != scope.end()){
        return false; //cannot get the line number, thus ask codegen to emit error info
    }
    scope[name] = Alloca;
    return true;
}

void addScope(){
    symbolTable.push_back(std::map<std::string,const QAlloca*>());
}

bool removeScope(){
    if(symbolTable.size()==0){
        return false;
    }
    symbolTable.pop_back();
    return true;
}

const QAlloca* findSymbol(std::string name){

    for( int i = symbolTable.size()-1 ; i>=0 ; i--){
        std::map<std::string,const QAlloca*> symbolMap = symbolTable[i];
        std::map<std::string,const QAlloca*>::iterator iter = symbolMap.find(name);
        if(iter != symbolMap.end()){
            return iter->second;
        }
    }
    return NULL;
}
