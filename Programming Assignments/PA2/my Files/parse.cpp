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

/*
	Prog ::= PROGRAM IDENT; DeclBlock ProgBody
	DeclBlock ::= VAR {DeclStmt;}
	DeclStmt ::= Ident {, Ident} : (Integer | Real | String)
	ProgBody ::= BEGIN {Stmt;} END
	Stmt ::= AssignStmt | IfStmt | WriteLnStmt | ForStmt
	WriteLnStmt ::= WRITELN (ExprList)
	IfStmt ::= IF ( LogicExpr ) THEN Stmt [ELSE Stmt]
	ForStmt ::= FOR Var := ICONST (TO | DOWNTO) ICONST DO Stmt
	AssignStmt ::= Var := Expr
	ExprList ::= Expr {, Expr}
	Expr ::= Term {(+|-) Term}
	Term ::= SFactor {( * | / ) SFactor}
	SFactor ::= [(+ | -)] Factor
	LogicExpr ::= Expr (= | > | <) Expr
	Var ::= IDENT
	Factor ::= IDENT | ICONST | RCONST | SCONST | (Expr)
*/

bool Program(istream &in, int &line)
{
	LexItem currentKeyword = Parser::GetNextToken(in, line);

	if (currentKeyword != PROGRAM)
	{
		ParseError(line, "Missing PROGRAM.");
		return false;
	}
	else
	{
		currentKeyword = Parser::GetNextToken(in, line);
	}

	if (currentKeyword != IDENT)
	{
		ParseError(line, "Missing Program Name.");
		return false;
	}
	else
	{
		defVar[currentKeyword.GetLexeme()] = true;
		currentKeyword = Parser::GetNextToken(in, line);
	}

	if (currentKeyword != SEMICOL)
	{
		ParseError(line, "Missing semicolon in Statement.");
		return false;
	}

	if (!DeclBlock(in, line))
	{
		ParseError(line, "Incorrect Declaration Section");
		return false;
	}

	if (ProgBody(in, line))
	{
		ParseError(line, "Incorrect Program Body.");
		return false;
	}
	return true;
}

bool DeclBlock(istream &in, int &line)
{

	if (Parser::GetNextToken(in, line) != VAR)
	{
		ParseError(line, "Missing 'VAR' in DeclBlock");
		return false;
	}

	while (true)
	{
		LexItem currentKey = Parser::GetNextToken(in, line);

		if (currentKey == BEGIN)
		{
			Parser::PushBackToken(currentKey);
			break;
		}
		else
		{
			Parser::PushBackToken(currentKey);
		}

		if (!DeclStmt(in, line))
		{
			ParseError(line, "Syntactic error in Declaration Block.");
			return false;
		}

		currentKey = Parser::GetNextToken(in, line);
		if (currentKey != SEMICOL)
		{
			ParseError(line, "Missing semicol");
			return false;
		}
	}
	return true;
}

bool DeclStmt(istream &in, int &line)
{
	LexItem currentKey = Parser::GetNextToken(in, line);

	while (currentKey == IDENT || currentKey == COMMA)
	{
		if (currentKey == IDENT)
		{
			if (defVar[currentKey.GetLexeme()] == true)
			{
				ParseError(line, "Ident already defined");
				return false;
			}
		}
		currentKey = Parser::GetNextToken(in, line);
	}

	if (currentKey != COLON)
	{
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << currentKey.GetLexeme() << ")" << endl;
		ParseError(line, "Incorrect variable in Declaration Statement.");
		return false;
	}
	else
	{
		currentKey = Parser::GetNextToken(in, line);
	}

	if (currentKey != REAL || currentKey != INTEGER || currentKey != STRING)
	{
		ParseError(line, "Incorrect Declaration Type.");
		return false;
	}
	return true;
}

// ProgBody ::= BEGIN {Stmt;} END
bool ProdBody(istream &in, int &line)
{
	LexItem currentKey = Parser::GetNextToken(in, line);

	while (currentKey != END)
	{
		if (!Stmt(in, line))
		{
			ParseError(line, "Syntactic error in Program Body.");
			return false;
		}
		else
		{
			currentKey = Parser::GetNextToken(in, line);
		}

		if (currentKey != SEMICOL)
		{
			ParseError(line, "Missing semicolon in Statement.");
			return false;
		}
	}

	return true;
}

