#pragma once
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include "AST.h"
#include "main.h"

enum class Token
{
	tok_eof,

	// Function
	tok_def ,
    tok_void,
    tok_return ,

	tok_identifier,
	tok_number,

	// control
	tok_if ,
	tok_then ,
	tok_else,
	tok_for ,
	tok_while ,
    tok_break,

	//array
	tok_new ,
    tok_null,

	//type
	tok_i1 ,
	tok_i8 ,
	tok_i16,
	tok_i32,
	tok_i64,
	tok_i128,

    tok_true,
    tok_false,

	//symbol
	equal_sign,  //==
	not_equal,   //!=
	less_equal,  //<=
	more_equal,  //>=

	plus_sign,   //+
	minus,       //-
	star,        //*
	disvision,   // /
	left_paren,  // (
	right_paren, // )
	left_square_bracket,  // [
	right_square_bracket, // ]
	left_brace,           // {
	right_brace,          // }
	comma,                // ,
	exclamation_point,    // !
	andT,                 // &
	orT,                  // |
	more_sign,            // >
	less_sign,            // <
	assignment,           // =
	semicolon,			  // ;
	error_token
};

class Parser {
static int lineN = 1;
static Token CurTok;

static int charIndex = 0;
static std::string IdentifierStr; // Filled in if tok_identifier
static long long NumVal;		  // Filled in if tok_number
// although -9 will be regarded as two tokens, - and 9
// I still use signed num instead of unsigned 

std::map<Token, int> BinopPrecedence;
void initPrecedence(){
    BinopPrecedence[Token::assignment] = 2;         // =
    BinopPrecedence[Token::less_sign] = 10;        // <
    BinopPrecedence[Token::more_sign] = 10;        // >
    BinopPrecedence[Token::less_equal] = 10;  // <=
    BinopPrecedence[Token::more_equal] = 10;  // >=
    BinopPrecedence[Token::not_equal] = 10;   // ==
    BinopPrecedence[Token::equal_sign] = 10;  // ==
    BinopPrecedence[Token::plus_sign] = 20;        // +
    BinopPrecedence[Token::minus] = 20;	      // -
    BinopPrecedence[Token::star] = 40; 	      // *
    BinopPrecedence[Token::disvision] = 40;        // /
}

void ErrorQ(const char * info, int line){
    fprintf(stderr, "Syntax Error: line %d -- %s\n",line, info);
   // return nullptr;
}

/// this function is reminding me there is bug, but maybe I will not get error info
/// the function need to be removed in the future
void Bug(const char * info){
	fprintf(stderr, "There is Bug that need to be solved: %s \n" , info);
	exit(1);
}

long long string2longlong(std::string x) {

	//becuse the developing enviroment is 64bit, the range of the result of the function is 0 - (2^63-1)
	long long a;
	std::string res = x;
	std::stringstream ss;
	ss << res;
	ss >> a;
	return a;
}

int GetTokPrecedence()
{
	if(BinopPrecedence.empty()){
		initPrecedence();
	}
	// Make sure it's a declared binop.
	int TokPrec = BinopPrecedence[CurTok];

	if (TokPrec <= 0) 
		return -1;
	return TokPrec;
}

int getChar(){

    if(charIndex < fileStr.size()){
        int c = fileStr[charIndex++];
        return c;
    }else{
        return EOF;
    }

}

/// gettok - Return the next token from standard input.
static Token gettok()
{
	
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
		return Token::plus_sign;
		break;   					//+
	case '-': 
		return Token::minus;
		break;      			    //-
	case '*': 
		return Token::star;
		break;      			    //*
	case '/': 
		return Token::disvision;
		break;  				    // /
	case '(': 
		return Token::left_paren;
		break;  					// (
	case ')': 
		return Token::right_paren;
		break; 						// )
	case '[': 
		return Token::left_square_bracket;
		break; 					    // [
	case ']': 
		return Token::right_square_bracket;
		break; 						// ]
	case '{': 
		return Token::left_brace;
		break;          		    // {
	case '}': 
		return Token::right_brace;
		break;         			    // }
	case ',': 
		return Token::comma;
		break;              	    // ,
	case '&': 
		return Token::andT;
		break;                	    // &
	case '|': 
		return Token::orT;
		break;                      // |
	case ';': 
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
	default:
		return Token::error_token;
	}

	// Check for end of file.  Do not eat the EOF.
	if (LastChar == EOF)
		return Token::tok_eof;

    // other symbol return error
	return Token::error_token;
}

Token getNextToken() {   
    CurTok = gettok();
	if(CurTok == Token::error_token){
		ErrorQ("unvalid character or word",lineN);
		exit(1);
	} 
	return CurTok;
}
};

//other functions do need to be used in other files
static std::unique_ptr<ExprAST> ParseExpr();
static std::unique_ptr<CallExprAST> ParseCallExpr(std::string functionName);
static std::unique_ptr<ArrayIndexExprAST> ParseArrayIndexExpr(std::string arrayName);
static std::unique_ptr<CommandAST> ParseCommand();
static std::unique_ptr<DefAST> ParseVarOrArrDef(bool global);
static std::unique_ptr<BlockAST> ParseBlock();