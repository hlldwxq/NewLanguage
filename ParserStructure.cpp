#include "Parser.h"
#include "main.h"
VarAndPointType ParseType(){
    if(CurTok > Token::tok_i128 || CurTok < Token::tok_i1 ){
        ErrorQ("except a type",lineN);
        return VarAndPointType(TypeType::errorType);
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
            Bug("switch type error for global var and array");
    }
    VarAndPointType type = IntType(vt);
    getNextToken();
    while(CurTok==Token::star){
        type = PointType(type);
        getNextToken();
    }

    return type;
}

/// prototype::=def type functionName( arg* , ) ;
static std::unique_ptr<StructureAST> ParseProtoOrFunction(){
    if(CurTok != Token::tok_def){
        ErrorQ("except def",lineN);
        return nullptr;
    }

    getNextToken();  //eat def
    ReturnType returnType = ReturnType(TypeType::errorType);
    std::string FnName;
    std::map<std::string,VarAndPointType*> args;
    
    //get return Type
    if(CurTok == Token::tok_void){
        returnType = voidType();
        getNextToken();
    }else{ 
        returnType = ParseType();
    }
    if(returnType.getType()==TypeType::errorType){
        return nullptr;
    }

    if(CurTok != Token::tok_identifier){
        ErrorQ("except the function name",lineN);
        return nullptr;
    }
    FnName = IdentifierStr;
    getNextToken();

    if(CurTok != Token::left_paren){
        ErrorQ("except ( in function definition", lineN);
        return nullptr;
    }
    while(true){
        if(CurTok == Token::right_paren)
            break;
        
        VarAndPointType t = ParseType();
        if(t.getType()==TypeType::errorType){
            return nullptr; 
        }

        std::string argName;
        if(CurTok != Token::tok_identifier){
           ErrorQ("except arguement name", lineN); 
        }
        argName = IdentifierStr;
        getNextToken();
        if(args.find(argName) == args.end()){
            ErrorQ("Duplicate parameter names", lineN);
            return nullptr;
        }
        args[argName] = &t;

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

    return std::make_unique<FunctionAST>(std::move(proto),std::move(body));
}

static std::unique_ptr<StructureAST> ParseStructure(){
    if(CurTok == Token::tok_def){
        return ParseProtoOrFunction();
    }else{
        return ParseVarOrArrDef(true);
    }
}

static void HandleTopLevel(){
    while(CurTok != Token::tok_eof){
        std::unique_ptr<StructureAST> structure = ParseStructure();
        if(structure != nullptr){
            structure->structureCodegen();
        }
    }
}