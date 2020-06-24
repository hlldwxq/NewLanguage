#include "../header/AST.h"
#include "../header/ASTExpr.h"
#include "../header/ASTCommand.h"
#include "../header/Scope.h"
void DefAST::codegenCommand(){

    llvm::AllocaInst* Alloca = Builder.CreateAlloca(type->getLLVMType(), ConstantInt::get(Type::getInt32Ty(TheContext), 1), name);
    QAlloca* allo = new QAlloca(type,Alloca);

    if(!scope.addSymbol(name,allo)){
        CommandAST::lerror("repeated symbol");
    }

    QValue* init = value->codegen();

    init = assignCast(init,type);
    if(!init){
        CommandAST::lerror("type cannot be converted");
    }

    Value* store = Builder.CreateStore(init->getValue(), allo->getAlloca());
    if(!store){
        Bug("failed store",1);
    }
}

void AssignAST::codegenCommand(){

    const QAlloca* leftV = left->codegenLeft();
    QValue* rightV = right->codegen();

    rightV = assignCast(rightV,leftV->getType());
    if(!rightV){
        lerror("type cannot be converted");
    }

    llvm::Value* store = Builder.CreateStore(rightV->getValue(), leftV->getAlloca());
    if(!store){
        Bug("failed store",0);
    }

}

void ReturnAST::codegenCommand(){

    //const QAlloca* returnAlloca = scope.findSymbol("return");

    const ReturnType *rt = scope.getRetType();

    if (rt->getIsVoid()) {
      if (value!=NULL) lerror("return from void function cannot return value");
      Builder.CreateRetVoid();
    } else {
      if (value==NULL) lerror("return from non-void function cannot return void");

      QValue* returnQv = value->codegen();
      returnQv = assignCast(returnQv,rt->getType());
      if(!returnQv){
          lerror("type cannot be converted");
      }

      Builder.CreateRet(returnQv->getValue());
    }
/*
    const QAlloca* returnAlloca = scope.getReturnAlloca();
    if(!returnAlloca){
        lerror("The function does not need return value");
    }
    QValue* returnQv = value->codegen();
    
    returnQv = assignCast(returnQv,returnAlloca->getType());
    if(!returnQv){
        lerror("type cannot be converted");
    }

    Builder.createRet(returnQv->getValue());*/

  /*
    Value* store = Builder.CreateStore(returnQv->getValue(), returnAlloca->getAlloca());
    if(!store){
        Bug("failed store",0);
    }

    Function *f = Builder.GetInsertBlock()->getParent();
    for(auto iter = f->getBasicBlockList().begin(); iter != f->getBasicBlockList().end();iter++)
    {
        BasicBlock &bb = *iter;
        if(bb.getName().str() == "returnBB") {
            Builder.CreateBr(&bb); //will not meet an error when there is no returnBB
        }
    }
  */
}

void BlockAST::codegenCommand(){

    scope.addScope();
    for(int i=0 ; i < cmds.size(); i++){
        cmds[i]->codegenCommand();

    }
    
    scope.removeScope();
}

void CallExprAST::codegenCommand(){
    codegen_internal(true);
}

void IfAST::codegenCommand(){
   // printf("if1\n");
    bool needAfterBr = !(isRet() || isBreak() || isRetOrBreak());
   // printf("if2\n");
    QValue* CondV = condition->codegen();
    
    CondV = assignCast(CondV,&IntType::bool_type);
    if(!CondV){
        lerror("type cannot be converted to Boolean");
    }

    Function *TheFunction = Builder.GetInsertBlock()->getParent();

	// Create blocks for the then and else cases.  Insert the 'then' block at the
	// end of the function.
	BasicBlock *ThenBB = BasicBlock::Create(TheContext, "then", TheFunction);
	BasicBlock *ElseBB = BasicBlock::Create(TheContext, "else",TheFunction);
    BasicBlock *MergeBB = NULL;
   // printf("if3\n");
    if(needAfterBr)
	    MergeBB = BasicBlock::Create(TheContext, "afterIf",TheFunction);
   // printf("if4\n");
	Builder.CreateCondBr(CondV->getValue(), ThenBB, ElseBB);
   // printf("if5\n");
    // then
	Builder.SetInsertPoint(ThenBB);
	thenC->codegenCommand();
    if(!thenC->isRet() && !thenC->isBreak() && MergeBB!=NULL){ 
        Builder.CreateBr(MergeBB);
    }
   // printf("if6\n");
	ThenBB = Builder.GetInsertBlock();  // Codegen of 'Then' can change the current block.
   // printf("if7\n");
	// else
	Builder.SetInsertPoint(ElseBB);
	elseC->codegenCommand();
    if(!elseC->isRet() && !elseC->isBreak() && MergeBB!=NULL) {
        Builder.CreateBr(MergeBB);
    }
   // printf("if8\n");
	ElseBB = Builder.GetInsertBlock();
   // printf("if9\n");

	// merge
    if(MergeBB){
        Builder.SetInsertPoint(MergeBB);
    }
    //printf("if10\n");
}

