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


enum Token
{
	tok_eof = -1,

	// Function
	tok_def = -2,
    tok_void = -3,
    tok_return = -4,

	tok_identifier = -5,
	tok_number = -6,

	// control
	tok_if = -7,
	tok_then = -8,
	tok_else = -9,
	tok_for = -10,
	tok_while = -11,
    tok_break = -12,

	//array
	tok_new = -13,
    tok_null = -14,

	//type
	tok_i1 = -15,
	tok_i8 = -16,
	tok_i16 = -17,
	tok_i32 = -18,
	tok_i64 = -19,
	tok_i128 = -20,

    tok_true = -21,
    tok_false = -22,

	//symbol
	equalSign = -23,  //==
	notEqual = -24,   //!=
	lessEqual = -25,  //<=
	moreEqual = -26,  //>=
// other symbol  ASCII : + - * / ( ) [ ] { } , ! & | > < =
};

static std::string fileStr;
static int lineN = 1;
static int CurTok;

static int charIndex = 0;
static std::string IdentifierStr; // Filled in if tok_identifier
static long long NumVal;		   // Filled in if tok_number
// although -9 will be regarded as two tokens, - and 9
// I still use signed num instead of unsigned 

std::map<int, int> BinopPrecedence;
void initPrecedence(){
    BinopPrecedence['='] = 2;         // =
    BinopPrecedence['<'] = 10;        // <
    BinopPrecedence['>'] = 10;        // >
    BinopPrecedence[lessEqual] = 10;  // <=
    BinopPrecedence[moreEqual] = 10;  // >=
    BinopPrecedence[notEqual] = 10;   // ==
    BinopPrecedence[equalSign] = 10;  // ==
    BinopPrecedence['+'] = 20;        // +
    BinopPrecedence['-'] = 20;	      // -
    BinopPrecedence['*'] = 40; 	      // *
    BinopPrecedence['/'] = 40;        // /
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
static int gettok()
{
	
	static int LastChar = ' ';

	// Skip any whitespace.
	while (isspace(LastChar)){
        if(LastChar == '\n' || LastChar == '\r' || LastChar == '\r\n'){
            lineN++;
        }
		LastChar = getChar(); 
    }

	// identifier: [a-zA-Z][a-zA-Z0-9]*
	if (isalpha(LastChar)){ //[a-zA-Z]
		IdentifierStr = LastChar;
		while (isalnum((LastChar = getChar()))) //isalnum [a-zA-Z0-9]
			IdentifierStr += LastChar;

		if (IdentifierStr == "def")
			return tok_def;
        if (IdentifierStr == "void")
			return tok_void;
        if (IdentifierStr == "return")
			return tok_return;
		if (IdentifierStr == "if")
			return tok_if;
		if (IdentifierStr == "then")
			return tok_then;
		if (IdentifierStr == "else")
			return tok_else;
		if (IdentifierStr == "for")
			return tok_for;
		if (IdentifierStr == "while")
			return tok_while;
        if (IdentifierStr == "break")
			return tok_break;
		if(IdentifierStr == "new")
			return tok_new;
        if(IdentifierStr == "null")
            return tok_null;
		if(IdentifierStr == "int1")
			return tok_i1;
		if(IdentifierStr == "int8")
			return tok_i8;
		if(IdentifierStr == "int16")
			return tok_i16;
		if(IdentifierStr == "int32")
			return tok_i32;
		if(IdentifierStr == "int64")
			return tok_i64;
		if(IdentifierStr == "int128")
			return tok_i128;
		if(IdentifierStr == "true")
			return tok_true;
		if(IdentifierStr == "false")
			return tok_false;	
		return tok_identifier;
	}

    // Number: [0-9.]+
	if (isdigit(LastChar) || LastChar == '.'){ 
		std::string NumStr;
		do
		{
			NumStr += LastChar;
			LastChar = getChar();
		} while (isdigit(LastChar));

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
			return equalSign;
		}else{
			return '=';
		}
		break;
	case '!':
		LastChar = getChar();
		if(LastChar == '='){
			LastChar = getChar();
			return notEqual;
		}else{
			return '!';
		}
		break;
	case '>':
	LastChar = getChar();
		if(LastChar == '='){
			LastChar = getChar();
			return moreEqual;
		}else{
			return '>';
		}
		break;
	case '<':
		LastChar = getChar();
		if(LastChar == '='){
			LastChar = getChar();
			return lessEqual;
		}else{
			return '<';
		}
		break;
	}

	if (LastChar == '#')
	{
		// Comment until end of line.
		do
			LastChar = getChar();
		while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

		if (LastChar != EOF)
			return gettok();
	}

	// Check for end of file.  Do not eat the EOF.
	if (LastChar == EOF)
		return tok_eof;

    // other symbol return their ascii code
	int ThisChar = LastChar;
	LastChar = getChar();
	return ThisChar;
}

int getNextToken() {   
    return CurTok = gettok(); 
}

//other functions do need to be used in other files
static std::unique_ptr<ExprAST> ParseExpression();
static std::unique_ptr<CallExprAST> ParseCallExpr(std::string functionName);
static std::unique_ptr<ArrayIndexExprAST> ParseArrayIndexExpr(std::string arrayName);
static std::unique_ptr<CommandAST> ParseCommand();