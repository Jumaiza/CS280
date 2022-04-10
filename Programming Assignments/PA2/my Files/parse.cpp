/* Definitions and some functions implementations
 * parse.cpp to be completed
 * Programming Assignment 2
 * Spring 2022
 */

#include "parse.h"

map<string, bool> defVar;
map<string, Token> SymTable;

namespace Parser
{
	bool pushed_back = false;
	LexItem pushed_token;

	static LexItem GetNextToken(istream &in, int &line)
	{
		if (pushed_back)
		{
			pushed_back = false;
			return pushed_token;
		}
		return getNextToken(in, line);
	}

	static void PushBackToken(LexItem &t)
	{
		if (pushed_back)
		{
			abort();
		}
		pushed_back = true;
		pushed_token = t;
	}

}

static int error_count = 0;

int ErrCount()
{
	return error_count;
}

void ParseError(int line, string msg)
{
	++error_count;
	cout << line << ": " << msg << endl;
}

bool Prog(istream &in, int &line)
{

	if (Parser::GetNextToken(in, line) != PROGRAM)
	{
		ParseError(line, "Missing PROGRAM.");
		// print error no program
		return false;
	}

	LexItem identToken = Parser::GetNextToken(in, line);

	if (identToken != IDENT)
	{
		ParseError(line, "Missing Program Name.");
		// print error no name
		return false;
	}

	// save ident in map
	defVar[identToken.GetLexeme()] = true;

	if (Parser::GetNextToken(in, line) != SEMICOL)
	{
		ParseError(line, "Missing semicolon in Statement.");
		// print error no semicolon
		return false;
	}

	if (!DeclBlock(in, line))
	{
		ParseError(line, "Incorrect Declaration Section.");
		// print error wrong declaration section
		return false;
	}

	if (!ProgBody(in, line))
	{
		ParseError(line, "Incorrect Program Body.");
		// print error wrong body
		return false;
	}

	return true;
}

bool DeclBlock(istream &in, int &line)
{

	if (Parser::GetNextToken(in, line) != VAR)
	{
		ParseError(line, "Missing 'VAR' in DeclBlock");
		// print error no var
		return false;
	}

	bool RunLoop = true;

	while (RunLoop)
	{
		LexItem currentToken = Parser::GetNextToken(in, line);

		if (currentToken == BEGIN)
		{
			// bring token back
			Parser::PushBackToken(currentToken);
			break;
		}
		else
		{
			Parser::PushBackToken(currentToken);
		}

		// check statement
		if (!DeclStmt(in, line))
		{
			ParseError(line, "Syntactic error in Declaration Block.");
			// print error syntax in decblock
			return false;
		}

		// get semicolon
		currentToken = Parser::GetNextToken(in, line);

		if (currentToken != SEMICOL)
		{
			ParseError(line, "Missing semicolon");
			// print error no semicol
			return false;
		}
	}
	return true;
}

bool DeclStmt(istream &in, int &line)
{
	// left of statement

	bool runLoop = true;

	while (runLoop)
	{
		LexItem currentToken = Parser::GetNextToken(in, line);

		if (currentToken != IDENT)
		{
			ParseError(line, "Not Ident in DeclStmt");
			// not an identifier
			return false;
		}

		bool isDefined = defVar.find(currentToken.GetLexeme())->second == true;

		if (isDefined)
		{
			ParseError(line, "Variable Redefinition");
			// print error already defined
			ParseError(line, "Incorrect variable in Declaration Statement.");
			// wrong variable
			return false;
		}
		else
		{

			defVar[currentToken.GetLexeme()] = true;
		}

		LexItem commaToken = Parser::GetNextToken(in, line);

		if (commaToken != COMMA)
		{
			Parser::PushBackToken(commaToken);
			// take back token
			break;
		}
	}

	// colon

	LexItem colonToken = Parser::GetNextToken(in, line);

	if (colonToken != COLON)
	{
		ParseError(line, "Unrecognized Input Pattern");
		// print token lexeme with error
		cout << "(" << colonToken.GetLexeme() << ")" << endl;
		// return error wrong var
		ParseError(line, "Incorrect variable in Declaration Statement.");
		// exit
		return false;
	}

	// right side

	LexItem dataType = Parser::GetNextToken(in, line);

	if (dataType != INTEGER && dataType != REAL && dataType != STRING)
	{
		ParseError(line, "Incorrect Declaration Type.");
		// error wrong data type
		return false;
	}

	return true;
}

