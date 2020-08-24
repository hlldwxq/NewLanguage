#include "AST.h"

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

    void printAST(int level=0);

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
    void codegenStructure(bool hasDef);
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

    void printAST(int level=0);
    void codegenStructure();
};
