#ifndef SCOPE_H
#define SCOPE_H

#include <vector>
#include <map>
#include <string>
#include <iostream>
extern llvm::IRBuilder<> Builder;
template <class T, class T1, class T2, class T3, class T4>
class Scope{

    std::vector<typename std::map<std::string,const T*>> symbolTable;
    //typename std::map<std::string,T4*> argTable;
    typename std::map<std::string,const T1*> functionTable;
    typename std::map<std::string,const T2*> globalVariable;
    std::vector<const T1*> initFunction; //init global var
    std::map<std::string, llvm::Value*> strLiteral; //str map
    const T3* retType = NULL;
    llvm::BasicBlock* breakBB = NULL;

public:

    void addStr(std::string str, llvm::Value* s){
        strLiteral[str] = s;
    }

    llvm::Value* findStr(std::string str){
        typename std::map<std::string,llvm::Value*>::iterator iter = strLiteral.find(str);
        if(iter != strLiteral.end()){
            return iter->second;
        }
        return NULL;
    }

    void addInitFunction(T1* f){
        initFunction.push_back(f);
    }

    std::vector<const T1*> getInitFunction(){
        return initFunction;
    }

    bool addFunction(std::string name, T1* qf){
        std::string fn = name;
        if(functionTable.find(fn)!=functionTable.end()){
            return false; //cannot get the line number, thus ask codegen to emit error info
        }
        functionTable[fn] = qf;
        return true;
    }

    const T1* getFunction(std::string fname){
        if(functionTable.find(fname)!=functionTable.end()){
            return functionTable[fname]; //cannot get the line number, thus ask codegen to emit error info
        }
        return NULL;
    }

    /*void cleanArg(){
        argTable.clear();
    }

    bool addArg(std::string name , T4* value){
        
        if(argTable.find(name) != argTable.end()){
            return false; //cannot get the line number, thus ask codegen to emit error info
        }

        argTable[name] = value;
        return true;
    }

    bool addArg(std::string name , const T* all){
        
        std::map<std::string,const T*> &scope = symbolTable.front();
        scope[name] = all;
        return true;
    }

    T4* findArg(std::string name){
        typename std::map<std::string,T4*>::iterator iter = argTable.find(name);
        if(iter != argTable.end()){
            return iter->second;
        }
        return NULL;
    }*/

    bool addSymbol(std::string name , T* Alloca){
        if(symbolTable.size()==0) {printf("no scope"); exit(1);}

        std::map<std::string,const T*> &scope = symbolTable.back();
        if(scope.find(name) != scope.end()){
            return false; //cannot get the line number, thus ask codegen to emit error info
        }

        std::map<std::string,const T*> &scope1 = symbolTable.front();
        if(symbolTable.size()==2 && scope1.find(name) != scope1.end()){
            return false;
        }

        /*std::map<std::string,const T*> &scope1 = symbolTable.front();
        if(symbolTable.size()==2 && scope1.find(name) != scope1.end()){
            return false; //cannot get the line number, thus ask codegen to emit error info
        }*/

        if(globalVariable.find(name) != globalVariable.end()){
            return false; //cannot get the line number, thus ask codegen to emit error info
        }

        scope[name] = Alloca;
        return true;
    }

    const T* findSymbol(std::string name){

        for( int i = symbolTable.size()-1 ; i>=0 ; i--){
            typename std::map<std::string,const T*> symbolMap = symbolTable[i];
            typename std::map<std::string,const T*>::iterator iter = symbolMap.find(name);
            if(iter != symbolMap.end()){
                return iter->second;
            }
        }

      /*  typename std::map<std::string,const T*>::iterator iter = argTable.find(name);
        if(iter != argTable.end()){
            return iter->second;
        }*/
        return NULL;
    }

    void addScope(){
        symbolTable.push_back(std::map<std::string,const T*>());
    }

    bool removeScope(){
        if(symbolTable.size()==0){
            return false;
        }
        symbolTable.pop_back(); 
        if(symbolTable.size() == 0){
          retType=NULL;
        }
        return true;
    }


    bool addGlobalVar(std::string name, T2* gv){
        
        if(globalVariable.find(name)!=globalVariable.end()){
            return false; //cannot get the line number, thus ask codegen to emit error info
        }
        globalVariable[name] = gv;
        return true;
    }

    const T2* findGlobalVar(std::string name){
        if(globalVariable.find(name)!=globalVariable.end()){
            return globalVariable[name]; 
            //cannot get the line number, thus ask codegen to emit error info
        }
        return NULL;
    }

    const T3* getRetType() {assert(retType); return retType;}
    void setRetType(const T3* r) {retType = r;}

    llvm::BasicBlock* getBreakBB(){ return breakBB; }
    void setBreakBB(llvm::BasicBlock* bb){ breakBB = bb; }

};
#endif
