#include "../header/AST.h"
#include "../header/ASTExpr.h"
#include "../header/ASTCommand.h"
#include "../header/ASTStructure.h"

void PrototypeAST::codegenStructure(){
    codegenStructure(false);
}
void PrototypeAST::codegenStructure(bool hasDef){

    if( hasDef && scope.getFunctionProto(Name) != NULL ){
        /*
            do not declare the function twice
            def void func();
            def void func(){...}
        */
        return;
    }

    std::vector<llvm::Type*> args;
    
    for(int i=0;i<Args.size();i++){
        args.push_back(Args[i].second->getLLVMType());
    }
    
    llvm::Type* returnT = returnType->getLLVMType();
    FunctionType *FT = FunctionType::get(returnT,args,false);
    Function *F = Function::Create(FT,Function::ExternalLinkage, Name, TheModule.get());
    
    int idx = 0;
    for(auto &Arg : F->args()){
        Arg.setName(Args[idx].first);
        idx++;
    }

    std::vector<QType*> argsType;
    for(int i=0;i<Args.size();i++){
        argsType.push_back(Args[i].second);
    }
    QFunction* functionQ = new QFunction(returnType, argsType ,F);

    bool success = scope.addFunctionProto(Name,functionQ);
    if(!success){
        lerror("the function has been declared");
    }
}

void FunctionAST::codegenStructure(){

    Proto->codegenStructure(true);

    const QFunction* qfunction = scope.getFunctionProto(Proto->getFunctionName());
    if(qfunction == NULL){
        lerror("the function has not been declared");
    }

    Function* function = qfunction->getFunction();
    scope.removeFunctionProto(Proto->getFunctionName());
    scope.addFunction(Proto->getFunctionName(),qfunction);
    
    auto &P = *Proto;

    BasicBlock *BB = BasicBlock::Create(TheContext, "entry", function);
    
	Builder.SetInsertPoint(BB);
    scope.addScope(doCheck[CheckLevel::check_free] || doCheck[CheckLevel::check_array_bound]);

    scope.setRetType(P.getReturnType());

    std::vector<std::pair<std::string,QType*>> args = P.getArgs();

    int i=0;
    //args
    for(auto &Arg : function->args()){
        std::string name = args[i].first;
        QType* t = args[i].second;
        llvm::AllocaInst* Alloca = Builder.CreateAlloca(t->getLLVMType(), ConstantInt::get(Type::getInt32Ty(TheContext), 1), name);

        QAlloca* allo = new QAlloca(t, Alloca);
        //QAlloca* allo = new QAlloca(t,&Arg);
        if(!scope.addSymbol(name,allo)){
            error("the identifier has been declared");
        }

        Builder.CreateStore(&Arg, Alloca);
       /*  if(!scope.addArg(name,new QValue(t,&Arg))){
            error("the identifier has been declared at line: "+std::to_string(line));
        }*/
        i++;
    }

    Body->codegenCommand();

    if (!Body->isRet()) {
      if (!P.getReturnType()->getIsVoid()) error("non-void function needs return");
      Builder.CreateRetVoid();
    }

    //scope.removeScope();
    scope.removeScope(doCheck[CheckLevel::check_free] || doCheck[CheckLevel::check_array_bound]);

}

Function* DefAST::globalInitFunc(){

    std::vector<llvm::Type*> args;
    
    llvm::Type* returnT = llvm::Type::getVoidTy(TheContext);
    FunctionType *FT = FunctionType::get(returnT,args,false);
    Function *F = Function::Create(FT,Function::InternalLinkage, "init_global_var_"+name, TheModule.get());

    BasicBlock *BB = BasicBlock::Create(TheContext, "entry", F);
    Builder.SetInsertPoint(BB);
    return F;
}

GlobalVariable* DefAST::globalInit(){

    llvm::Constant* constantP = Constant::getNullValue(type->getLLVMType());
    
    GlobalVariable * globalV = new GlobalVariable(type->getLLVMType(),false, 
                                    GlobalValue::CommonLinkage,constantP,name); 
    TheModule->getGlobalList().push_back(globalV);
    if(!scope.addGlobalVar(name,new QGlobalVariable(type,globalV))){
        CommandAST::lerror("the gloabal variable has been declared");
    }

    return globalV;
}

void createFunction(llvm::Value* rightV, GlobalVariable* globalV, Function* F){
    Builder.CreateStore(rightV, globalV);
    Builder.CreateRetVoid();
    std::vector<QType*> argsT;
    scope.addInitFunction(new QFunction(new ReturnType(),argsT,F));
}

void VarDefAST::codegenStructure(){

    GlobalVariable* globalV = globalInit();

    assert(globalV);

    QValue* qvalue = value->codegen();
    if(!(qvalue->getType()->isConstant())){
        CommandAST::lerror("the value of global variable must be constant");
    }

    ConstantType* numType = dynamic_cast<ConstantType*>(qvalue->getType());
    if(!numType->getValue().isZero()) {

        qvalue = assignCast(qvalue,type);
        llvm::Value* rightV = qvalue->getValue();
        Function* F = globalInitFunc();
        
        createFunction(rightV, globalV, F);
        
    }

}

void ArrayDefAST::codegenStructure(){

    GlobalVariable* globalV = globalInit();

    if(value->getType()!=ASTType::nullT){
        
        Function* F = globalInitFunc();
        QValue* rightQv = value->codegen();
        QType* rightQt = rightQv->getType();
        Value* rightV = rightQv->getValue();
        if(!type->compare(rightQt)){
            CommandAST::lerror("The types on both sides of the equal sign must be the same");
        }

        createFunction(rightV, globalV, F);
        
    }
}

void StrDefAST::codegenStructure(){
    
    if(value->getType()!= ASTType::stringT){
        CommandAST::lerror("The value of global string must be string literal");
    }
    GlobalVariable * globalV = globalInit();
    Function* F = globalInitFunc();
    QValue* qvalue = value->codegen();

    qvalue = assignCast(qvalue,type);
    llvm::Value* rightV = qvalue->getValue();
    createFunction(rightV, globalV, F);

}
