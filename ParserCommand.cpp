#include "Parser.h"

//================Commands===============//

/// assign ::= leftvalue = expression
std::unique_ptr<AssignAST> Parser::ParseAssign(std::string name){
    

    std::unique_ptr<LeftValueAST> left;
    
    if(CurTok == Token::left_square_bracket){
        
        std::unique_ptr<VarOrPointerAST> pointerName = std::make_unique<VarOrPointerAST>(name,true);
        left = ParseArrayIndexExpr(std::move(pointerName));
        if(left == nullptr)
            return nullptr;
    }else{
        left = std::make_unique<VarOrPointerAST>(name,false);
    }
    
    if(CurTok != Token::assignment){
        ErrorQ("unexpected symbol in assign", lineN);
        return nullptr;
    }

    getNextToken(); //eat =

    std::unique_ptr<ExprAST> right = ParseExpr();
    if(right == nullptr){
        return nullptr;
    }

    return std::make_unique<AssignAST>(std::move(left),std::move(right));
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
        case Token::left_brace:
        return ParseCallExpr(name);
        default:
        return ParseAssign(name);
    }
}

/// return ::= return expression
std::unique_ptr<ReturnAST> Parser::ParseReturn(){
    
    if(CurTok != Token::tok_return)
        Bug("call ParseReturn, but no return",lineN);

    getNextToken(); //eat return
    std::unique_ptr<ExprAST> value = ParseExpr();
    if(value==nullptr){
        return nullptr;
    }
    return std::make_unique<ReturnAST>(std::move(value));
}

/// block ::= {  cmd*  }
std::unique_ptr<BlockAST> Parser::ParseBlock(){

    if(CurTok != Token::left_brace)
        Bug("call ParseBlock, but no {",lineN);

    getNextToken(); //eat {
    std::vector<std::unique_ptr<CommandAST>> cmds;

    while(CurTok != Token::right_brace && CurTok!=Token::tok_eof){
       // ErrorQ("parse cmd,no error",lineN);
        std::unique_ptr<CommandAST> cmd = ParseCommand();
        if(cmd == nullptr)
            return nullptr;
        cmds.push_back(std::move(cmd));
    }

    if(CurTok != Token::right_brace){
        ErrorQ("expect }",lineN);
        return nullptr;
    }
    getNextToken(); //eat }

    return std::make_unique<BlockAST>(std::move(cmds));
}

/// if ::= if condition then cmds [else cmds]
std::unique_ptr<IfAST> Parser::ParseIf(){
    
    if(CurTok != Token::tok_if)
        Bug("call ParseIf, but no if",lineN);

    getNextToken(); //eat if
    
    std::unique_ptr<ExprAST> con = ParseExpr();
    if(con == nullptr)
        return nullptr;
    
    if(CurTok != Token::tok_then){
        ErrorQ("except then", lineN);
        return nullptr;
    }
    getNextToken(); //eat if

    std::unique_ptr<CommandAST> then = ParseCommand();
    if(then == nullptr)
        return nullptr;
    
    std::unique_ptr<CommandAST> elseT;
    if(CurTok != Token::tok_else)
        elseT = std::make_unique<BlockAST>();  //empty block
    else {
        getNextToken(); //eat else
        elseT = ParseCommand();
        if(elseT == nullptr)
            return nullptr;
    }

    return std::make_unique<IfAST>(std::move(con),std::move(then),std::move(elseT));
}

