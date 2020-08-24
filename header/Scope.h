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
    std::vector<llvm::Value*> freedTable;

    typename std::map<std::string,const T1*> functionProto;
    typename std::map<std::string,const T1*> functionTable;
    std::vector<const T1*> initFunction; //init global var

    typename std::map<std::string,const T2*> globalVariable;
    
    std::map<std::string, llvm::Value*> strLiteral; //str map
    
    const T3* retType = NULL;
    std::vector<llvm::BasicBlock*> breakBB;

public:


    void addInitFunction(T1* f){
        initFunction.push_back(f);
    }

    std::vector<const T1*> getInitFunction(){
        return initFunction;
    }

    bool addFunctionProto(std::string name, const T1* qf){
        
        std::string fn = name;
       
        if(functionProto.find(fn)!=functionProto.end()){
            return false;
        }else{
            if(functionTable.find(fn)!=functionTable.end()){
                return false; //cannot get the line number, thus ask codegen to emit error info
            }
        }
        functionProto[fn] = qf;
        return true;
    }

    const T1* getFunctionProto(std::string fname){
        if(functionProto.find(fname)!=functionProto.end()){
            return functionProto[fname]; //cannot get the line number, thus ask codegen to emit error info
        }
        return NULL;
    }

    const void removeFunctionProto(std::string fname){
        if(functionProto.find(fname)!=functionProto.end()){
            functionProto.erase(functionProto.find(fname));
        }
    }

    bool addFunction(std::string name, const T1* qf){
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
        assert(symbolTable.size()!=0);

        std::map<std::string,const T*> &scope = symbolTable.back();
        if(scope.find(name) != scope.end()){
            return false; //cannot get the line number, thus ask codegen to emit error info
        }

        std::map<std::string,const T*> &scope1 = symbolTable.front();
        if(symbolTable.size()==2 && scope1.find(name) != scope1.end()){
            return false;
        }

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
        return NULL;
    }

    void addFreedPtr(llvm::Value* ptr){
        std::cout<<"; add" <<std::endl;
        freedTable.push_back(ptr);
    }

    void addScope(bool arrayCheck){
        symbolTable.push_back(std::map<std::string,const T*>());
    }

    bool removeScope(bool arrayCheck){
        
        assert(symbolTable.size()!=0);
        symbolTable.pop_back();
        
        if(symbolTable.size() == 1 && arrayCheck){
            for (auto iter : freedTable){
                std::cout<< "; free" <<std::endl;
                llvm::Instruction* struct_free = llvm::CallInst::CreateFree(iter , Builder.GetInsertBlock());
                Builder.Insert(struct_free);
            }

            freedTable.clear();
        }

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

    const T3* getRetType() {assert(retType); return retType;}
    void setRetType(const T3* r) {retType = r;}

    llvm::BasicBlock* getBreakBB(){ 
        if(breakBB.size()<=0){
            return NULL;
        }
        return breakBB[breakBB.size()-1]; 
    }
    void setBreakBB(llvm::BasicBlock* bb){
        if(bb==NULL){
            breakBB.pop_back();
        }else{
            breakBB.push_back(bb);
        }
    }

};
#endif
