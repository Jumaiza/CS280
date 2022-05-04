// Zaid Abujumaiza
// PA3

#include <vector>
#include <string>
#include "parseInt.h"
#include "val.h"

using namespace std;
map<string, bool> defVar;
map<string, Token> SymTable;
map<string, Value> TempsResults;
queue<Value> *ValueQueue;

ostream &operator<<(ostream &out, const map<string, Token> &op)
{
    for (auto i = op.begin(); i != op.end(); i++)
    {
        out << i->first << ": " << i->second << endl;
    }
    return out;
}

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
    LexItem progToken = Parser::GetNextToken(in, line);

    if (progToken != PROGRAM)
    {
        ParseError(line, "Missing PROGRAM.");
        // print error no program
        return false;
    }

    LexItem identifierToken = Parser::GetNextToken(in, line);

    if (identifierToken != IDENT)
    {
        ParseError(line, "Missing Program Name.");
        // print error no name
        return false;
    }

    // save ident in map
    defVar[identifierToken.GetLexeme()] = true;

    LexItem semToken = Parser::GetNextToken(in, line);

    if (semToken != SEMICOL)
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

        // bring token back
        Parser::PushBackToken(currentToken);

        if (currentToken == BEGIN)
        {
            break;
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
    bool runLoop = true;

    vector<string> identifiers;

    while (runLoop)
    {
        LexItem currentToken = Parser::GetNextToken(in, line);

        if (currentToken != IDENT)
        {
            ParseError(line, "Not Ident in DeclStmt");
            // not an identifier
            return false;
        }

        if ((defVar.find(currentToken.GetLexeme())->second) == true)
        {
            ParseError(line, "Variable Redefinition");
            // print error already defined
            ParseError(line, "Incorrect variable in Declaration Statement.");
            // wrong variable
            return false;
        }

        defVar[currentToken.GetLexeme()] = true;
        // add to map
        identifiers.push_back(currentToken.GetLexeme());

        LexItem commaToken = Parser::GetNextToken(in, line);

        if (commaToken != COMMA)
        {
            Parser::PushBackToken(commaToken);
            // take back token
            break;
        }
    }

    // colon

    LexItem newToken = Parser::GetNextToken(in, line);

    if (newToken != COLON)
    {
        ParseError(line, "Unrecognized Input Pattern");
        // print token lexeme with error
        cout << "(" << newToken.GetLexeme() << ")" << endl;
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

    for (auto i = identifiers.begin(); i != identifiers.end(); i++)
    {
        // add to map
        SymTable[*i] = dataType.GetToken();
    }

    return true;
}

bool ProgBody(istream &in, int &line)
{

    bool runLoop = true;
    // placement boolean

    // skip BEGIN
    Parser::GetNextToken(in, line);

    while (runLoop)
    {
        LexItem currentToken = Parser::GetNextToken(in, line);

        if (currentToken == END)
        {
            break;
        }
        Parser::PushBackToken(currentToken);
        int lastLine = line;

        if (Stmt(in, line) == false)
        {
            ParseError(line, "Syntactic error in Program Body.");
            // send syntax error in progbody
            return false;
        }

        LexItem semiColonToken = Parser::GetNextToken(in, line);

        if (!(semiColonToken == SEMICOL))
        {
            if (line != lastLine)
            {
                line = lastLine;
            }

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

    default:
        // else push back nand return false
        Parser::PushBackToken(currentToken);
        // exit
        return false;
    }

    return statementStatus;
}

bool WriteLnStmt(istream &in, int &line)
{
    LexItem Token;
    ValueQueue = new queue<Value>;
    Token = Parser::GetNextToken(in, line);

    if (Token != LPAREN)
    {
        ParseError(line, "Missing Left Parenthesis");
        // print error then exit
        return false;
    }

    bool ex = ExprList(in, line); // puts all Exprs in ValueQueue

    if (!ex)
    {
        ParseError(line, "Missing expression after WriteLn");
        // print error then exit
        return false;
    }

    Token = Parser::GetNextToken(in, line);
    if (Token != RPAREN)
    {

        ParseError(line, "Missing Right Parenthesis");
        // print error then exit
        return false;
    }

    // Evaluate: print out the list of expressions' values
    while (!(*ValueQueue).empty())
    {
        Value nextValue = (*ValueQueue).front();
        // add
        cout << nextValue;
        // remove
        ValueQueue->pop();
    }

    cout << endl;

    return ex;
}

bool ExprList(istream &in, int &line)
{
    Value currentVal;

    bool expressionStatus = false;

    expressionStatus = Expr(in, line, currentVal);

    if (!expressionStatus)
    {
        ParseError(line, "Missing Expression in ExprList");
        // print error no expression
        return false;
    }

    // send value to queue
    (*ValueQueue).push(currentVal);

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

bool Expr(istream &in, int &line, Value &retVal)
{
    Value currentVal1, currentVal2;
    bool testTerm = Term(in, line, currentVal1);
    LexItem Token;

    if (!testTerm || currentVal1.IsErr())
    {
        // ParseError(line,"Run-Time Error Missing Expression in Assignment Statment");
        return false;
    }
    retVal = currentVal1;

    Token = Parser::GetNextToken(in, line);
    if (Token.GetToken() == ERR)
    {
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << Token.GetLexeme() << ")" << endl;
        return false;
    }
    // Evaluate: evaluate the expression for addition or subtraction
    while (Token == PLUS || Token == MINUS)
    {
        testTerm = Term(in, line, currentVal2);
        if (!testTerm)
        {
            ParseError(line, "Missing operand after operator");
            return false;
        }

        if (Token == PLUS)
        {
            retVal = retVal + currentVal2;
            if (retVal.IsErr())
            {
                ParseError(line, "Illegal addition operation.");
                return false;
            }
        }
        else if (Token == MINUS)
        {
            retVal = retVal - currentVal2;
            if (retVal.IsErr())
            {
                ParseError(line, "Illegal subtraction operation.");
                return false;
            }
        }

        Token = Parser::GetNextToken(in, line);
        if (Token.GetToken() == ERR)
        {
            ParseError(line, "Unrecognized Input Pattern");
            cout << "(" << Token.GetLexeme() << ")" << endl;
            return false;
        }
    }
    Parser::PushBackToken(Token);
    return true;
} // end of Expr

bool Term(istream &in, int &line, Value &retVal)
{
    Value currentVal1, currentVal2;

    if (SFactor(in, line, currentVal1) == false)
    {
        // go back
        return false;
    }

    bool runLoop = true;

    while (runLoop)
    {

        LexItem currentToken = Parser::GetNextToken(in, line);

        if (currentToken != DIV && currentToken != MULT)
        {
            // push back
            Parser::PushBackToken(currentToken);
            // leave loop
            break;
        }

        if (SFactor(in, line, currentVal2) == false)
        {
            return false;
            // exit
        }

        if (currentVal2.IsInt() == true)
        {
            if (0 == currentVal2.GetInt())
            {
                ParseError(line, "Run-Time Error-Illegal Division by Zero");
                // print run time division error
                return false;
            }
        }
        if (currentVal2.IsReal() == true)
        {
            if (0.00 == currentVal2.GetReal())
            {
                ParseError(line, "Run-Time Error-Illegal Division by Zero");
                // print error then exit
                return false;
            }
        }

        if (currentToken == MULT)
        {
            currentVal1 = currentVal1 * currentVal2;
        }
        else if (currentToken == DIV)
        {
            currentVal1 = currentVal1 / currentVal2;
        }
    }

    retVal = currentVal1;

    return true;
}

bool SFactor(istream &in, int &line, Value &retVal)
{

    LexItem currentToken = Parser::GetNextToken(in, line);

    int num = 0;

    if (currentToken == PLUS)
    {
        // return 1 to factor
        num = 1;
    }
    else if (currentToken == MINUS)
    {
        // return -1
        num = -1;
    }
    else
    {
        // send back
        Parser::PushBackToken(currentToken);
    }

    Value currentVal1;

    bool goodFactor = Factor(in, line, num, currentVal1);

    if (num != 0 && currentVal1.IsString())
    {
        // print error illegal type
        ParseError(line, "Illegal Operand Type for Sign Operator");
        // exit
        return false;
    }

    if (!goodFactor)
    {
        // print not good factor
        ParseError(line, "Not good factor in SFactor");
        // exit
        return false;
    }

    retVal = currentVal1;

    return true;
}

bool Factor(istream &in, int &line, int sign, Value &retVal)
{

    LexItem currentToken = Parser::GetNextToken(in, line);

    if ((currentToken == RCONST) || (currentToken == ICONST) || (currentToken == SCONST))
    {
        retVal = Value(currentToken.GetLexeme());

        if ((currentToken == ICONST))
        {
            retVal = Value(std::stoi(currentToken.GetLexeme()));

            if (sign == -1) // check for number
            {
                int newInt = retVal.GetInt() * -1;
                // set int
                retVal.SetInt(newInt);
            }
        }
        else if (currentToken == RCONST)
        {
            retVal = Value(std::stof(currentToken.GetLexeme()));
            // check number

            if (sign == -1)
            {
                int newReal = retVal.GetReal() * -1;
                retVal.SetReal(newReal);
            }
        }
        return true;
    }

    if (currentToken == IDENT)
    {
        if (defVar.find(currentToken.GetLexeme()) == defVar.end())
        {
            // print error
            ParseError(line, "Using Undeclared Variable: " + currentToken.GetLexeme());
            // exit
            return false;
        }
        if (TempsResults.find(currentToken.GetLexeme()) == TempsResults.end())
        {
            // print error undefined
            ParseError(line, "Undefined Variable: " + currentToken.GetLexeme());
            // exit
            return false;
        }

        retVal = TempsResults[currentToken.GetLexeme()];
        if (sign == -1) // check number
        {
            if (retVal.GetType() == VREAL)
            {
                int newReal2 = retVal.GetReal() * -1;
                retVal.SetReal(newReal2);
            }
            if (retVal.GetType() == VINT)
            {
                int newInt2 = retVal.GetInt() * -1;
                retVal.SetInt(newInt2);
            }
        }
        return true;
    }

    // continue

    if (currentToken != LPAREN)
    {
        // no left paranth
        return false;
    }

    Value currentVal1;
    // declare

    if (Expr(in, line, currentVal1) == false)
    {
        // print missing
        ParseError(line, "Missing expression after LPAREN in Factor");
        // exit
        return false;
    }

    LexItem rParenthToken = Parser::GetNextToken(in, line);

    if (rParenthToken != RPAREN)
    {
        ParseError(line, "Missing ) after expression");
        // print error no right parenth
        return false;
    }
    retVal = currentVal1;

    // exit
    return true;
}

bool IfStmt(istream &in, int &line)
{
    Value currentVal;

    LexItem currentToken = Parser::GetNextToken(in, line);

    if (currentToken != LPAREN)
    {
        ParseError(line, "Missing Left Parenthesis in IfStmt");
        // print error no left parenth
        return false;
    }

    if (!LogicExpr(in, line, currentVal))
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

    bool check = currentVal.GetBool();

    if (check)
    {

        if (!Stmt(in, line))
        {
            ParseError(line, "Missing Statement for If-Stmt Then-Part");
            // print error no stmt
            return false;
        }
    }
    else
    {
        bool runLoop = true;
        while (runLoop)
        {
            currentToken = Parser::GetNextToken(in, line);

            if (currentToken == SEMICOL || currentToken == ELSE)
            {
                Parser::PushBackToken(currentToken);
                // dip
                break;
            }
            if (currentToken == ERR)
            {
                // print error
                ParseError(line, "Looking for THEN or ; after logicalExpr.");
                // exit
                return false;
            }
        }
    }

    currentToken = Parser::GetNextToken(in, line);

    if (currentToken == ELSE)
    {
        bool checkValue = currentVal.GetBool();
        if (!checkValue)
        {
            if (!Stmt(in, line))
            {
                ParseError(line, "Missing Statement for If-Stmt else-Part");
                return false;
            }
        }
        else
        {
            // check for end of statement
            bool runLoop2 = true;
            while (runLoop2)
            {
                currentToken = Parser::GetNextToken(in, line);

                if (currentToken == ELSE || currentToken == SEMICOL)
                {
                    Parser::PushBackToken(currentToken);
                    // send back then exit
                    break;
                }
                if (currentToken == ERR)
                {
                    ParseError(line, "Looking for THEN or ; after logicalExpr.");
                    // exit and print error
                    return false;
                }
            }
        }
    }
    else
    {
        Parser::PushBackToken(currentToken);
    }
    return true;
}

bool LogicExpr(istream &in, int &line, Value &retVal)
{
    Value currentVal1, currentVal2;

    if (Expr(in, line, currentVal1) == false)
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

    if (Expr(in, line, currentVal2) == false)
    {
        ParseError(line, "Missing if statement Logic Expression");
        // print error no statement
        return false;
    }

    // continue

    if (currentToken == EQUAL)
    {
        retVal = currentVal1 == currentVal2;
        // check bool
    }
    else if (currentToken == LTHAN)
    {
        retVal = currentVal1 < currentVal2;
        // bool value
    }
    else if (currentToken == GTHAN)
    {
        retVal = currentVal1 > currentVal2;
        // check if greater
    }

    bool errorValue = retVal.IsErr();

    if (errorValue)
    {
        // print error mixed
        ParseError(line, "Run-Time Error-Illegal Mixed Type Operands for a Logic Expression");
        // exit
        return false;
    }

    return true;
}

bool AssignStmt(istream &in, int &line)
{

    Value currentValue;
    LexItem varToken;

    if (!Var(in, line, varToken))
    {
        ParseError(line, "Missing Left-Hand Side Variable in Assignment statement");
        // print error no var
        return false;
    }

    if (Parser::GetNextToken(in, line) != ASSOP)
    {
        ParseError(line, "Missing Assignment Operator");
        // print error no assignment
        return false;
    }

    if (!(Expr(in, line, currentValue)))
    {
        ParseError(line, "Missing Expression in Assignment Statment");
        // print error no expr
        return false; // exit
    }

    if (SymTable[varToken.GetLexeme()] == REAL)
    {
        if (currentValue.GetType() == VINT)
        {
            currentValue.SetReal(currentValue.GetInt());
            currentValue.SetType(VREAL);
        }
        else if (currentValue.GetType() != VREAL)
        {
            cout << currentValue << endl;
            // print value
            ParseError(line, "Illegal Assignment Operation");
            // exit false
            return false;
        }
    }
    else if (SymTable[varToken.GetLexeme()] == INTEGER)
    {
        if (currentValue.GetType() == VREAL)
        {
            currentValue.SetInt(currentValue.GetReal());
            currentValue.SetType(VINT);
        }
        else if (currentValue.GetType() != VINT)
        {
            // return error
            ParseError(line, "Illegal Assignment Operation");
            // exit false
            return false;
        }
    }

    TempsResults[varToken.GetLexeme()] = currentValue;

    return true;
}

bool Var(istream &in, int &line, LexItem &idtok)
{

    LexItem currentToken = Parser::GetNextToken(in, line);

    idtok = currentToken;

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

    return true; // exit
}