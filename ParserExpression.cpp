#include "Parser.h"

///==========Expression========================//

/// boolExpr ::= true   false
std::unique_ptr<NumberExprAST> Parser::ParseBoolConstantExpr(){
    int line1 = lineN;
    if(CurTok != Token::tok_true && CurTok != Token::tok_false)
        Bug("call ParseBoolExpr, except true or false",lineN);
    
    int b = (CurTok == Token::tok_true) ? 1 : 0;
    
    std::unique_ptr<NumberExprAST> num = std::make_unique<NumberExprAST>(b,line1);
    getNextToken(); // eat the bool
    return num;

}

/// boolExpr ::= null
std::unique_ptr<NullExprAST> Parser::ParseNullExpr(){
    int line1 = lineN;
    if(CurTok != Token::tok_null)
        Bug("call ParseNullExpr, except null",lineN);

    getNextToken(); //eat null
    return std::make_unique<NullExprAST>(line1);
}

/// numberExpr ::= number
std::unique_ptr<NumberExprAST> Parser::ParseNumberExpr(){
	int line1 = lineN;
    if(CurTok != Token::tok_number)
        Bug("call ParseNumberExpr, except constant number",lineN);

    std::unique_ptr<NumberExprAST> num = std::make_unique<NumberExprAST>(NumVal,line1); 
	getNextToken();	// eat the number						   
	return num;
}

/// callExpr ::= fName(expression1,expression2, ..)
std::unique_ptr<CallExprAST> Parser::ParseCallExpr(std::string functionName){
    int line1 = lineN;
    if(CurTok != Token::left_paren)
        Bug("call ParseCallExpr, except ( after the function name",lineN);

    getNextToken();  // eat (

    std::vector<std::unique_ptr<ExprAST>> args;
    while(CurTok != Token::right_paren){

        std::unique_ptr<ExprAST> arg = ParseExpr();
        if(arg == nullptr){
            return nullptr;  // we can believe that the ErrorQ info has been given
        }

        args.push_back(std::move(arg));

        if(CurTok != Token::comma){
            if(CurTok != Token::right_paren){
                ErrorQ("expect ) in function call", lineN);
                return nullptr;
            }
        }else{
            getNextToken();  //eat ,
        }
    }
    getNextToken(); //eat )
    return std::make_unique<CallExprAST>(functionName, std::move(args),line1);
}

/// arrayIndexExpr ::= arrayName[expression1][expression2]
std::unique_ptr<ArrayIndexExprAST> Parser::ParseArrayIndexExpr(std::unique_ptr<ExprAST> left){
    int line1 = lineN;
    if(CurTok != Token::left_square_bracket)
        Bug("call ParseArrayIndexExpr, except [ after array name",lineN);

    std::unique_ptr<ArrayIndexExprAST> arrayIndex = nullptr;
    
    do{
        getNextToken(); // eat [

        std::unique_ptr<ExprAST> index = ParseExpr();
        if(index == nullptr){
            return nullptr;  // we can believe that the ErrorQ log has been given 
        }

        if(CurTok != Token::right_square_bracket ){
            ErrorQ("except ] in array index", lineN);
            return nullptr;
        }
        getNextToken(); //eat ]

        if(arrayIndex == nullptr){
            arrayIndex = std::make_unique<ArrayIndexExprAST>(std::move(left),std::move(index),line1);
        }else{
            arrayIndex = std::make_unique<ArrayIndexExprAST>(std::move(arrayIndex),std::move(index),line1);
        }

    }while(CurTok == Token::left_square_bracket);

    return arrayIndex;
}

/// identifierexpr
///   ::= variableName
///   ::= arrayName[expression][expression]
///   ::= functionName '(' expression* ')'
std::unique_ptr<ExprAST> Parser::ParseIdentifierExpr()
{
    int line1 = lineN;
	if(CurTok != Token::tok_identifier)
        Bug("call ParseIdentifierExpr, except an identifier",lineN);

	std::string IdName = IdentifierStr;
	getNextToken(); // eat identifier.
	
	if (CurTok == Token::left_paren)
	{
        return ParseCallExpr(IdName);
	}
	else if (CurTok == Token::left_square_bracket)
	{
        std::unique_ptr<VariableAST> pointerName = std::make_unique<VariableAST>(IdName,line1,true);
        return ParseArrayIndexExpr(std::move(pointerName));
	}

    return std::make_unique<VariableAST>(IdName,line1);
}

/// unary
///   ::= ! expression
///   ::= - expression
std::unique_ptr<ExprAST> Parser::ParseUnary(){
    int line1 = lineN;
    Token Op = CurTok;

    if(Op != Token::exclamation_point && Op != Token::minus){
        Bug("call ParseUnary, but unvalid unary symbol",lineN);        
    }
    
    Operators unaryOp;
    switch(Op){
        case Token::exclamation_point:
            unaryOp = Operators::exclamation_point;
            break;
        case Token::minus:
            unaryOp = Operators::minus;
            break;
        default:
            ErrorQ("unvalid unary operator",lineN);
            return nullptr;
    }
	getNextToken();  //eat op
    
    if(Op == Token::minus && CurTok == Token::tok_number){
        // -8 is regarded as two token: - and constant number 8
        // and -8 should be regarded as constant number, no Unary
        long long number = -NumVal;
        getNextToken();  //eat number
        return std::make_unique<NumberExprAST>(number,line1);
    }

    if(Op == Token::minus || Op == Token::exclamation_point){
        std::unique_ptr<ExprAST> Operand = ParseExpr();
        if(Operand == nullptr)
            return nullptr;  //do not need to give an ErrorQ info again
        return std::make_unique<UnaryExprAST>(unaryOp, std::move(Operand),line1);
    }

    return nullptr;
}

