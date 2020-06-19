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

/// this function is reminding me there is bug, but maybe I will not get error info
/// the function need to be removed in the future

void Bug(const char * info,int lineN);
void ErrorQ(const char * info, int line);
void ErrorD(const char* info, int line);
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
	tok_si1 ,
	tok_si8 ,
	tok_si16,
	tok_si32,
	tok_si64,
	tok_si128,

	tok_ui1 ,
	tok_ui8 ,
	tok_ui16,
	tok_ui32,
	tok_ui64,
	tok_ui128,

    tok_true,
    tok_false,

	//symbol
	equal_sign,  //==
	not_equal,   //!=
	less_equal,  //<=
	greater_equal,  //>=
	andT,                 // &
	orT,                  // |
	greater_than,            // >
	less_than,            // <
	plus,   //+
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
	assignment,           // =
	semicolon,			  // ;
	error_token
};


class Parser {
private:
	static int returnNum; 
	std::string fileName;
	std::string fileStr;
	int lineN = 1;
	int charIndex = 0;
	Token CurTok;
	std::string IdentifierStr; // Filled in if tok_identifier
	long long NumVal;		   // Filled in if tok_number
							   // although -9 will be regarded as two tokens, - and 9
							   // I still use signed num instead of unsigned 
	
	unsigned long retNumForCurrfunc = 0;
	std::map<Token, int> BinopPrecedence;

	void initPrecedence();
	long long string2longlong(std::string x) ;
	int getChar();
	Token gettok();  		   // gettok - Return the next token from standard input.
	
	int GetTokPrecedence();
	bool isType();
	QType* ParseType();
	
	//========Expression========================//
	std::unique_ptr<NumberExprAST> ParseBoolConstantExpr();
	std::unique_ptr<NullExprAST> ParseNullExpr();
	std::unique_ptr<NumberExprAST> ParseNumberExpr();
	std::unique_ptr<CallExprAST> ParseCallExpr(std::string functionName);
	std::unique_ptr<ArrayIndexExprAST> ParseArrayIndexExpr(std::unique_ptr<ExprAST> left);
	std::unique_ptr<ExprAST> ParseIdentifierExpr();
	std::unique_ptr<ExprAST> ParseUnary();
	std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec, std::unique_ptr<ExprAST> LHS);
	std::unique_ptr<NewExprAST> ParseNewExpr();
	std::unique_ptr<ExprAST> ParseParen();
	std::unique_ptr<ExprAST> ParseExpr();
	std::unique_ptr<ExprAST> ParseBinaryExpr();
	//===========Command========================//
	std::unique_ptr<AssignAST> ParseAssign(std::string name);
	std::unique_ptr<CommandAST> ParseIdentifier();
	std::unique_ptr<ReturnAST> ParseReturn();
	std::unique_ptr<BlockAST> ParseBlock();
	std::unique_ptr<IfAST> ParseIf();
	std::unique_ptr<DefAST> ParseVariableDef(bool global);
	std::unique_ptr<ForAST> ParseFor();
	std::unique_ptr<WhileAST> ParseWhile();
	std::unique_ptr<CommandAST> ParseCommand();

	//==========structure========================//
	std::unique_ptr<StructureAST> ParseProtoOrFunction();
	std::unique_ptr<StructureAST> ParseStructure();
public:
	
	Parser(std::string fileN , const std::string &file){
		fileStr = file;
		fileName = fileN; 
		initPrecedence();
		initModule(fileN);
	}

	Token getNextToken();
	void Parse();
	static void addReturnNum(){
		returnNum++;
	}
	static int getReturnNum(){
		return returnNum;
	}
	static void setRetNumZero(){
		returnNum = 0;
	}
};
