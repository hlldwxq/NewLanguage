#include "../header/Parser.h"


//================Commands===============//

/// free ::= free expr
std::unique_ptr<FreeAST> Parser::ParseFree(){
    int line1 = lineN;
    if(CurTok != Token::tok_free)
        Bug("call ParseFree, but no free",lineN);

    getNextToken(); //eat return
    std::unique_ptr<ExprAST> ptr = ParseExpr();
    return std::make_unique<FreeAST>(std::move(ptr),line1);

}

/// assign ::= leftvalue = expression
std::unique_ptr<AssignAST> Parser::ParseAssign(std::string name){

    int line1 = lineN;
    std::unique_ptr<LeftValueAST> left;
    
    if(CurTok == Token::left_square_bracket){
        
        std::unique_ptr<VariableAST> pointerName = std::make_unique<VariableAST>(name,true,line1);
        left = ParseArrayIndexExpr(std::move(pointerName));
        if(left == nullptr)
            return nullptr;
    }else{
        left = std::make_unique<VariableAST>(name,false,line1);
    }
    
    if(CurTok != Token::assignment){
        error("unexpected symbol in assign at line: "+std::to_string(lineN));
       // ErrorQ("unexpected symbol in assign", lineN);
       // return nullptr;
    }

    getNextToken(); //eat =

    std::unique_ptr<ExprAST> right = ParseExpr();

    return std::make_unique<AssignAST>(std::move(left),std::move(right),line1);
}

/// identifier ::= varname = expression
///            ::= arrayname[expression1]+ = expression
///            ::= functionName( [expression]* , )
std::unique_ptr<CommandAST> Parser::ParseIdentifier(){

    if(CurTok != Token::tok_identifier)
        Bug("call ParseIdentifier, but no identifier",lineN);

    std::string name = IdentifierStr;
    getNextToken();  // eat identifier
    switch(CurTok){
        case Token::left_paren:
        return ParseCallExpr(name);
        default:
        return ParseAssign(name);
    }
}

/// return ::= return expression
std::unique_ptr<ReturnAST> Parser::ParseReturn(){

    int line1 = lineN;
    if(CurTok != Token::tok_return)
        Bug("call ParseReturn, but no return",lineN);

    getNextToken(); //eat return

    if (CurTok == Token::tok_void) {
      getNextToken(); //eat void

      return std::make_unique<ReturnAST>(nullptr,line1);

    } else {

      std::unique_ptr<ExprAST> value = ParseExpr();
      return std::make_unique<ReturnAST>(std::move(value),line1);
      
    }
}

/// block ::= {  cmd*  }
std::unique_ptr<BlockAST> Parser::ParseBlock(){

    int line1 = lineN;
    if(CurTok != Token::left_brace)
        Bug("call ParseBlock, but no {",lineN);

    getNextToken(); //eat {
    std::vector<std::unique_ptr<CommandAST>> cmds;
    bool hasRet = false;
    bool hasBreak = false;
    while(CurTok != Token::right_brace && CurTok!=Token::tok_eof){
        std::unique_ptr<CommandAST> cmd = ParseCommand();

        if(!hasRet && !hasBreak){ // ignore the command after return
            if(cmd->isRet())
                hasRet = true;
            else if(cmd->isBreak())
                hasBreak = true;
            cmds.push_back(std::move(cmd));
        }
    }

    if(CurTok != Token::right_brace){
        error("expect } at line: "+std::to_string(lineN));
       // ErrorQ("expect }",lineN);
       // return nullptr;
    }
    getNextToken(); //eat }

    return std::make_unique<BlockAST>(std::move(cmds),line1,hasRet,hasBreak);
}

/// if ::= if condition then cmds [else cmds]
std::unique_ptr<IfAST> Parser::ParseIf(){

    int line1 = lineN;
    int retNum = 0;
    if(CurTok != Token::tok_if)
        Bug("call ParseIf, but no if",lineN);

    getNextToken(); //eat if
    
    std::unique_ptr<ExprAST> con = ParseExpr();
    
    if(CurTok != Token::tok_then){
        error("expect then at line: "+std::to_string(lineN));
    }
    getNextToken(); //eat if

    std::unique_ptr<CommandAST> then = ParseCommand();

    if(then->isRet())
        retNum++;

    std::unique_ptr<CommandAST> elseT;
    if(CurTok != Token::tok_else)
        elseT = std::make_unique<BlockAST>(line1);  //empty block
    else {
        getNextToken(); //eat else
        elseT = ParseCommand();
    }

    if(elseT->isRet())
        retNum++;
    bool isRet = then->isRet() && elseT->isRet();
    bool isBreak = then->isBreak() && elseT->isBreak();
    bool retOrBreak = (then->isRet() || then->isBreak()) && (elseT->isRet() || elseT->isBreak()); 
    return std::make_unique<IfAST>(std::move(con),std::move(then),std::move(elseT),line1,isRet,isBreak,retOrBreak);
}