/// binoprhs
///   ::= ('+' unary)*
std::unique_ptr<ExprAST> Parser::ParseBinOpRHS(int ExprPrec, std::unique_ptr<ExprAST> LHS)
{
	int line1 = lineN;
	while (true)
	{
		int TokPrec = GetTokPrecedence();
        // more check,if sumbol is valid
        //unvalid symbol does not mean error

		if (TokPrec < ExprPrec)
			return LHS;
        // in normal case, next TokPrec is bigger than or equals to the current prec
        // if TokPrec< ExprPrec, it means that there is no more BinOp

		Token Op = CurTok;
        Operators BinOp;
        switch(Op){
            case Token::equal_sign:
                BinOp = Operators::equal_sign;
                break;  //==
            case Token::not_equal:   //!=
                BinOp = Operators::not_equal;
                break;
            case Token::less_equal:  //<=
                BinOp = Operators::less_equal;
                break;
            case Token::more_equal:
                BinOp = Operators::more_equal;
                break;  //>=
            case Token::plus_sign:   //+
                BinOp = Operators::plus_sign;
                break;
            case Token::minus:       //-
                BinOp = Operators::minus;
                break;
            case Token::star:        //*
                BinOp = Operators::star;
                break;
            case Token::disvision:   // /
                BinOp = Operators::division;
                break;
            case Token::andT:                 // &
                BinOp = Operators::andT;
                break;
            case Token::orT:                   // |
                BinOp = Operators::orT;
                break;
            case Token::more_sign:            // >
                BinOp = Operators::more_sign;
                break;
            case Token::less_sign:            // <
                BinOp = Operators::less_sign;
                break;
            case Token::assignment:           // =
                BinOp = Operators::assignment;
                break;
            default:
                ErrorQ("unvalid symbol for binary operator",lineN);
                return nullptr;
        }
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
		LHS = std::make_unique<BinaryExprAST>(BinOp, std::move(LHS), std::move(RHS),line1);
	}
}

/// newExpr ::= new type name[expr]
std::unique_ptr<NewExprAST> Parser::ParseNewExpr(){
    int line1 = lineN;
    if(CurTok != Token::tok_new){
        ErrorQ("except new", lineN);
        return nullptr;
    }
    getNextToken(); //eat new

    if( !(isType()) ){
        ErrorQ("except a type", lineN);
        return nullptr;
    }
    
    auto type = ParseType();
    if(!type){
        ErrorQ("except an type for new",lineN);
        return nullptr;
    }
    
    if(CurTok!=Token::left_square_bracket){
        ErrorQ("unenough [, except [",lineN);
        return nullptr;
    }
    getNextToken(); //eat [

    std::unique_ptr<ExprAST> index = ParseExpr();
    if(index==nullptr)
        return nullptr;

    if(CurTok!=Token::right_square_bracket){
        ErrorQ("except ] in array definition",lineN);
        return nullptr;
    }
    getNextToken(); //eat ]

    return std::make_unique<NewExprAST>(type,std::move(index),line1);
}

/// paren
///    ::= (expression)
std::unique_ptr<ExprAST> Parser::ParseParen(){
    
    if(CurTok != Token::left_paren)
        Bug("call ParseParen, but no (",lineN);
    getNextToken(); //eat (

    std::unique_ptr<ExprAST> Expr = ParseExpr();

    if(CurTok!=Token::right_paren){
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
std::unique_ptr<ExprAST> Parser::ParseExpr(){

    std::unique_ptr<ExprAST> Expr;
    switch(CurTok){
    case Token::tok_number:
        Expr = ParseNumberExpr();
        break;
    case Token::tok_true:
        Expr = ParseBoolConstantExpr();
        break;
    case Token::tok_false:
        Expr = ParseBoolConstantExpr();
        break;
    case Token::tok_null:
        Expr = ParseNullExpr();
        break;
    case Token::tok_identifier:
        Expr = ParseIdentifierExpr();
        break;
    case Token::exclamation_point :
        Expr = ParseUnary();
        break;
    case Token::minus :
        Expr = ParseUnary();
        break;
    case Token::tok_new:
        Expr = ParseNewExpr();
        break;
    case Token::left_paren :
        Expr = ParseParen();
        break;
    default:
        ErrorQ("unexpected word for an expression",lineN);
        return nullptr;
    }

    // so long if, better method?
    if(CurTok==Token::plus_sign||CurTok==Token::minus||CurTok==Token::star||CurTok==Token::disvision||CurTok==Token::andT||CurTok==Token::orT||CurTok==Token::assignment||(static_cast<int>(CurTok)>=static_cast<int>(Token::equal_sign) && static_cast<int>(CurTok)<= static_cast<int>(Token::less_sign))){
        Expr = ParseBinOpRHS(0,std::move(Expr));
    }
    if(CurTok == Token::left_square_bracket){
        Expr = ParseArrayIndexExpr(std::move(Expr));
    }
    return Expr;
}

/// expression
/// the difference of Expr and Expression is that, Expression must be not a part of binary or unary expression
/// such as the right value , parameter , array index , return value
/// but Expr may be a part of binary expression  Expression::= Expr1 * (Expr2 - Expr3) Expression::= !(Expr1&Expr2)
/// the expression could not be wrapped in paren
/*static std::unique_ptr<ExprAST> ParseExpression()
{
	std::unique_ptr<ExprAST> expression = ParseExpr();
    if(expression->wrapInParen()){
       ErrorQ("the expression do not need to be wrapped in ()", lineN);
       return nullptr;
    }
    return expression;
}*/

