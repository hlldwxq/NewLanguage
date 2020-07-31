#include "../header/Parser.h"

void Bug(const char * info,int lineN){
	fprintf(stderr, "There is Bug that need to be solved: %s , at %d\n" , info,lineN);
	exit(1);
}

void Parser::initPrecedence(){
    BinopPrecedence[Token::assignment] = 2;         // =
    BinopPrecedence[Token::andT] = 5;               //&
    BinopPrecedence[Token::orT] = 5;                // |
    BinopPrecedence[Token::less_than] = 10;         // <
    BinopPrecedence[Token::greater_than] = 10;      // >
    BinopPrecedence[Token::less_equal] = 10;        // <=
    BinopPrecedence[Token::greater_equal] = 10;     // >=
    BinopPrecedence[Token::not_equal] = 10;         // ==
    BinopPrecedence[Token::equal_sign] = 10;        // ==
    BinopPrecedence[Token::plus] = 20;              // +
    BinopPrecedence[Token::minus] = 20;	            // -
    BinopPrecedence[Token::star] = 40; 	            // *
    BinopPrecedence[Token::disvision] = 40;         // /
}

int Parser::getChar(){

    if(charIndex < fileStr.size()){
        int c = fileStr[charIndex++];
        return c;
    }else{
        return EOF;
    }

}

/// gettok - Return the next token from standard input.
Token Parser::gettok(){
    
    static int LastChar = ' ';

    // Skip any whitespace.
    while (isspace(LastChar)){
        if(LastChar == '\n' || LastChar == '\r'){
            lineN++;
            int c = LastChar;
            LastChar = getChar();
            if(c == '\r' && LastChar == '\n'){
                LastChar = getChar();
            }
            
        }else{
            LastChar = getChar(); 
        }
    }

    // identifier: [a-zA-Z][a-zA-Z0-9]*
    if (isalpha(LastChar)){ //[a-zA-Z]
        IdentifierStr = LastChar;
        while (isalnum((LastChar = getChar())) || LastChar=='_') //isalnum [a-zA-Z0-9]
            IdentifierStr += LastChar;

        if (IdentifierStr == "def")
            return Token::tok_def;
        if (IdentifierStr == "void")
            return Token::tok_void;
        if (IdentifierStr == "return")
            return Token::tok_return;
        if (IdentifierStr == "if")
            return Token::tok_if;
        if (IdentifierStr == "then")
            return Token::tok_then;
        if (IdentifierStr == "else")
            return Token::tok_else;
        if (IdentifierStr == "for")
            return Token::tok_for;
        if (IdentifierStr == "while")
            return Token::tok_while;
        if (IdentifierStr == "break")
            return Token::tok_break;
        if(IdentifierStr == "new")
            return Token::tok_new;
        if(IdentifierStr == "free")
            return Token::tok_free;
        if(IdentifierStr == "null")
            return Token::tok_null;
        if(IdentifierStr == "sint1")
            return Token::tok_si1;
        if(IdentifierStr == "sint8")
            return Token::tok_si8;
        if(IdentifierStr == "sint16")
            return Token::tok_si16;
        if(IdentifierStr == "sint32")
            return Token::tok_si32;
        if(IdentifierStr == "sint64")
            return Token::tok_si64;
        if(IdentifierStr == "sint128")
            return Token::tok_si128;
        if(IdentifierStr == "uint1")
            return Token::tok_ui1;
        if(IdentifierStr == "uint8")
            return Token::tok_ui8;
        if(IdentifierStr == "uint16")
            return Token::tok_ui16;
        if(IdentifierStr == "uint32")
            return Token::tok_ui32;
        if(IdentifierStr == "uint64")
            return Token::tok_ui64;
        if(IdentifierStr == "uint128")
            return Token::tok_ui128;
        if(IdentifierStr == "string"){
            return Token::tok_stringType;
        }
        if(IdentifierStr == "true")
            return Token::tok_true;
        if(IdentifierStr == "false")
            return Token::tok_false;	
        return Token::tok_identifier;
    }

    // Number: [0-9]+
    if (isdigit(LastChar)){ 
        NumStr = "";
        do
        {
            NumStr += LastChar;
            LastChar = getChar();
        } while (isdigit(LastChar));

        if(isalpha(LastChar)){ 
            return Token::error_token;
        }

        while(NumStr[0] == '0'){
            if(NumStr.size()>1)
                NumStr.erase(0,1);
            else 
                break;
        }

        //39 is the size of the max value of the sigend num
        if(NumStr.length()>=39 && !checkRange(NumStr,false)){
            error("invalid number because it is too big or too small at line: "+std::to_string(lineN));
        }

        return Token::tok_number;
    }

    if (LastChar == '\"'){
        Str = "";
        LastChar = getChar();

        if(LastChar != '\"'){
            
            do{
                if(LastChar == EOF){
                    error("expect \" at the end of string at line: "+std::to_string(lineN));
                }
                Str += LastChar;
                LastChar = getChar();
            }while(LastChar!='\"');

            LastChar = getChar();
            
        }
        return Token::tok_string;
    }

    // symbol
    switch(LastChar){
    case '=':
        LastChar = getChar();
        if(LastChar == '='){
            LastChar = getChar();
            return Token::equal_sign;
        }else{
            return Token::assignment;
        }
        break;
    case '!':
        LastChar = getChar();
        if(LastChar == '='){
            LastChar = getChar();
            return Token::not_equal;
        }else{
            return Token::exclamation_point;
        }
        break;
    case '>':
    LastChar = getChar();
        if(LastChar == '='){
            LastChar = getChar();
            return Token::greater_equal;
        }else{
            return Token::greater_than;
        }
        break;
    case '<':
        LastChar = getChar();
        if(LastChar == '='){
            LastChar = getChar();
            return Token::less_equal;
        }else{
            return Token::less_than ;
        }
        break;
    case '+': 
        LastChar = getChar() ;   
        return Token::plus;
        break;   					//+
    case '-': 
        LastChar = getChar(); 
        return Token::minus;
        break;      			    //-
    case '*': 
        LastChar = getChar() ;
        return Token::star;
        break;      			    //*
    case '/': 
        LastChar = getChar() ;
        return Token::disvision;
        break;  				    // /
    case '(': 
        LastChar = getChar() ;
        return Token::left_paren;
        break;  					// (
    case ')': 
        LastChar = getChar() ;
        return Token::right_paren;
        break; 						// )
    case '[': 
        LastChar = getChar() ;
        return Token::left_square_bracket;
        break; 					    // [
    case ']': 
        LastChar = getChar() ;
        return Token::right_square_bracket;
        break; 						// ]
    case '{': 
        LastChar = getChar() ;
        return Token::left_brace;
        break;          		    // {
    case '}': 
        LastChar = getChar() ;
        return Token::right_brace;
        break;         			    // }
    case ',': 
        LastChar = getChar() ;
        return Token::comma;
        break;              	    // ,
    case '&': 
        LastChar = getChar() ;
        return Token::andT;
        break;                	    // &
    case '|': 
        LastChar = getChar() ;
        return Token::orT;
        break;                      // |
    case ';': 
        LastChar = getChar() ;
        return Token::semicolon;
        break;				   	    // ;
    case '#':
        // Comment until end of line.
        do
            LastChar = getChar();
        while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

        if (LastChar != EOF)
            return gettok();

        break;
    case EOF:
        return Token::tok_eof;
    default:
        return Token::error_token;
    }

    // Check for end of file.  Do not eat the EOF.
    if (LastChar == EOF)
        return Token::tok_eof;

    // other symbol return error
    return Token::error_token;
}

