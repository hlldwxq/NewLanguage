#include "../header/AST.h"
#include "../header/ASTExpr.h"
#include "../header/ASTCommand.h"
#include "../header/ASTStructure.h"

void PrototypeAST::codegenStructure(){

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

    bool success = scope.addFunction(Name,functionQ);
    if(!success){
        error("the function has been declared");
    }
}

void FunctionAST::codegenStructure(){

    Proto->codegenStructure();
    Function* function = scope.getFunction(Proto->getFunctionName())->getFunction();
    auto &P = *Proto;

    BasicBlock *BB = BasicBlock::Create(TheContext, "entry", function);

    /*
    BasicBlock *returnBB = NULL;
    if(!P.getReturnType()->getIsVoid()){
        if(Body->isRet() && Parser::getReturnNum()>1){ 
            //the last instruction is return and it is the only return
            returnBB = BasicBlock::Create(TheContext, "returnBB", function);
        }
    }
    */
    
	Builder.SetInsertPoint(BB);

    scope.addScope();
   // printf("func1\n");
    scope.setRetType(P.getReturnType());
   // printf("func2\n");
//     llvm::AllocaInst* retAlloca;
//     if(!P.getReturnType()->getIsVoid()){
//         retAlloca = Builder.CreateAlloca(P.getReturnType()->getLLVMType(), ConstantInt::get(Type::getInt32Ty(TheContext), 1));
//         scope.setReturnAlloca(new QAlloca(P.getReturnType()->getType(),retAlloca));
//        // scope.addSymbol("return",new QAlloca(P.getReturnType()->getType(),retAlloca));
//     }

    std::vector<std::pair<std::string,QType*>> args = P.getArgs();
   // printf("func2.5\n");
    int i=0;
    //args
    for(auto &Arg : function->args()){
        std::string name = args[i].first;
       // printf("func2.61\n");
        QType* t = args[i].second;
       // printf("func2.62\n"); 
        Type* y = t->getLLVMType();
       // printf("func2.621\n");
        Value* v =ConstantInt::get(Type::getInt32Ty(TheContext),1);
      //  printf("func2.622\n");
        llvm::AllocaInst* Alloca = Builder.CreateAlloca(t->getLLVMType(), ConstantInt::get(Type::getInt32Ty(TheContext), 1), name);
      //  printf("func2.63\n");
        QAlloca* allo = new QAlloca(t, Alloca);
      //  printf("func2.64\n");
        if(!scope.addSymbol(name,allo)){
            error("the identifier has been declared");
        }
      //  printf("func2.7\n");
        Builder.CreateStore(&Arg, Alloca);
        i++;
    }
   // printf("func3\n");
    Body->codegenCommand();
   // printf("func4\n");
    if (!Body->isRet()) {
      if (!P.getReturnType()->getIsVoid()) error("non-void function needs return");
      Builder.CreateRetVoid();
    }
   // printf("func5\n");
    scope.removeScope();
  //  printf("func6\n");

//     if(returnBB != NULL){
//         //make sure the returnBB at the end of func
//         returnBB->moveAfter(&(function->getBasicBlockList().back()));
//         Builder.SetInsertPoint(returnBB);
//     }
//
//     if(P.getReturnType()->getIsVoid()){
//         Builder.CreateRetVoid();
//     }
//     else{
//         llvm::Value* retValue = Builder.CreateLoad(retAlloca);
// 		Builder.CreateRet(retValue);
//     }
//
//     scope.removeScope();
    
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

void VarDefAST::codegenStructure(){

    GlobalVariable* globalV = globalInit();
    assert(globalV);

    QValue* qvalue = value->codegen();
    if(!(qvalue->getType()->isConstant())){
        CommandAST::lerror("the value of global variable must be constant");
    }

    ConstantType* numType = dynamic_cast<ConstantType*>(qvalue->getType());
    if(numType->getValue() != 0){
        IntType* leftType = dynamic_cast<IntType*>(type);
        if(leftType->getSigned() && numType->getValue()<0){
            CommandAST::lerror("cannot assign a negative value to an unsigned variable");
        }
        if(getSuitableWidth(numType->getValue(),leftType->getSigned()) > leftType->getWidth()){
            CommandAST::lerror("type cannot be converted");
        }
        
        Function* F = globalInitFunc();
        llvm::Value* rightV = ConstantInt::get(leftType->getLLVMType(),numType->getValue()); 
        Builder.CreateStore(rightV, globalV);
        Builder.CreateRetVoid();
        std::vector<QType*> argsT;
        scope.addInitFunction(new QFunction(new ReturnType(),argsT,F));
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
        Builder.CreateStore(rightV, globalV);
        Builder.CreateRetVoid();
        std::vector<QType*> args;
        scope.addInitFunction(new QFunction(new ReturnType(),args,F));
    }
}