void ForAST::codegenCommand(){

    if(isRet())
        lerror("the command in for loop cannot be total return");

    Function *TheFunction = Builder.GetInsertBlock()->getParent();
	
    scope.addScope(); //this scope is for the start variable

	// Emit the start code first, without 'variable' in scope.
    const QType* startType = start->getType();
    if(startType->getIsPointer()){
        lerror("the start variable must not be pointer");
    }
	start->codegenCommand();

	BasicBlock *ConBB = BasicBlock::Create(TheContext,"conbr", TheFunction);
	BasicBlock *BodyBB = BasicBlock::Create(TheContext,"bodybr", TheFunction);
	BasicBlock *AfterBB = BasicBlock::Create(TheContext,"afterbr", TheFunction);

    scope.setBreakBB(AfterBB);

    //judge condition
	Builder.CreateBr(ConBB);
	Builder.SetInsertPoint(ConBB);

	QValue *Cond = condition->codegen();
    Cond = assignCast(Cond,&IntType::bool_type);
    if(!Cond){
        lerror("type cannot be converted to Boolean");
    }
	Builder.CreateCondBr(Cond->getValue(), BodyBB, AfterBB);

	ConBB = Builder.GetInsertBlock();

	// Emit body block.
	Builder.SetInsertPoint(BodyBB);

	body->codegenCommand();

	BodyBB = Builder.GetInsertBlock();

	// Emit the step value.
    const QAlloca* startAlloca = scope.findSymbol(start->getName());
	Value *CurVar = Builder.CreateLoad(startAlloca->getAlloca(), start->getName());
    QValue* stepVal = step->codegen();
    stepVal = assignCast(stepVal,startAlloca->getType());
    if(!stepVal){
        lerror("the step is invalid because type cannot be converted");
    }

    Value *NextVar = Builder.CreateAdd(CurVar, stepVal->getValue(), "nextvar");
	Builder.CreateStore(NextVar, startAlloca->getAlloca());
	Builder.CreateBr(ConBB);

	// Emit after block.
	Builder.SetInsertPoint(AfterBB);

    scope.removeScope();
    scope.setBreakBB(NULL);

}

void WhileAST::codegenCommand(){
    
    if(isRet())
        lerror("the command in for loop cannot be total return");
    
    Function *TheFunction = Builder.GetInsertBlock()->getParent();

	BasicBlock *ConBB = BasicBlock::Create(TheContext,"conbr", TheFunction);
	BasicBlock *BodyBB = BasicBlock::Create(TheContext,"bodybr", TheFunction);
	BasicBlock *AfterBB = BasicBlock::Create(TheContext,"afterbr", TheFunction);

    scope.setBreakBB(AfterBB);

    //judge condition
	Builder.CreateBr(ConBB);
	Builder.SetInsertPoint(ConBB);

	QValue *Cond = condition->codegen();
    Cond = assignCast(Cond,&IntType::bool_type);
    if(!Cond){
        lerror("type cannot be converted to Boolean");
    }
	Builder.CreateCondBr(Cond->getValue(), BodyBB, AfterBB);

	ConBB = Builder.GetInsertBlock();

	// Emit body block.
//	TheFunction->getBasicBlockList().push_back(BodyBB);
	Builder.SetInsertPoint(BodyBB);

	body->codegenCommand();

	BodyBB = Builder.GetInsertBlock();
	Builder.CreateBr(ConBB);

	// Emit after block.
//	TheFunction->getBasicBlockList().push_back(AfterBB);
	Builder.SetInsertPoint(AfterBB);
    scope.setBreakBB(NULL);
}

void BreakAST::codegenCommand(){

    BasicBlock* breakBB = scope.getBreakBB();
    if(!breakBB)
        lerror("the break command must be in while or for loop");
   // printf("break1\n");
    Builder.CreateBr(breakBB);
   // printf("break2\n");

}