Token Parser::getNextToken(){   
    CurTok = gettok();
    if(CurTok == Token::error_token){
        error("unvalid character or word at line: "+std::to_string(lineN));
        //exit(1);
    } 

    return CurTok;
}

int Parser::GetTokPrecedence(){
    if(BinopPrecedence.empty()){
        initPrecedence();
    }
    // Make sure it's a declared binop.
    int TokPrec = BinopPrecedence[CurTok];

    if (TokPrec <= 0) 
        return -1;
    return TokPrec;
}

bool Parser::isType(){
    if( CurTok == Token::tok_ui1 || CurTok == Token::tok_ui8|| CurTok == Token::tok_ui16|| CurTok == Token::tok_ui32 || CurTok == Token::tok_ui64||  CurTok == Token::tok_ui128 )
        return true;
    if( CurTok == Token::tok_si1 || CurTok == Token::tok_si8|| CurTok == Token::tok_si16|| CurTok == Token::tok_si32 || CurTok == Token::tok_si64||  CurTok == Token::tok_si128 )
        return true;
    if( CurTok == Token::tok_stringType )
        return true;

    return false;
}

QType* Parser::ParseType(){
    
    if(!isType()){
        error("except a type at line: "+std::to_string(lineN));
        //return NULL;
    }

    if(CurTok == Token::tok_stringType){
        getNextToken();
        return new StringType();
    }

    unsigned long long width;
    bool isSigned;

    switch(CurTok){
        case Token::tok_si1:
        width = 1;
        isSigned = true;
        break;
        case Token::tok_si8:
        width = 8;
        isSigned = true;
        break;
        case Token::tok_si16:
        width = 16;
        isSigned = true;
        break;
        case Token::tok_si32:
        width = 32;
        isSigned = true;
        break;
        case Token::tok_si64:
        width = 64;
        isSigned = true;
        break;
        case Token::tok_si128:
        width = 128;
        isSigned = true;
        break;
        case Token::tok_ui1:
        width = 1;
        isSigned = false;
        break;
        case Token::tok_ui8:
        width = 8;
        isSigned = false;
        break;
        case Token::tok_ui16:
        width = 16;
        isSigned = false;
        break;
        case Token::tok_ui32:
        width = 32;
        isSigned = false;
        break;
        case Token::tok_ui64:
        width = 64;
        isSigned = false;
        break;
        case Token::tok_ui128:
        width = 128;
        isSigned = false;
        break;
        default:
        error("expect a type at line: "+std::to_string(lineN));
        return nullptr;
    }
    QType* type = new IntType(isSigned,width);
    
    getNextToken();
    while(CurTok==Token::star){
        type = new PointType(type);
        getNextToken();
    }

    return type;
}

void Parser::Parse(){
    getNextToken();
    while(CurTok != Token::tok_eof){
        std::unique_ptr<StructureAST> structure = ParseStructure();
        if(structure != nullptr){
            //structure->printAST();
            structure->codegenStructure();
        }else{
            exit(0);
        }
    }
    CallInitFunction();
}
