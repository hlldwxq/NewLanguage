#include "../header/Parser.h"

///==========Expression========================//

/// boolExpr ::= true   false
std::unique_ptr<ConstantBoolAST> Parser::ParseBoolConstantExpr(){
    int line1 = lineN;
    if(CurTok != Token::tok_true && CurTok != Token::tok_false)
        Bug("call ParseBoolExpr, except true or false",lineN);
    
    bool b = (CurTok == Token::tok_true) ? true : false;
    
    std::unique_ptr<ConstantBoolAST> num = std::make_unique<ConstantBoolAST>(b,line1);
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

    std::unique_ptr<NumberExprAST> num = std::make_unique<NumberExprAST>(NumStr,line1); 
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
                error("expect ) at line: "+std::to_string(lineN));
               // ErrorQ("expect ) in function call", lineN);
               // return nullptr;
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
            error("expect ] at line: "+std::to_string(lineN));
           // ErrorQ("except ] in array index", lineN);
           // return nullptr;
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
    
    UOperator* unaryOp;
    switch(Op){
        case Token::exclamation_point:
            unaryOp = new exclamation(line1);
            break;
        case Token::minus:
            unaryOp = new negative(line1);
            break;
        default:
            error("unvalid unary operator at line: "+std::to_string(lineN));
           // ErrorQ("unvalid unary operator",lineN);
           // return nullptr;
    }
	getNextToken();  //eat op
    
    if(Op == Token::minus && CurTok == Token::tok_number){
        if(NumStr=="0"){
            error("neg zero is invalid at line: "+std::to_string(line1));
        }
        std::string number = NumStr;
        if(number[0]=='-')
            number.erase(0,1);
        else
            number = "-"+number;

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
std::unique_ptr<ExprAST> Parser::ParseBinOpRHS(int TokPrec, std::unique_ptr<ExprAST> LHS)
{
	int line1 = lineN;
    std::unique_ptr<ExprAST> RHS;
    BOperator* BinOp;
	while(GetTokPrecedence()>0){
        TokPrec = GetTokPrecedence();
        switch( CurTok ){
            case Token::equal_sign:
                BinOp = new equal_sign();
                break;  //==
            case Token::not_equal:   //!=
                BinOp = new not_equal();
                break;
            case Token::less_equal:   //<=
                BinOp = new less_equal();
                break;
            case Token::greater_equal:   //>=
                BinOp = new greater_equal();
                break;  
            case Token::plus:   //+
                BinOp = new plus(lineN);
                break;
            case Token::minus:       //-
                BinOp = new minus(lineN);
                break;
            case Token::star:        //*
                BinOp = new star(lineN);
                break;
            case Token::disvision:   // /
                BinOp = new division(lineN);
                break;
            case Token::andT:        // &
                BinOp = new andT(lineN);
                break;
            case Token::orT:         // |
                BinOp = new orT(lineN);
                break;
            case Token::greater_than:   // >
                BinOp = new greater_than();
                break;
            case Token::less_than:    // <
                BinOp = new less_than();
                break;
            default:
                error("unvalid symbol for binary operator at line: "+std::to_string(lineN));
               // ErrorQ(,lineN);
               // return nullptr;
        }
		getNextToken(); // eat binop

		// Parse the right expression after the binary operator.
		RHS = ParseBinaryExpr();
		if (RHS == nullptr)
			return nullptr;

		// If BinOp binds less tightly with RHS than the operator after RHS, let
		// the pending operator take RHS as its LHS.
		int NextPrec = GetTokPrecedence();
		
		if (TokPrec < NextPrec)
		{
			RHS = ParseBinOpRHS(NextPrec, std::move(RHS));
			if (!RHS)
				return nullptr;
		}else if( NextPrec > 0){
            LHS = std::make_unique<BinaryExprAST>(BinOp, std::move(LHS), std::move(RHS),line1);
        }
	}
    return std::make_unique<BinaryExprAST>(BinOp, std::move(LHS), std::move(RHS),line1);
}

/// newExpr ::= new type name[expr]
std::unique_ptr<NewExprAST> Parser::ParseNewExpr(){
    int line1 = lineN;
    if(CurTok != Token::tok_new){
        error("expect new at line: "+std::to_string(lineN));
       // ErrorQ("except new", lineN);
        //return nullptr;
    }
    getNextToken(); //eat new

    if( !(isType()) ){
        error("expect a type at line: "+std::to_string(lineN));
       // ErrorQ("except a type", lineN);
       // return nullptr;
    }
    
    auto eletype = ParseType();
    if(!eletype){
        error("expect an type for new at line: "+std::to_string(lineN));
       // ErrorQ("except an type for new",lineN);
       // return nullptr;
    }
    
    PointType* type = new PointType(eletype);
    
    if(CurTok!=Token::left_square_bracket){
       error("expect [ at line: "+std::to_string(lineN));
       // ErrorQ("unenough [, except [",lineN);
       // return nullptr;
    }
    getNextToken(); //eat [

    std::unique_ptr<ExprAST> index = ParseExpr();
    if(index==nullptr)
        return nullptr;

    if(CurTok!=Token::right_square_bracket){
        error("expect ] at line: "+std::to_string(lineN));
       // ErrorQ("except ] in array definition",lineN);
       // return nullptr;
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
        error("expect ) at line: "+std::to_string(lineN));
       // ErrorQ("expect )",lineN);
       // return nullptr;
    }
    getNextToken(); // eat )

    return Expr;
}

std::unique_ptr<ExprAST> Parser::ParseBinaryExpr(){
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
        error("unexpected word for an expression at line: "+std::to_string(lineN));
       // ErrorQ("unexpected word for an expression",lineN);
       // return nullptr;
    }
    if(CurTok == Token::left_square_bracket){
        Expr = ParseArrayIndexExpr(std::move(Expr));
    }
    return Expr;
}
/// Expr ::= number
///      ::= true false
///      ::= null
///      ::= identifier ...  //variableName, array index, function call
///      ::= ! - 
///      ::= Expr binop Expr
std::unique_ptr<ExprAST> Parser::ParseExpr(){

    std::unique_ptr<ExprAST> Expr = Parser::ParseBinaryExpr();
    // so long if, better method?
    if(CurTok==Token::plus||CurTok==Token::minus||CurTok==Token::star||CurTok==Token::disvision||CurTok==Token::andT||CurTok==Token::orT||CurTok==Token::assignment||(static_cast<int>(CurTok)>=static_cast<int>(Token::equal_sign) && static_cast<int>(CurTok)<= static_cast<int>(Token::less_than))){
        Expr = ParseBinOpRHS(GetTokPrecedence(),std::move(Expr));
    }
    
    return Expr;
}