bool Stmt(istream &in, int &line)
{
	bool status;
	// cout << "in ContrlStmt" << endl;
	LexItem t = Parser::GetNextToken(in, line);

	switch (t.GetToken())
	{

	case WRITELN:
		status = WriteLnStmt(in, line);
		// cout << "After WriteStmet status: " << (status? true:false) <<endl;
		break;

	case IF:
		status = IfStmt(in, line);
		break;

	case IDENT:
		Parser::PushBackToken(t);
		status = AssignStmt(in, line);

		break;

	case FOR:
		status = ForStmt(in, line);
		break;

	default:
		Parser::PushBackToken(t);
		return false;
	}

	return status;
} // End of Stmt

// WriteStmt:= wi, ExpreList
bool WriteLnStmt(istream &in, int &line)
{
	LexItem t;
	// cout << "in WriteStmt" << endl;

	t = Parser::GetNextToken(in, line);
	if (t != LPAREN)
	{

		ParseError(line, "Missing Left Parenthesis");
		return false;
	}

	bool ex = ExprList(in, line);

	if (!ex)
	{
		ParseError(line, "Missing expression after WriteLn");
		return false;
	}

	t = Parser::GetNextToken(in, line);
	if (t != RPAREN)
	{

		ParseError(line, "Missing Right Parenthesis");
		return false;
	}
	// Evaluate: print out the list of expressions values

	return ex;
}

bool IfStmt(istream &in, int &line)
{
	LexItem currentToken = Parser::GetNextToken(in, line);
	if (currentToken != LPAREN)
	{
		ParseError(line, "Missing Left Parenthesis in IfStmt");
		return false;
	}

	if (!LogicExpr(in, line))
	{
		// ParseError(line, "Logical Expression flawed in IfStmt");
		return false;
	}
	else
	{
		currentToken = Parser::GetNextToken(in, line);
	}

	if (currentToken != RPAREN)
	{
		ParseError(line, "Missing Right Parenthesis after logical in IfStmt");
		return false;
	}
	else
	{
		currentToken = Parser::GetNextToken(in, line);
	}

	if (currentToken != THEN)
	{
		ParseError(line, "Missing THEN after LogicExpr in IfStmt");
		return false;
	}

	if (!Stmt(in, line))
	{
		ParseError(line, "Missing Statement for If-Stmt Then-Part");
		return false;
	}
	else
	{
		currentToken = Parser::GetNextToken(in, line);
	}

	if (currentToken == ELSE)
	{
		if (!Stmt(in, line))
		{
			ParseError(line, "Stmt wrong in IfStmt's else");
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
		return false;
	}
	else
	{

		currentToken = Parser::GetNextToken(in, line);
	}

	if (currentToken != ASSOP)
	{
		ParseError(line, "no ASSOP after Variable in For");
		return false;
	}
	else
	{

		currentToken = Parser::GetNextToken(in, line);
	}

	if (currentToken != ICONST)
	{
		ParseError(line, "No ICONST assignment in for");
		return false;
	}
	else
	{

		currentToken = Parser::GetNextToken(in, line);
	}

	if (currentToken != TO && currentToken != DOWNTO)
	{
		ParseError(line, "No TO or DOWNTO assignment in for");
		return false;
	}
	else
	{

		currentToken = Parser::GetNextToken(in, line);
	}

	if (currentToken != ICONST)
	{
		ParseError(line, "No ICONST assignment in for after To/DOWNTO");
		return false;
	}
	else
	{
		currentToken = Parser::GetNextToken(in, line);
	}

	if (currentToken != DO)
	{
		ParseError(line, "No DO in for");
		return false;
	}

	if (!Stmt(in, line))
	{
		ParseError(line, "Stmt wrong in For loop");
		return false;
	}

	return true;
}

bool ExprList(istream &in, int &line)
{
	bool expressionIsGood = Expr(in, line);

	if (!expressionIsGood)
	{
		ParseError(line, "Missing Expression in ExprList");
		return false;
	}

	LexItem currentToken = Parser::GetNextToken(in, line);

	if (currentToken == COMMA)
	{
		expressionIsGood = ExprList(in, line);
	}
	else if (currentToken.GetToken() == ERR)
	{
		ParseError(line, "Unrecognized Input Pattern");
		return false;
	}
	else
	{
		Parser::PushBackToken(currentToken);
		return true;
	}

	return expressionIsGood;
}

bool Expr(istream &in, int &line)
{
	LexItem currentToken = Parser::GetNextToken(in, line);

	while (currentToken != ERR)
	{

		bool TermIsGood = Term(in, line);

		if (!TermIsGood)
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}

		if (currentToken != PLUS && currentToken != MINUS)
		{
			Parser::PushBackToken(currentToken);
			break;
		}

		currentToken = Parser::GetNextToken(in, line);
	}

	if (currentToken == ERR)
	{
		return false;
	}

	return true;
}