bool ProgBody(istream &in, int &line)
{

	bool runLoop = true;
	// placement boolean

	while (runLoop)
	{
		LexItem currentToken = Parser::GetNextToken(in, line);

		if (currentToken == BEGIN)
		{
			continue;
		}
		else if (currentToken == END)
		{
			break;
		}
		else
		{
			Parser::PushBackToken(currentToken);
		}

		if (Stmt(in, line) == false)
		{
			ParseError(line, "Syntactic error in Program Body.");
			// send syntax error in progbody
			return false;
		}

		LexItem semiColonToken = Parser::GetNextToken(in, line);

		if (!(semiColonToken == SEMICOL))
		{
			ParseError(line--, "Missing semicolon in Statement.");
			// print error no semicolon
			return false;
		}
	}

	return true;
}

bool Stmt(istream &in, int &line)
{
	bool statementStatus;

	LexItem currentToken = Parser::GetNextToken(in, line);

	// switch been different statments
	switch (currentToken.GetToken())
	{

	case WRITELN:
		// send to write line statment
		statementStatus = WriteLnStmt(in, line);
		// break from switch
		break;

	case IF:
		// send to if statement
		statementStatus = IfStmt(in, line);
		// exit
		break;

	case IDENT:
		// send ident back
		Parser::PushBackToken(currentToken);
		// send to assignstmet
		statementStatus = AssignStmt(in, line);
		// exit
		break;

	case FOR:
		// send to for function
		statementStatus = ForStmt(in, line);
		// exit
		break;

	default:
		// else push back nand return false
		Parser::PushBackToken(currentToken);
		return false;
	}

	return statementStatus;
}

bool WriteLnStmt(istream &in, int &line)
{
	LexItem currentToken = Parser::GetNextToken(in, line);

	if (currentToken != LPAREN)
	{
		ParseError(line, "Missing Left Parenthesis");
		// print error no left parenth
		return false;
	}

	bool expression = ExprList(in, line);

	if (!expression)
	{
		ParseError(line, "Missing expression after WriteLn");
		// print error no expression
		return false;
	}

	currentToken = Parser::GetNextToken(in, line);

	if (currentToken != RPAREN)
	{
		ParseError(line, "Missing Right Parenthesis");
		// print error no right parenth
		return false;
	}

	return expression;
}

bool IfStmt(istream &in, int &line)
{
	LexItem currentToken = Parser::GetNextToken(in, line);

	if (currentToken != LPAREN)
	{
		ParseError(line, "Missing Left Parenthesis in IfStmt");
		// print error no left parenth
		return false;
	}

	if (!LogicExpr(in, line))
	{
		// exit
		return false;
	}

	currentToken = Parser::GetNextToken(in, line);

	if (currentToken != RPAREN)
	{
		ParseError(line, "Missing Right Parenthesis after logical in IfStmt");
		// print error no right parenth
		return false;
	}

	currentToken = Parser::GetNextToken(in, line);
	if (currentToken != THEN)
	{
		ParseError(line, "Missing THEN after LogicExpr in IfStmt");
		// print error no right parenth
		return false;
	}

	if (!Stmt(in, line))
	{
		ParseError(line, "Missing Statement for If-Stmt Then-Part");
		// print error no stmt
		return false;
	}

	currentToken = Parser::GetNextToken(in, line);
	if (currentToken == ELSE)
	{
		if (!Stmt(in, line))
		{
			ParseError(line, "Stmt wrong in IfStmt's else");
			// print error wrong statment
			return false;
		}
	}
	else
	{
		Parser::PushBackToken(currentToken);
	}

	return true;
}

bool ForStmt(istream &in, int &line)
{

	LexItem currentToken = Parser::GetNextToken(in, line);
	if (currentToken != IDENT)
	{
		ParseError(line, "no IDENT after FOR");
		// print error no ident
		return false;
	}

	LexItem operatorToken = Parser::GetNextToken(in, line);
	if (operatorToken != ASSOP)
	{
		ParseError(line, "no ASSOP after Variable in For");
		// print error no operator
		return false;
	}

	LexItem iconstToken = Parser::GetNextToken(in, line);
	if (iconstToken != ICONST)
	{
		ParseError(line, "No ICONST assignment in for");
		// print error no iconst
		return false;
	}

	LexItem assginmentToken = Parser::GetNextToken(in, line);
	if (assginmentToken != TO && assginmentToken != DOWNTO)
	{
		ParseError(line, "No TO or DOWNTO assignment in for");
		// print error no assignment
		return false;
	}

	LexItem newIconstToken = Parser::GetNextToken(in, line);
	if (newIconstToken != ICONST)
	{
		ParseError(line, "No ICONST assignment in for after To/DOWNTO");
		// print error no iconst after assginment
		return false;
	}

	LexItem doToken = Parser::GetNextToken(in, line);
	if (doToken != DO)
	{
		ParseError(line, "No DO in for");
		// print error no do
		return false;
	}

	if (!Stmt(in, line))
	{
		ParseError(line, "Stmt wrong in For loop");
		// print error wrong statement
		return false;
	}

	return true;
}

