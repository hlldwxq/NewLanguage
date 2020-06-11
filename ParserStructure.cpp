#include "Parser.h"

/// prototype::=def type functionName( arg* , ) ;
std::unique_ptr<StructureAST> Parser::ParseProtoOrFunction(){
    int line1 = lineN;
    
    if(CurTok != Token::tok_def){
        ErrorQ("except def",lineN);
        return nullptr;
    }

    getNextToken();  //eat def
    ReturnType* returnType = NULL;
    std::string FnName;
    std::vector<std::pair<std::string,QType*>> args;
    
    //get return Type
    if(CurTok == Token::tok_void){
        returnType = new ReturnType(true);
        //ErrorD("return Type void",0);
        getNextToken();
    }else{ 
        //ErrorD("not return Type void",0);
        returnType = new ReturnType(ParseType());
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
        QType* t = ParseType();
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
    std::unique_ptr<PrototypeAST> proto = std::make_unique<PrototypeAST>(FnName,args,returnType,line1);

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

    std::unique_ptr<FunctionAST> f = std::make_unique<FunctionAST>(std::move(proto),std::move(body),line1);
    return std::move(f);
}

std::unique_ptr<StructureAST> Parser::ParseStructure(){
    if(CurTok == Token::tok_def){
        return ParseProtoOrFunction();
    
    }else if(isType()){
        return ParseVariableDef(true);
    }
    ErrorQ("unexpected word",lineN);
    return nullptr;
}
