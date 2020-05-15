#include "Parser.h"

///==========Expression========================//

static std::unique_ptr<ExprAST> ParseExpr();

/// boolExpr ::= true   false
static std::unique_ptr<NumberExprAST> ParseBoolExpr(){
    
    if(CurTok != tok_true && CurTok != tok_false)
        Bug("call ParseBoolExpr, except true or false");
    
    int b = (CurTok == tok_true) ? 1 : 0;
    
    std::unique_ptr<NumberExprAST> num = std::make_unique<NumberExprAST>(b);
    getNextToken(); // eat the bool
    return num;
}

/// boolExpr ::= null
static std::unique_ptr<NullExprAST> ParseNullExpr(){

    if(CurTok != Token::tok_null)
        Bug("call ParseNullExpr, except null");

    getNextToken(); //eat null
    return std::make_unique<NullExprAST>();
}

/// numberExpr ::= number
static std::unique_ptr<ExprAST> ParseNumberExpr(){
	
    if(CurTok != Token::tok_number)
        Bug("call ParseNumberExpr, except constant number");

    std::unique_ptr<NumberExprAST> num = std::make_unique<NumberExprAST>(NumVal); 
	getNextToken();	// eat the number						   
	return std::move(num);
}

/// callExpr ::= fName(expression1,expression2, ..)
static std::unique_ptr<CallExprAST> ParseCallExpr(std::string functionName){
    
    if(CurTok != '(')
        Bug("call ParseCallExpr, except ( after the function name");

    getNextToken();  // eat (

    std::vector<std::unique_ptr<ExprAST>> args;
    while(CurTok != ')'){

        std::unique_ptr<ExprAST> arg = ParseExpression();
        if(arg == nullptr){
            return nullptr;  // we can believe that the ErrorQ info has been given
        }

        args.push_back(std::move(arg));

        if(CurTok != ','){
            if(CurTok != ')'){
                ErrorQ("expect ) in function call", lineN);
                return nullptr;
            }
        }else{
            getNextToken();  //eat ,
        }
    }

    return std::make_unique<CallExprAST>(functionName, std::move(args));
}

/// arrayIndexExpr ::= arrayName[expression1][expression2]
static std::unique_ptr<ArrayIndexExprAST> ParseArrayIndexExpr(std::string arrayName){

    if(CurTok != '[')
        Bug("call ParseArrayIndexExpr, except [ after array name");

    std::vector<std::unique_ptr<ExprAST>> indexs;
    do{
        getNextToken(); // eat [

        std::unique_ptr<ExprAST> index = ParseExpression();
        if(index == nullptr){
            return nullptr;  // we can believe that the ErrorQ log has been given 
        }
        
        indexs.push_back(std::move(index));

        if(CurTok != ']'){
            ErrorQ("except ] in array index", lineN);
            return nullptr;
        }
        getNextToken(); //eat ]

    }while(CurTok != '[');

    return std::make_unique<ArrayIndexExprAST>(arrayName, std::move(indexs));
}

/// identifierexpr
///   ::= variableName
///   ::= arrayName[expression][expression]
///   ::= identifier '(' expression* ')'
static std::unique_ptr<ExprAST> ParseIdentifierExpr()
{
	if(CurTok != Token::tok_identifier)
        Bug("call ParseIdentifierExpr, except an identifier");

	std::string IdName = IdentifierStr;
	getNextToken(); // eat identifier.
	
	if (CurTok == '(')
	{
        return ParseCallExpr(IdName);
	}
	else if (CurTok == '[')
	{
        return ParseArrayIndexExpr(IdName);
	}

    return std::make_unique<VariableExprAST>(IdName);
}

