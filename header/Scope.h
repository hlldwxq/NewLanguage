#ifndef SCOPE_H
#define SCOPE_H

#include <vector>
#include <map>
#include <string>


template <class T, class T1, class T2, class T3>
class Scope{

    std::vector<typename std::map<std::string,const T*>> symbolTable;
    typename std::map<std::string,const T1*> functionTable;
    typename std::map<std::string,const T2*> globalVariable;
    std::vector<const T1*> initFunction; //init global var
    const T3* retType = NULL;
    llvm::BasicBlock* breakBB = NULL;

public:

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

    bool addSymbol(std::string name , T* Alloca){
        if(symbolTable.size()==0) {printf("no scope"); exit(1);}

        std::map<std::string,const T*> &scope = symbolTable.back();
        if(scope.find(name) != scope.end()){
            return false; //cannot get the line number, thus ask codegen to emit error info
        }

        std::map<std::string,const T*> &scope1 = symbolTable.front();
        if(scope1.find(name) != scope1.end()){
            return false; //cannot get the line number, thus ask codegen to emit error info
        }

        if(globalVariable.find(name) != globalVariable.end()){
            return false; //cannot get the line number, thus ask codegen to emit error info
        }



        scope[name] = Alloca;
        return true;
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

    const T* findSymbol(std::string name){
        for( int i = symbolTable.size()-1 ; i>=0 ; i--){
            typename std::map<std::string,const T*> symbolMap = symbolTable[i];
            typename std::map<std::string,const T*>::iterator iter = symbolMap.find(name);
            if(iter != symbolMap.end()){
                return iter->second;
            }
        }
        return NULL;
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
