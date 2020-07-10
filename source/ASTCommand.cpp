#include "../header/AST.h"
#include "../header/ASTExpr.h"
#include "../header/ASTCommand.h"
#include "../header/Scope.h"

void FreeAST::codegenCommand(){
    QValue* p = ptr->codegen();
    if(!p->getType()->getIsPointer()){
        lerror("what will be freed must be a pointer");
    }

    Value* arraySize = Builder.CreateStructGEP(p->getValue(),0);
    Builder.CreateStore(ConstantInt::get(sizet,0,true),arraySize);

    Value* arrayAddress = Builder.CreateStructGEP(p->getValue(),1);
    arrayAddress = Builder.CreateLoad(arrayAddress);

    Instruction* var_free = CallInst::CreateFree(arrayAddress,Builder.GetInsertBlock());
    Builder.Insert(var_free);
}

void DefAST::codegenCommand(){
    
    llvm::AllocaInst* Alloca = Builder.CreateAlloca(type->getLLVMType(), ConstantInt::get(Type::getInt32Ty(TheContext), 1), name);

    QValue* init = value->codegen();
    init = assignCast(init,type);
        
    if(!init){
        CommandAST::lerror("definition: type cannot be converted");
    }

    QAlloca* allo = new QAlloca(init->getType(),Alloca); //for null ptr
    if(!scope.addSymbol(name,allo)){
        CommandAST::lerror("repeated variable name");
    }

    Value* initV = init->getValue();
    Builder.CreateStore(initV, allo->getAlloca());

}

void AssignAST::codegenCommand(){
    
    QValue* rightV = right->codegen();
    const QAlloca* leftV = left->codegenLeft();

    rightV = assignCast(rightV,leftV->getType());
    if(!rightV){
        lerror("assign: type cannot be converted");
    }

    llvm::Value* rightValue = rightV->getValue();
    llvm::Value* store = Builder.CreateStore(rightValue, leftV->getAlloca());

    if(!store){
        Bug("failed store",0);
    }
}

void ReturnAST::codegenCommand(){

    const ReturnType *rt = scope.getRetType();

    if (rt->getIsVoid()) {
      if (value!=NULL) lerror("return from void function cannot return value");
      Builder.CreateRetVoid();
    } else {
      if (value==NULL) lerror("return from non-void function cannot return void");

      QValue* returnQv = value->codegen();
      returnQv = assignCast(returnQv,rt->getType());
      if(!returnQv){
          lerror("return: type cannot be converted");
      }

      Builder.CreateRet(returnQv->getValue());
    }

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

    bool needAfterBr = !(isRet() || isBreak() || isRetOrBreak());

    Function *TheFunction = Builder.GetInsertBlock()->getParent();

	// Create blocks for the then and else cases.  Insert the 'then' block at the
	// end of the function.
	BasicBlock *ThenBB = BasicBlock::Create(TheContext, "then", TheFunction);
	BasicBlock *ElseBB = BasicBlock::Create(TheContext, "else",TheFunction);
    BasicBlock *MergeBB = NULL;

    if(needAfterBr)
	    MergeBB = BasicBlock::Create(TheContext, "afterIf",TheFunction);

    QValue *Cond = condition->codegen();
    Cond = assignCast(Cond,&IntType::bool_type);
    if(!Cond){
        lerror("if condition: type cannot be converted to Boolean");
    }
	Builder.CreateCondBr(Cond->getValue(), ThenBB, ElseBB);

	Builder.SetInsertPoint(ThenBB);
	thenC->codegenCommand();
    if(!thenC->isRet() && !thenC->isBreak() && MergeBB!=NULL){ 
        Builder.CreateBr(MergeBB);
    }

	ThenBB = Builder.GetInsertBlock();  // Codegen of 'Then' can change the current block.

	Builder.SetInsertPoint(ElseBB);
	elseC->codegenCommand();
    if(!elseC->isRet() && !elseC->isBreak() && MergeBB!=NULL) {
        Builder.CreateBr(MergeBB);
    }

	ElseBB = Builder.GetInsertBlock();

	// merge
    if(MergeBB){
        Builder.SetInsertPoint(MergeBB);
    }
}

void ForAST::codegenCommand(){


    if(isRet()||isBreak())
        lerror("the command in for loop cannot be total return or break");

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
        lerror("for: type cannot be converted to Boolean");
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
    if(!stepVal->getType()->isConstant()){
        lerror("the step of fr command must to be constant");
    }
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
    
    if(isRet()||isBreak())
        lerror("the command in for loop cannot be total return or break");
    
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
        lerror("while: type cannot be converted to Boolean");
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

    Builder.CreateBr(breakBB);

}