/// unary
///   ::= ! expression
///   ::= - expression
static std::unique_ptr<ExprAST> ParseUnary(){

    int Op = CurTok;

    if(Op != '!' && Op != '-'){
        Bug("call ParseUnary, but unvalid unary symbol");        
    }
    
	getNextToken();  //eat op
    
    if(Op == '-' && CurTok == tok_number){
        // -8 is regarded as two token: - and constant number 8
        // and -8 should be regarded as constant number, no Unary
        long long number = -NumVal;
        getNextToken();  //eat number
        return std::make_unique<NumberExprAST>(number);
    }

    if(Op == '-' || Op == '!'){
        std::unique_ptr<ExprAST> Operand = ParseExpr();
        if(Operand == nullptr)
            return nullptr;  //do not need to give an ErrorQ info again
        return std::make_unique<UnaryExprAST>(Op, std::move(Operand));
    }

    return nullptr;
}

/// binoprhs
///   ::= ('+' unary)*
static std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec, std::unique_ptr<ExprAST> LHS)
{
	
	while (true)
	{
		int TokPrec = GetTokPrecedence();
		
		if (TokPrec < ExprPrec)
			return LHS;
        // in normal case, next TokPrec is bigger than or equals to the current prec
        // if TokPrec< ExprPrec, it means that there is no more BinOp

		int BinOp = CurTok;
		getNextToken(); // eat binop

		// Parse the right expression after the binary operator.
		std::unique_ptr<ExprAST> RHS = ParseExpr();
		if (RHS == nullptr)
			return nullptr;

		// If BinOp binds less tightly with RHS than the operator after RHS, let
		// the pending operator take RHS as its LHS.
		int NextPrec = GetTokPrecedence();
		//printint(NextPrec);
		if (TokPrec < NextPrec)
		{
			RHS = ParseBinOpRHS(TokPrec + 1, std::move(RHS));
			if (!RHS)
				return nullptr;
		}

		// Merge LHS/RHS.
		LHS = std::make_unique<BinaryExprAST>(BinOp, std::move(LHS), std::move(RHS));
	}
}

/// paren
///    ::= (expression)
static std::unique_ptr<ExprAST> ParseParen(){
    
    if(CurTok != '(')
        Bug("call ParseParen, but no (");
    getNextToken(); //eat (

    std::unique_ptr<ExprAST> Expr = ParseExpr();
    Expr->hasParen(); //modifiy the paren flag

    if(CurTok!=')'){
        ErrorQ("expect )",lineN);
        return nullptr;
    }
    getNextToken(); // eat )

    return Expr;
}

/// Expr ::= number
///      ::= true false
///      ::= null
///      ::= identifier ...  //variableName, array index, function call
///      ::= ! - 
///      ::= Expr binop Expr
static std::unique_ptr<ExprAST> ParseExpr(){
    
    std::unique_ptr<ExprAST> Expr;
    switch(CurTok){
    case tok_number:
        Expr = ParseNumberExpr();
        break;
    case tok_true:
        Expr = ParseBoolExpr();
        break;
    case tok_false:
        Expr = ParseBoolExpr();
        break;
    case tok_null:
        Expr = ParseNullExpr();
        break;
    case tok_identifier:
        Expr = ParseIdentifierExpr();
        break;
    case '!':
        Expr = ParseUnary();
        break;
    case '-':
        Expr = ParseUnary();
        break;
    case '(':
        Expr = ParseParen();
    }

    // so long if, better method?
    if(CurTok=='+'||CurTok=='-'||CurTok=='*'||CurTok=='/'||CurTok=='&'||CurTok=='|'||(CurTok>=moreEqual && CurTok<= equalSign)){
        Expr = ParseBinOpRHS(0,std::move(Expr));
    }
    return Expr;
}

/// expression
/// the difference of Expr and Expression is that, Expression must be not a part of binary or unary expression
/// such as the right value , parameter , array index , return value
/// but Expr may be a part of binary expression  Expression::= Expr1 * (Expr2 - Expr3) Expression::= !(Expr1&Expr2)
/// the expression could not be wrapped in paren
static std::unique_ptr<ExprAST> ParseExpression()
{
	std::unique_ptr<ExprAST> expression = ParseExpr();
    if(expression->wrapInParen()){
       ErrorQ("the expression do not need to be wrapped in ()", lineN);
       return nullptr;
    }
    return expression;
}