/// def ::= Type variableName [ = expression] 
///     ::= Type arrayName*+ = new Type [expression]+
std::unique_ptr<DefAST> Parser::ParseVarOrArrDef(bool global){
    if(!isType()){
        Bug("call ParseVarOrArrDef, but no type",lineN);
    }
    Token CurType = CurTok;
    QType* type = ParseType();

    if(CurTok!=Token::tok_identifier){
        ErrorQ("except name", lineN);
        //return nullptr;
    }
    std::string name = IdentifierStr;
    getNextToken(); //eat name

    if(!type->getIsPointer()){
        std::unique_ptr<ExprAST> value;
        if(CurTok != Token::assignment){
            //default value
            value = std::make_unique<NumberExprAST>(0);
        }
        else{
            getNextToken(); //eat =
           
            value = ParseExpr();
            if(value==nullptr)
                return nullptr;              
        }
        return std::make_unique<VarDefAST>(dynamic_cast<IntType*>(type),name,std::move(value),global);
    }else{
        std::unique_ptr<ExprAST> right;
        
        // if the indexs is empty, codegen should init the pointer as null
        if(CurTok == Token::assignment){
            getNextToken(); //eat =
        
            if(CurTok != Token::tok_new){
                ErrorQ("except new", lineN);
                return nullptr;
            }
            getNextToken(); //eat new

            if( !(isType()) ){
                ErrorQ("except a type", lineN);
                return nullptr;
            }

            if(CurType != CurTok){
                ErrorQ("two types in a array definition need to be the same",lineN);
                return nullptr;
            }
            auto newType = ParseType();
            
            if(CurTok!=Token::left_square_bracket){
                ErrorQ("unenough [, except [",lineN);
                return nullptr;
            }
            getNextToken(); //eat [

            std::unique_ptr<ExprAST> index = ParseExpr();
            if(index==nullptr)
                return nullptr;

            if(CurTok!=Token::right_square_bracket){
                ErrorQ("except ] in array definition",lineN);
                return nullptr;
            }
            getNextToken(); //eat ]
            
            right = std::make_unique<NewExprAST>(newType,std::move(index));

        }else{ // no assignment, default value is null
            right = std::make_unique<NullExprAST>();
        }
        return std::make_unique<ArrayDefAST>(dynamic_cast<PointType*>(type),name,std::move(right),global);
    }
}

/// for ::= for start, cond, step cmds
std::unique_ptr<ForAST> Parser::ParseFor(){
    
    if(CurTok != Token::tok_for)
        Bug("call ParseFor, but no for",lineN);

    getNextToken(); //eat for
   // ErrorQ("parse for, no error",lineN);
    std::unique_ptr<DefAST> start = ParseVarOrArrDef(false);
   // ErrorQ("parse for, end start no error",lineN);
    if(start == nullptr)
        return nullptr;
    
    if(CurTok != Token::comma){
        ErrorQ("except ,",lineN);
        return nullptr;
    }
    getNextToken();

    std::unique_ptr<ExprAST> cond = ParseExpr();
    if(cond == nullptr)
        return nullptr;
    
    if(CurTok != Token::comma){
        ErrorQ("except ,",lineN);
        return nullptr;
    }
    getNextToken();

    long long step;
    if(CurTok != Token::tok_number)
        step = 1;
    else{
        step = NumVal;
        getNextToken(); //eat num
    }
    std::unique_ptr<CommandAST> cmds = ParseCommand();
    if(cmds==nullptr)
        return nullptr;
    
    return std::make_unique<ForAST>(std::move(start),std::move(cond),step,std::move(cmds));
}

/// while ::= while cond cmds
std::unique_ptr<WhileAST> Parser::ParseWhile(){
    if(CurTok != Token::tok_while)
        Bug("call ParseWhile, but no while",lineN);

    getNextToken();

    std::unique_ptr<ExprAST> cond = ParseExpr();
    if(cond == nullptr)
        return nullptr;
    std::unique_ptr<CommandAST> cmds = ParseCommand();
    if(cmds == nullptr)
        return nullptr;

    return std::make_unique<WhileAST>(std::move(cond),std::move(cmds));
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
    default:
        if(isType())
        {   return ParseVarOrArrDef(false);}
        else
        {   
            ErrorQ("unexpected Command", lineN);
            return nullptr;
        }
        break;
    }
}