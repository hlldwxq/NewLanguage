#include "Parser.h"

void Bug(const char * info,int lineN){
	fprintf(stderr, "There is Bug that need to be solved: %s , at %d\n" , info,lineN);
	exit(1);
}

void Parser::initPrecedence(){
    BinopPrecedence[Token::assignment] = 2;         // =
    BinopPrecedence[Token::andT] = 5;               //&
    BinopPrecedence[Token::orT] = 5;                // |
    BinopPrecedence[Token::less_sign] = 10;        // <
    BinopPrecedence[Token::more_sign] = 10;        // >
    BinopPrecedence[Token::less_equal] = 10;       // <=
    BinopPrecedence[Token::more_equal] = 10;       // >=
    BinopPrecedence[Token::not_equal] = 10;         // ==
    BinopPrecedence[Token::equal_sign] = 10;        // ==
    BinopPrecedence[Token::plus_sign] = 20;        // +
    BinopPrecedence[Token::minus] = 20;	            // -
    BinopPrecedence[Token::star] = 40; 	            // *
    BinopPrecedence[Token::disvision] = 40;        // /
}

long long Parser::string2longlong(std::string x) {

    //becuse the developing enviroment is 64bit
    //the range of the result of the function is 0 - (2^63-1)
    long long a;
    std::string res = x;
    std::stringstream ss;
    ss << res;
    ss >> a;
    return a;
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
        while (isalnum((LastChar = getChar()))) //isalnum [a-zA-Z0-9]
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
        if(IdentifierStr == "null")
            return Token::tok_null;
        if(IdentifierStr == "int1")
            return Token::tok_i1;
        if(IdentifierStr == "int8")
            return Token::tok_i8;
        if(IdentifierStr == "int16")
            return Token::tok_i16;
        if(IdentifierStr == "int32")
            return Token::tok_i32;
        if(IdentifierStr == "int64")
            return Token::tok_i64;
        if(IdentifierStr == "int128")
            return Token::tok_i128;
        if(IdentifierStr == "true")
            return Token::tok_true;
        if(IdentifierStr == "false")
            return Token::tok_false;	
        return Token::tok_identifier;
    }

    // Number: [0-9]+
    if (isdigit(LastChar)){ 
        std::string NumStr;
        do
        {
            NumStr += LastChar;
            LastChar = getChar();
        } while (isdigit(LastChar));

        if(isalpha(LastChar)){ // 5657t is unvalid
            return Token::error_token;
        }

        //cover string to number
        NumVal = string2longlong(NumStr);
        //if num is out of the range of target architecture, the result will be the maxnum

        return Token::tok_number;
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
            return Token::more_equal;
        }else{
            return Token::more_sign ;
        }
        break;
    case '<':
        LastChar = getChar();
        if(LastChar == '='){
            LastChar = getChar();
            return Token::less_equal;
        }else{
            return Token::less_sign ;
        }
        break;
    case '+': 
        LastChar = getChar() ;   
        return Token::plus_sign;
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
        ErrorQ("unvalid character or word",lineN);
        exit(1);
    } 
    // printf( "Now Token: line %d -- %d\n",lineN, static_cast<int>(CurTok));

    return CurTok;
}

void Parser::ErrorQ(const char * info, int line){
    fprintf(stderr, "Syntax Error: line %d -- %s\n",line, info);
// return nullptr;
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
    if( CurTok == Token::tok_i1 || CurTok == Token::tok_i8|| CurTok == Token::tok_i16|| CurTok == Token::tok_i32 || CurTok == Token::tok_i64||  CurTok == Token::tok_i128 )
        return true;
    return false;
}

void Parser::Parse(){
    getNextToken();
    while(CurTok != Token::tok_eof){
        std::unique_ptr<StructureAST> structure = ParseStructure();
        if(structure != nullptr){
            structure->printAST(0);
        }else{
            exit(1);
        }
    }
}