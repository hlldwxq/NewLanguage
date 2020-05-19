#include "Parser.h"

//================Commands===============//

/// assign ::= leftvalue = expression
static std::unique_ptr<AssignAST> ParseAssign(std::string name){
    

    std::unique_ptr<LeftValueAST> left;
    
    if(CurTok == Token::left_square_bracket){
        left = ParseArrayIndexExpr(name);
        if(left == nullptr)
            return nullptr;
    }else{
        left = std::make_unique<VariableExprAST>(name);
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
static std::unique_ptr<CommandAST> ParseIdentifier(){
    
    if(CurTok != Token::tok_identifier)
        Bug("call ParseIdentifier, but no identifier");

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
static std::unique_ptr<ReturnAST> ParseReturn(){
    
    if(CurTok != Token::tok_return)
        Bug("call ParseReturn, but no return");

    getNextToken(); //eat return
    std::unique_ptr<ExprAST> value = ParseExpr();
    if(value==nullptr){
        return nullptr;
    }
    return std::make_unique<ReturnAST>(std::move(value));
}

/// block ::= {  cmd*  }
static std::unique_ptr<BlockAST> ParseBlock(){

    if(CurTok != Token::left_brace)
        Bug("call ParseBlock, but no {");

    getNextToken(); //eat {
    std::vector<std::unique_ptr<CommandAST>> cmds;

    while(CurTok != Token::right_brace && CurTok!=Token::tok_eof){
        std::unique_ptr<CommandAST> cmd = ParseCommand();
        cmds.push_back(std::move(cmd));
        if(cmd == nullptr)
            return nullptr;
    }

    if(CurTok != Token::right_brace){
        ErrorQ("expect }",lineN);
        return nullptr;
    }
    getNextToken(); //eat }

    return std::make_unique<BlockAST>(std::move(cmds));
}

/// if ::= if condition then cmds [else cmds]
static std::unique_ptr<IfAST> ParseIf(){
    
    if(CurTok != Token::tok_if)
        Bug("call ParseIf, but no if");

    getNextToken(); //eat if
    
    std::unique_ptr<ExprAST> con = ParseExpr();
    if(con == nullptr)
        return nullptr;
    
    if(CurTok != Token::tok_then){
        ErrorQ("except then", lineN);
        return nullptr;
    }
    
    std::unique_ptr<CommandAST> then = ParseCommand();
    if(then == nullptr)
        return nullptr;
    
    std::unique_ptr<CommandAST> elseT;
    if(CurTok != Token::tok_else)
        elseT = std::make_unique<BlockAST>();  //empty block
    else {
        elseT = ParseCommand();
        if(elseT == nullptr)
            return nullptr;
    }

    return std::make_unique<IfAST>(std::move(con),std::move(then),std::move(elseT));
}

/// def ::= Type variableName [ = expression] 
///     ::= Type arrayName*+ = new Type [expression]+
static std::unique_ptr<DefAST> ParseVarOrArrDef(bool global){
    if( !(CurTok >= Token::tok_i1 && CurTok <= Token::tok_i128) ){
        Bug("call ParseVarOrArrDef, but no type");
    }
    Token CurType = CurTok;
    VarType type;
    switch(CurType){
        case Token::tok_i1:
        type = VarType::int1;
        break;
        case Token::tok_i8:
        type = VarType::int8;
        break;
        case Token::tok_i16:
        type = VarType::int16;
        break;
        case Token::tok_i32:
        type = VarType::int32;
        break;
        case Token::tok_i64:
        type = VarType::int64;
        break;
        case Token::tok_i128:
        type = VarType::int128;
        break;
        default:
        ErrorQ("expect a type",lineN);
        return nullptr;
    }

    int ptr = 0;
    while(CurTok == Token::star){
        ptr++;
        getNextToken();
    }

    if(CurTok!=Token::tok_identifier){
        ErrorQ("except name", lineN);
        //return nullptr;
    }
    std::string name = IdentifierStr;

    if(ptr==0){
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
        IntType i = IntType(type);
        return std::make_unique<VarDefAST>(i,name,std::move(value),global);
    }
    else{
        IntType i = IntType(type);
        PointType pt = PointType(i);
        for(int i=1;i<ptr;i++){
            pt = PointType(pt);
        }
        std::unique_ptr<ExprAST> left;
        
        // if the indexs is empty, codegen should init the pointer as null
        if(CurTok == Token::assignment){
            getNextToken(); //eat =
        
            if(CurTok != Token::tok_new){
                ErrorQ("except new", lineN);
                return nullptr;
            }
            getNextToken(); //eat new

            if( !(CurTok >= Token::tok_i1 && CurTok <= Token::tok_i128) ){
                ErrorQ("except a type", lineN);
                return nullptr;
            }

            if(CurType != CurTok){
                ErrorQ("two types in a array definition need to be the same",lineN);
                return nullptr;
            }
            getNextToken(); //eat type

            std::vector<std::unique_ptr<ExprAST>> indexs;
            for(int i=0;i<ptr;i++){
                if(CurTok!=Token::left_square_bracket){
                   ErrorQ("unenough [, except [",lineN);
                   return nullptr;
                }
                getNextToken(); //eat [

                std::unique_ptr<ExprAST> index = ParseExpr();
                if(index==nullptr)
                    return nullptr;
                indexs.push_back(std::move(index));

                if(CurTok!=Token::right_square_bracket){
                    ErrorQ("except ] in array definition",lineN);
                    return nullptr;
                }
            }
            left = std::make_unique<NewExprAST>(type,std::move(indexs));
        }else{ // no assignment, default value is null
            left = std::make_unique<NullExprAST>();
        }
        return std::make_unique<ArrayDefAST>(pt,name,std::move(left),global);
    }
}

/// for ::= for start, cond, step cmds
static std::unique_ptr<ForAST> ParseFor(){
    
    if(CurTok != Token::tok_for)
        Bug("call ParseFor, but no for");

    getNextToken(); //eat for
    std::unique_ptr<DefAST> start = ParseVarOrArrDef(false);
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
    else
        step = NumVal;
    
    std::unique_ptr<CommandAST> cmds = ParseCommand();
    if(cmds==nullptr)
        return nullptr;
    
    return std::make_unique<ForAST>(std::move(start),std::move(cond),step,std::move(cmds));
}

/// while ::= while cond cmds
static std::unique_ptr<WhileAST> ParseWhile(){
    if(CurTok != Token::tok_while)
        Bug("call ParseWhile, but no while");

    getNextToken();

    std::unique_ptr<ExprAST> cond = ParseExpr();
    if(cond == nullptr)
        return nullptr;
    std::unique_ptr<CommandAST> cmds = ParseCommand();
    if(cmds == nullptr)
        return nullptr;

    return std::make_unique<WhileAST>(std::move(cond),std::move(cmds));
}

static std::unique_ptr<CommandAST> ParseCommand(){
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
        if(CurTok>=Token::tok_i1 && CurTok<=Token::tok_i128)
        {   return ParseVarOrArrDef(false);}
        else
        {   
            ErrorQ("unexpected Command", lineN);
            return nullptr;
        }
        break;
    }
}