/// def ::= Type variableName [ = expression] 
///     ::= Type arrayName*+ = new Type [expression]+
std::unique_ptr<DefAST> Parser::ParseVariableDef(bool global=false){

    int line1 = lineN;
    if(!isType()){
        Bug("call ParseVariableDef, but no type",lineN);
    }
    
    QType* type = ParseType();

    if(CurTok!=Token::tok_identifier){
        error("expect name at line: "+std::to_string(lineN));
        //ErrorQ("except name", lineN);
        //return nullptr;
    }
    std::string name = IdentifierStr;
    getNextToken(); //eat name

    if(!type->getIsPointer()){
        std::unique_ptr<ExprAST> value;
        if(CurTok != Token::assignment){
            //default value
            value = std::make_unique<NumberExprAST>("0",line1);
        }
        else{
            getNextToken(); //eat =
           
            value = ParseExpr();
            if(value==nullptr)
                return nullptr;              
        }
        return std::make_unique<VarDefAST>(dynamic_cast<IntType*>(type),name,std::move(value),line1,global);
    }else{
        std::unique_ptr<ExprAST> right;
        
        // if the indexs is empty, codegen should init the pointer as null
        if(CurTok == Token::assignment){
           
            getNextToken(); //eat =
            right = ParseExpr();

        }else{ // no assignment, default value is null
            right = std::make_unique<NullExprAST>(line1);
        }
        return std::make_unique<ArrayDefAST>(dynamic_cast<PointType*>(type),name,std::move(right),line1,global);
    }
}

/// for ::= for start, cond, step cmds
std::unique_ptr<ForAST> Parser::ParseFor(){

    int line1 = lineN;
    if(CurTok != Token::tok_for)
        Bug("call ParseFor, but no for",lineN);

    getNextToken(); //eat for

    std::unique_ptr<DefAST> start = ParseVariableDef(false);
    if(start == nullptr)
        return nullptr;
    
    if(CurTok != Token::comma){
        error("expect , at line: "+std::to_string(lineN));
    }
    getNextToken(); //eat ,

    std::unique_ptr<ExprAST> cond = ParseExpr();
    
    if(CurTok != Token::comma){
        error("expect , at line: "+std::to_string(lineN));
    }
    getNextToken(); //eat ,

    std::unique_ptr<ExprAST> step =ParseExpr();
    /*if(CurTok != Token::tok_number){
        if(CurTok==Token::minus){
            getNextToken(); //eat -
            if(CurTok != Token::tok_number){
                error("unexpected number at for step at line: "+std::to_string(lineN));
            }
            std::string num = NumStr;
            getNextToken();
            if(num == "0"){
                error("-0 is invalid at line: "+std::to_string(lineN));
            }
            if(num[0]=='+')
                num[0] = '-';
            else
                num[0] = '+';
            
            step = std::make_unique<NumberExprAST>(num,lineN);
        }else{
            step = std::make_unique<NumberExprAST>("+1",lineN);
        }
    }
    else{
        step = ParseNumberExpr();
    }*/

    std::unique_ptr<CommandAST> cmds = ParseCommand();
    if(cmds->isRet()||cmds->isBreak())
        error("the command in for loop cannot be total return or break at line: "+std::to_string(lineN));

    return std::make_unique<ForAST>(std::move(start),std::move(cond),std::move(step),std::move(cmds),line1,false);
}

/// while ::= while cond cmds
std::unique_ptr<WhileAST> Parser::ParseWhile(){

    int line1 = lineN;
    if(CurTok != Token::tok_while)
        Bug("call ParseWhile, but no while",lineN);

    getNextToken();

    std::unique_ptr<ExprAST> cond = ParseExpr();

    std::unique_ptr<CommandAST> cmds = ParseCommand();

    if(cmds->isRet()||cmds->isBreak())
        error("the command in while loop cannot be total return at line: "+std::to_string(lineN));

    return std::make_unique<WhileAST>(std::move(cond),std::move(cmds),line1,cmds->isRet());
}

//break
std::unique_ptr<BreakAST> Parser::ParseBreak(){
    if(CurTok != Token::tok_break)
        Bug("call ParseBreak, but no break",lineN);

    getNextToken(); //eat break

    return std::make_unique<BreakAST>(lineN);
}

std::unique_ptr<CommandAST> Parser::ParseCommand(){

    switch(CurTok){
    case Token::tok_identifier:
        return ParseIdentifier();
        break;
    case Token::tok_return:
        return ParseReturn();
        break;
    case Token::left_brace:
        return ParseBlock();
        break;
    case Token::tok_if:
        return ParseIf();
        break;
    case Token::tok_for:
        return ParseFor();
        break;
    case Token::tok_while:
        return ParseWhile();
        break;
    case Token::tok_break:
        return ParseBreak();
        break;
    case Token::tok_free:
        return ParseFree();
        break;
    default:
        if(isType())
        {   
            return ParseVariableDef(false);
        } else
        {   
            //std::cout<<CurTok<<std::endl;
            error("unexpect command at line: "+std::to_string(lineN));
           // ErrorQ("unexpected Command", lineN);
           // return nullptr;
        }
        break;
    }
}
