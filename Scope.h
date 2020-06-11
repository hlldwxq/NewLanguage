#include "AST.h"
#include <vector>
#include <map>
#include <string>

extern void ErrorQ(const char* info, int lineN);



template <class T, class T1, class T2>
class Scope{

    std::vector<typename std::map<std::string,const T*>> symbolTable;
    typename std::map<std::string,const T1*> functionTable;
    typename std::map<std::string,const T2*> gloabalVariable;
    std::vector<Function*> initFunction;
public:

    void addInitFunction(Function* f){
        initFunction.push_back(f);
    }

    std::vector<Function*> getInitFunction(){
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
        if(symbolTable.size()==0) {Bug("No scopes",0); exit(1);}
        std::map<std::string,const QAlloca*> &scope = symbolTable.back();
        if(scope.find(name) != scope.end()){
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

    bool addGloabalVar(std::string name, T2* gv){
        
        if(gloabalVariable.find(name)!=gloabalVariable.end()){
            return false; //cannot get the line number, thus ask codegen to emit error info
        }
        gloabalVariable[name] = gv;
        return true;
    }

    const T2* findGloableVar(std::string name){
        if(gloabalVariable.find(name)!=gloabalVariable.end()){
            return gloabalVariable[name]; 
            //cannot get the line number, thus ask codegen to emit error info
        }
        return NULL;
    }
};
