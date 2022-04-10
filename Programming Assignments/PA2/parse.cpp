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
	while (true)
	{
		LexItem currentKey = Parser::GetNextToken(in, line);

		if (currentKey != IDENT)
		{
			ParseError(line, "Not Ident in DeclStmt");
			return false;
		}
		else if ((defVar.find(currentKey.GetLexeme())->second) == true)
		{
			ParseError(line, "Variable Redefinition");
			ParseError(line, "Incorrect variable in Declaration Statement.");
			return false;
		}
		else
		{
			defVar[currentKey.GetLexeme()] = true;
			currentKey = Parser::GetNextToken(in, line);
		}
	}
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