bool Term(istream &in, int &line)
{
	while (true)
	{
		bool SfactorIsGood = SFactor(in, line);

		if (!SfactorIsGood)
		{
			return false;
		}
		LexItem currentToken = Parser::GetNextToken(in, line);

		if (currentToken != MULT && currentToken != DIV)
		{
			Parser::PushBackToken(currentToken);
			break;
		}
	}
	return true;
}
bool SFactor(istream &in, int &line)
{
	LexItem currentToken = Parser::GetNextToken(in, line);

	int sign = 0;

	if (currentToken == PLUS)
	{
		sign = 1;
	}
	else if (currentToken == MINUS)
	{
		sign = -1;
	}
	else
	{
		Parser::PushBackToken(currentToken);
	}

	bool factorIsGood = Factor(in, line, sign);
	if (!factorIsGood)
	{
		return false;
	}

	return true;
}

bool Factor(istream &in, int &line, int sign)
{
	LexItem currentToken = Parser::GetNextToken(in, line);

	if ((currentToken == ICONST) || (currentToken == RCONST) || (currentToken == SCONST))
	{
		return true;
	}
	else if (currentToken == IDENT)
	{
		if (defVar.find(currentToken.GetLexeme()) == defVar.end())
		{
			ParseError(line, "Using Undefined Variable");
			return false;
		}
		return true;
	}
	else if (currentToken != LPAREN)
	{
		return false;
	}

	bool exprIsGood = Expr(in, line);

	if (!exprIsGood)
	{
		ParseError(line, "Missing expression after LPAREN in Factor");
		return false;
	}
	else
	{
		currentToken = Parser::GetNextToken(in, line);
	}

	if (currentToken != RPAREN)
	{
		ParseError(line, "Missing ) after expression");
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
		return false;
	}
	else if ((defVar.find(currentToken.GetLexeme())->second) == false)
	{
		ParseError(line, "Undeclared Variable");
		return false;
	}
	return true;
}

bool AssignStmt(istream &in, int &line)
{
	LexItem currrentToken = Parser::GetNextToken(in, line);

	if (Var(in, line))
	{
		ParseError(line, "Missing Left-Hand Side Variable in Assignment statement");
		return false;
	}
	else
	{
		currrentToken = Parser::GetNextToken(in, line);
	}

	if (currrentToken != ASSOP)
	{
		ParseError(line, "Missing Assignment Operator");
		return false;
	}
	else
	{
		currrentToken = Parser::GetNextToken(in, line);
	}

	if (Expr(in, line))
	{
		ParseError(line, "Missing Expression in Assignment Statment");
		return false;
	}

	return true;
}

// ExprList:= Expr {,Expr}
bool ExprList(istream &in, int &line)
{
	bool status = false;
	// cout << "in ExprList and before calling Expr" << endl;
	status = Expr(in, line);
	if (!status)
	{
		ParseError(line, "Missing Expression");
		return false;
	}

	LexItem tok = Parser::GetNextToken(in, line);

	if (tok == COMMA)
	{
		// cout << "before calling ExprList" << endl;
		status = ExprList(in, line);
		// cout << "after calling ExprList" << endl;
	}
	else if (tok.GetToken() == ERR)
	{
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else
	{
		Parser::PushBackToken(tok);
		return true;
	}
	return status;
}
