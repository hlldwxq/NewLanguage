#include "Parser.h"
#include "AST.h"

//================Commands===============//

/// assign ::= leftvalue = expression
static std::unique_ptr<AssignAST> ParseAssign(std::string name){
    

    std::unique_ptr<LeftValueAST> left;
    
    if(CurTok == '['){
        left = ParseArrayIndexExpr(name);
        if(left == nullptr)
            return nullptr;
    }else{
        left = std::make_unique<VariableExprAST>(name);
    }
    
    if(CurTok != '='){
        ErrorQ("unexpected symbol in assign", lineN);
        return nullptr;
    }

    getNextToken(); //eat =

    std::unique_ptr<ExprAST> right = ParseExpression();
    if(right == nullptr){
        return nullptr;
    }

    return std::make_unique<AssignAST>(std::move(left),std::move(right));
}

/// identifier ::= varname = expression
///            ::= arrayname[expression1]+ = expression
///            ::= functionName( [expression]* , )
static std::unique_ptr<CommandAST> ParseIdentifier(){
    
    if(CurTok != tok_identifier)
        Bug("call ParseIdentifier, but no identifier");

    std::string name = IdentifierStr;
    getNextToken();  // eat identifier
    switch(CurTok){
        case '(':
        return ParseCallExpr(name);
        default:
        return ParseAssign(name);
    }
}

/// return ::= return expression
static std::unique_ptr<ReturnAST> ParseReturn(){
    
    if(CurTok != tok_return)
        Bug("call ParseReturn, but no return");

    getNextToken(); //eat return
    std::unique_ptr<ExprAST> value = ParseExpression();
    if(value==nullptr){
        return nullptr;
    }
    return std::make_unique<ReturnAST>(std::move(value));
}

/// block ::= {  cmd+  }
static std::unique_ptr<BlockAST> ParseBlock(){

    if(CurTok != '{')
        Bug("call ParseBlock, but no {");

    getNextToken(); //eat {
    std::vector<std::unique_ptr<CommandAST>> cmds;

    while(CurTok != '}' && CurTok!=EOF){
        std::unique_ptr<CommandAST> cmd = ParseCommand();
        cmds.push_back(std::move(cmd));
        if(cmd == nullptr)
            return nullptr;
    }

    if(CurTok != '}'){
        ErrorQ("expect }",lineN);
        return nullptr;
    }
    getNextToken(); //eat }

    return std::make_unique<BlockAST>(std::move(cmds));
}

/// if ::= if condition then cmds [else cmds]
static std::unique_ptr<IfAST> ParseIf(){
    
    if(CurTok != tok_if)
        Bug("call ParseIf, but no if");

    getNextToken(); //eat if
    
    std::unique_ptr<ExprAST> con = ParseExpression();
    if(con == nullptr)
        return nullptr;
    
    if(CurTok != tok_then){
        ErrorQ("except then", lineN);
        return nullptr;
    }
    
    std::unique_ptr<CommandAST> then = ParseCommand();
    if(then == nullptr)
        return nullptr;
    
    std::unique_ptr<CommandAST> elseT;
    if(CurTok != tok_else)
        elseT = nullptr;
    else {
        elseT = ParseCommand();
        if(elseT == nullptr)
            return nullptr;
    }

    return std::make_unique<IfAST>(std::move(con),std::move(then),std::move(elseT));
}

/// def ::= Type variableName [ = expression] 
///     ::= Type arrayName*+ = new Type [expression]+
static std::unique_ptr<DefAST> ParseVarOrArrDef(){
    if( !(CurTok <= tok_i1 && CurTok >= tok_i128) ){
        Bug("call ParseVarOrArrDef, but no type");
    }

    int type = CurTok;

    int ptr = 0;
    while(CurTok == '*'){
        ptr++;
        getNextToken();
    }

    if(CurTok!=tok_identifier){
        ErrorQ("except name", lineN);
        //return nullptr;
    }
    std::string name = IdentifierStr;

    if(ptr==0){
        std::unique_ptr<ExprAST> value;
        if(CurTok != '='){
            //default value
            value = std::make_unique<NumberExprAST>(0);
        }
        else{
            getNextToken(); //eat =
           
            value = ParseExpression();
            if(value==nullptr)
                return nullptr;              
        }
        return std::make_unique<VarDefAST>(type,name,std::move(value));
    }
    else{
        std::vector<std::unique_ptr<ExprAST>> indexs;
        // if the indexs is empty, codegen should init the pointer as null
        if(CurTok == '='){
            getNextToken(); //eat =
        
            if(CurTok != tok_new){
                ErrorQ("except new", lineN);
                return nullptr;
            }
            getNextToken(); //eat new

            if( !(CurTok <= tok_i1 && CurTok >= tok_i128) ){
                ErrorQ("except a type", lineN);
                return nullptr;
            }

            if(type != CurTok){
                ErrorQ("two types in a array definition need to be the same",lineN);
                return nullptr;
            }
            getNextToken(); //eat type

            for(int i=0;i<ptr;i++){
                if(CurTok!='['){
                   ErrorQ("unenough [, except [",lineN);
                   return nullptr;
                }
                getNextToken(); //eat [

                std::unique_ptr<ExprAST> index = ParseExpression();
                if(index==nullptr)
                    return nullptr;
                indexs.push_back(std::move(index));

                if(CurTok!=']'){
                    ErrorQ("except ] in array definition",lineN);
                    return nullptr;
                }
            }
        }
        return std::make_unique<ArrayDefAST>(type,name,std::move(indexs));
    }
}

/// for ::= for start, cond, step cmds
static std::unique_ptr<ForAST> ParseFor(){
    
    if(CurTok != tok_for)
        Bug("call ParseFor, but no for");

    getNextToken(); //eat for
    std::unique_ptr<DefAST> start = ParseVarOrArrDef();
    if(start == nullptr)
        return nullptr;
    
    if(CurTok != ','){
        ErrorQ("except ,",lineN);
        return nullptr;
    }
    getNextToken();

    std::unique_ptr<ExprAST> cond = ParseExpression();
    if(cond == nullptr)
        return nullptr;
    
    if(CurTok != ','){
        ErrorQ("except ,",lineN);
        return nullptr;
    }
    getNextToken();

    long long step;
    if(CurTok != tok_number)
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
    if(CurTok != tok_while)
        Bug("call ParseWhile, but no while");

    getNextToken();

    std::unique_ptr<ExprAST> cond = ParseExpression();
    if(cond == nullptr)
        return nullptr;
    std::unique_ptr<CommandAST> cmds = ParseCommand();
    if(cmds == nullptr)
        return nullptr;

    return std::make_unique<WhileAST>(std::move(cond),std::move(cmds));
}

static std::unique_ptr<CommandAST> ParseCommand(){
    switch(CurTok){
    case tok_identifier:
        return ParseIdentifier();
        break;
    case tok_return:
        return ParseReturn();
        break;
    case '{':
        return ParseBlock();
        break;
    case tok_if:
        return ParseIf();
        break;
    case tok_for:
        return ParseFor();
        break;
    case tok_while:
        return ParseWhile();
        break;
    default:
        if(CurTok<=tok_i1 && CurTok>=tok_i128)
        {   return ParseVarOrArrDef();}
        else
        {   
            ErrorQ("unexpected Command", lineN);
            return nullptr;
        }
        break;
    }
}