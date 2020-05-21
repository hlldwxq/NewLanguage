#include "Parser.h"

VarAndPointType* Parser::ParseType(){
    
    if(!isType()){
        fprintf(stderr, "Syntax Error1: line %d -- %d\n",lineN, static_cast<int>(CurTok));
        ErrorQ("except a type",lineN);
        return NULL;
    }
    VarType vt;
    switch(CurTok){
        case Token::tok_i1:
            vt = VarType::int1;
            break;
        case Token::tok_i8:
            vt = VarType::int8;
            break;
        case Token::tok_i16:
            vt = VarType::int16;
            break;
        case Token::tok_i32:
            vt = VarType::int32;
            break;
        case Token::tok_i64:
            vt = VarType::int64;
            break;
        case Token::tok_i128:
            vt = VarType::int128;
            break;
        default:
            Bug("switch type error for global var and array",lineN);
            break;
    }
    VarAndPointType* type = new IntType(vt);
    getNextToken();
    while(CurTok==Token::star){
        type = new PointType(type);
        getNextToken();
    }

    return type;
}

/// prototype::=def type functionName( arg* , ) ;
std::unique_ptr<StructureAST> Parser::ParseProtoOrFunction(){
    if(CurTok != Token::tok_def){
        ErrorQ("except def",lineN);
        return nullptr;
    }

    getNextToken();  //eat def
    ReturnType* returnType = NULL;
    std::string FnName;
    std::vector<std::pair<std::string,VarAndPointType*>> args;
    
    //get return Type
    if(CurTok == Token::tok_void){
        returnType = new voidType();
        getNextToken();
    }else{ 
        returnType = ParseType();
    }
    if(returnType==NULL){
        return nullptr;
    }

    if(CurTok != Token::tok_identifier){
        ErrorQ("except the function name",lineN);
        return nullptr;
    }
    FnName = IdentifierStr;
    getNextToken(); //eat function name

    if(CurTok != Token::left_paren){
        ErrorQ("except ( in function definition", lineN);
        return nullptr;
    }
    getNextToken(); //eat (

    while(true){
        if(CurTok == Token::right_paren)
            break;
        
        if(!isType()){
            ErrorQ("expected a type for a argument",lineN);
            return nullptr;
        }
        VarAndPointType* t = ParseType();
        if(t==NULL){
            return nullptr; 
        }

        std::string argName;
        if(CurTok != Token::tok_identifier){
           ErrorQ("except arguement name", lineN); 
        }
        argName = IdentifierStr;
        getNextToken();
        for(int i=0;i<args.size();i++){
            if(args[i].first ==argName){
                ErrorQ("Duplicate parameter names", lineN);
                return nullptr;
            }
        }
        args.push_back(std::make_pair(argName,t));

        if(CurTok==Token::comma){
            getNextToken();
        }else{
            if(CurTok!=Token::right_paren){
                ErrorQ("expect )",lineN);
            }else{
                break;
            }
        }

    }
    getNextToken(); //eat )
    std::unique_ptr<PrototypeAST> proto = std::make_unique<PrototypeAST>(FnName,args,returnType);
    
    if(CurTok == Token::semicolon){
        getNextToken();
        return std::move(proto);
    }else if(CurTok != Token::left_brace){
        ErrorQ("expect ; or {",lineN);
        return nullptr;
    }

    std::unique_ptr<BlockAST> body = ParseBlock();
    if(body == nullptr){
        return nullptr;
    }

    std::unique_ptr<FunctionAST> f = std::make_unique<FunctionAST>(std::move(proto),std::move(body));
    return std::move(f);
}

std::unique_ptr<StructureAST> Parser::ParseStructure(){
    if(CurTok == Token::tok_def){
        return ParseProtoOrFunction();
    
    }else if(isType()){
        return ParseVarOrArrDef(true);
    }
    ErrorQ("unexpected word",lineN);
    return nullptr;
}