bool AssignStmt(istream &in, int &line)
{
	if (!Var(in, line))
	{
		ParseError(line, "Missing Left-Hand Side Variable in Assignment statement");
		// print error no var
		return false;
	}
	else if (Parser::GetNextToken(in, line) != ASSOP)
	{
		ParseError(line, "Missing Assignment Operator");
		// print error no assignment
		return false;
	}
	else if (!Expr(in, line))
	{
		ParseError(line, "Missing Expression in Assignment Statment");
		// print error no expression
		return false;
	}
	return true;
}

bool ExprList(istream &in, int &line)
{
	bool expressionStatus = false;

	expressionStatus = Expr(in, line);
	if (!expressionStatus)
	{
		ParseError(line, "Missing Expression");
		// print error no expression
		return false;
	}

	LexItem currentToken = Parser::GetNextToken(in, line);

	if (currentToken == COMMA)
	{
		expressionStatus = ExprList(in, line);
	}
	else if (currentToken.GetToken() == ERR)
	{
		ParseError(line, "Unrecognized Input Pattern");
		// print error wrong input
		cout << "(" << currentToken.GetLexeme() << ")" << endl;
		// print lexeme
		return false;
	}
	else
	{
		Parser::PushBackToken(currentToken);
		// send back
		return true;
	}
	return expressionStatus;
}

bool Expr(istream &in, int &line)
{

	bool runLoop = true;

	while (runLoop)
	{
		LexItem currentToken;

		if (!Term(in, line))
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}

		currentToken = Parser::GetNextToken(in, line);

		if (currentToken == ERR)
		{
			// wrong input
			return false;
		}
		if (currentToken != MINUS && currentToken != PLUS)
		{
			// send back
			Parser::PushBackToken(currentToken);
			break;
		}
	}
	return true;
}

bool Term(istream &in, int &line)
{
	bool runLoop = true;

	while (runLoop)
	{
		if (SFactor(in, line) == false)
		{
			// go back
			return false;
		}

		LexItem currentToken = Parser::GetNextToken(in, line);
		if (currentToken != MULT && currentToken != DIV)
		{
			// push back
			Parser::PushBackToken(currentToken);
			break;
		}
	}
	return true;
}

bool SFactor(istream &in, int &line)
{
	LexItem currentToken = Parser::GetNextToken(in, line);

	if (currentToken != PLUS && currentToken != MINUS)
	{
		Parser::PushBackToken(currentToken);
	}

	int num = 0;
	if (Factor(in, line, num) == false)
	{
		return false;
	}

	return true;
}

bool LogicExpr(istream &in, int &line)
{

	if (Expr(in, line) == false)
	{
		ParseError(line, "left expr wrong in LogicExpr");
		// print error wrong left expression
		return false;
	}

	LexItem currentToken = Parser::GetNextToken(in, line);
	if (currentToken != GTHAN && currentToken != EQUAL && currentToken != LTHAN)
	{
		ParseError(line, "Operator not correct in LogicExpr");
		// print error wrong operator
		return false;
	}
	else if (Expr(in, line) == false)
	{
		ParseError(line, "Missing if statement Logic Expression");
		// print error no statement
		return false;
	}

	return true;
}

bool Factor(istream &in, int &line, int sign)
{
	LexItem currentToken = Parser::GetNextToken(in, line);

	if (currentToken == SCONST || currentToken == ICONST || currentToken == RCONST)
	{
		// factor is good
		return true;
	}
	else if (currentToken == IDENT)
	{
		bool identFound = defVar.find(currentToken.GetLexeme()) == defVar.end();

		if (identFound)
		{
			ParseError(line, "Using Undefined Variable");
			// ident not found in map
			return false;
		}
		else
			return true;
	}
	else if (currentToken != LPAREN)
	{
		// no left paranth
		return false;
	}

	if (Expr(in, line) == false)
	{
		ParseError(line, "Missing expression after LPAREN in Factor");
		return false;
	}

	LexItem rParenthToken = Parser::GetNextToken(in, line);
	if (rParenthToken != RPAREN)
	{
		ParseError(line, "Missing ) after expression");
		// print error no right parenth
		return false;
	}

	return true;
}

bool Var(istream &in, int &line)
{
	LexItem currentToken = Parser::GetNextToken(in, line);

	if (currentToken != IDENT)
	{
		ParseError(line, "not a valid variable because Not an IDENT in Var");
		// print error wrong var
		return false;
	}
	bool isDeclared = defVar.find(currentToken.GetLexeme())->second == true;
	if (!isDeclared)
	{
		ParseError(line, "Undeclared Variable");
		// print error not declared
		return false;
	}

	return true;
}