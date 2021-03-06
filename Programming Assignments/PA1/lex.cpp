#include <iostream>
#include "lex.h"
using namespace std;

LexItem getNextToken(istream &in, int &linenumber)
{
    enum TokState
    {
        START,
        INIDENT,
        INSTRING,
        ININTEGER,
        INREAL,
        INCOMMENT
    }

    lexState = START;

    char character;

    string lexeme = "";

    while (in.get(character))
    {

        switch (lexState)
        {

        case START:

            if (character == '\n')
            {
                linenumber++;
                break;
            }
            if (isspace(character))
            {
                break;
            }

            lexeme += character;

            if (isalpha(character) || character == '_')
            {
                lexState = INIDENT;
                continue;
            }
            if (character == '\'')
            {
                lexState = INSTRING;
                continue;
            }
            if (isdigit(character))
            {
                lexState = ININTEGER;
                continue;
            }
            if (character == '.')
            {
                lexState = INREAL;
                continue;
            }
            if (character == '(' && char(in.peek()) == '*')
            {
                lexState = INCOMMENT;
                continue;
            }
            if (character == '(')
            {
                lexeme += character;
                return LexItem(LPAREN, lexeme, linenumber);
            }

            if (character == '+')
            {
                lexeme += character;
                return LexItem(PLUS, lexeme, linenumber);
            }

            if (character == '-')
            {
                lexeme += character;
                return LexItem(MINUS, lexeme, linenumber);
            }

            if (character == '*')
            {
                lexeme += character;
                return LexItem(MULT, lexeme, linenumber);
            }

            if (character == '/')
            {
                lexeme += character;
                return LexItem(DIV, lexeme, linenumber);
            }

            if (character == ':')
            {
                lexeme += character;
                if (in.peek() == '=')
                {
                    in.get(character);
                    lexeme += character;
                    return LexItem(ASSOP, lexeme, linenumber);
                }
                else
                {
                    return LexItem(COLON, lexeme, linenumber);
                }
            }

            if (character == ')')
            {
                lexeme += character;
                return LexItem(RPAREN, lexeme, linenumber);
            }

            if (character == '=')
            {
                lexeme += character;
                return LexItem(EQUAL, lexeme, linenumber);
            }

            if (character == '>')
            {
                lexeme += character;
                return LexItem(GTHAN, lexeme, linenumber);
            }

            if (character == '<')
            {
                lexeme += character;
                return LexItem(LTHAN, lexeme, linenumber);
            }

            if (character == ',')
            {
                lexeme += character;
                return LexItem(COMMA, lexeme, linenumber);
            }

            if (character == ';')
            {
                lexeme += character;
                return LexItem(SEMICOL, lexeme, linenumber);
            }
            cout << "Error in line " << linenumber + 1 << " (" << lexeme << ")" << endl;
            return LexItem(ERR, lexeme, linenumber);
            break;
        case INIDENT:

            if (!(isalpha(character)) && !(character == '_') && !(isdigit(character)))
            {
                in.putback(character);
                lexState = START;
                // return LexItem(IDENT, lexeme, linenumber);
                return id_or_kw(lexeme, linenumber);
                break;
            }
            else
            {
                lexeme += character;
                // lexeme += char(in.peek());
                break;
            }
            break;
        case INSTRING:

            if (in.peek() == -1)
            {
                return LexItem(ERR, lexeme, linenumber);
            }
            else if (character == '\"' || ((isspace(character) && isspace(in.peek()))))
            {
                lexeme += character;
                cout << "Error in line " << linenumber + 1 << " (" << lexeme << ")" << endl;
                // return LexItem(ERR, lexeme, linenumber);
                exit(0);
            }
            else if (character != '\'')
            {
                lexeme += character;
                break;
            }
            else
            {
                lexeme += character;
                lexState = START;
                return LexItem(SCONST, lexeme, linenumber);
                break;
            }
        case ININTEGER:

            if (in.peek() == -1)
            {
                return LexItem(ERR, lexeme, linenumber);
            }
            else if (character == '.')
            {
                lexState = INREAL;
                lexeme += character;
                continue;
            }
            else if (!isdigit(character))
            {
                in.putback(character);
                return LexItem(ICONST, lexeme, linenumber);
                lexState = START;
                continue;
            }
            else
            {
                lexeme += character;
                continue;
            }

        case INREAL:

            if (character == '.')
            {
                lexeme += character;
                cout << "Error in line " << linenumber + 1 << " (" << lexeme << ")" << endl;
                // return LexItem(ERR, lexeme, linenumber);
                exit(0);
            }
            if (lexeme.at(lexeme.length() - 1) == '.' && !(isdigit(character)))
            {
                lexeme += character;
                cout << "Error in line " << linenumber << " (" << lexeme << ")" << endl;
                // return LexItem(ERR, lexeme, linenumber);
                exit(0);
            }
            else if (!(isdigit(character)))
            {
                in.putback(character);
                lexState = START;
                return LexItem(RCONST, lexeme, linenumber);
                break;
            }
            else
            {
                lexeme += character;
                continue;
            }
        case INCOMMENT:

            if (character == '\n')
            {
                linenumber++;
                break;
            }
            else if (character == ')' && lexeme.back() == '*')
            {
                lexState = START;
                lexeme = "";
                break;
            }
            else
            {
                lexeme = +character;
                break;
            }
        }
    }

    if (lexState == INCOMMENT)
        cout << endl
             << "Missing a comment end delimiters '*)' at line " << linenumber << endl;
    return LexItem(DONE, lexeme, linenumber);
}

LexItem id_or_kw(const string &lexeme, int linenum)
{

    string lexemeUpper = lexeme;

    for (int i = 0; i < lexemeUpper.length(); i++)
    {
        lexemeUpper[i] = toupper(lexemeUpper[i]);
    }

    // converts string to token
    map<string, Token> StringToToken =
        {{"IDENT", IDENT}, {"ICONST", ICONST}, {"RCONST", RCONST}, {"SCONST", SCONST}, {"PLUS", PLUS}, {"MINUS", MINUS}, {"MULT", MULT}, {"PROGRAM", PROGRAM}, {"WRITELN", WRITELN}, {"INTEGER", INTEGER}, {"BEGIN", BEGIN}, {"END", END}, {"IF", IF}, {"REAL", REAL}, {"STRING", STRING}, {"VAR", VAR}, {"ELSE", ELSE}, {"FOR", FOR}, {"THEN", THEN}, {"DO", DO}, {"TO", TO}, {"DOWNTO", DOWNTO}, {"DIV", DIV}, {":=", ASSOP}, {"LPAREN", LPAREN}, {"RPAREN", RPAREN}, {"COMMA", COMMA}, {"EQUAL", EQUAL}, {"GTHAN", GTHAN}, {"LTHAN", LTHAN}, {"SEMICOL", SEMICOL}, {"COLON", COLON}, {"ERR", ERR}, {"DONE", DONE}};

    // base
    Token currentToken = IDENT;

    // iterates through map boolean value
    map<string, Token>::iterator MapIterator = StringToToken.find(lexemeUpper);

    // determines the keyword was found
    if (StringToToken.end() != MapIterator)
    {
        currentToken = MapIterator->second;
    }

    // return lexitem with new token
    return LexItem(currentToken, lexemeUpper, linenum);
}

ostream &operator<<(ostream &out, const LexItem &tok)
{

    // convert token to string
    map<Token, string> TokenToString = {{IDENT, "IDENT"}, {ICONST, "ICONST"}, {RCONST, "RCONST"}, {SCONST, "SCONST"}, {PLUS, "PLUS"}, {MINUS, "MINUS"}, {MULT, "MULT"}, {PROGRAM, "PROGRAM"}, {WRITELN, "WRITELN"}, {INTEGER, "INTEGER"}, {BEGIN, "BEGIN"}, {END, "END"}, {IF, "IF"}, {REAL, "REAL"}, {STRING, "STRING"}, {VAR, "VAR"}, {ELSE, "ELSE"}, {FOR, "FOR"}, {THEN, "THEN"}, {DO, "DO"}, {TO, "TO"}, {DOWNTO, "DOWNTO"}, {DIV, "DIV"}, {ASSOP, "ASSOP"}, {LPAREN, "LPAREN"}, {RPAREN, "RPAREN"}, {COMMA, "COMMA"}, {EQUAL, "EQUAL"}, {GTHAN, "GTHAN"}, {LTHAN, "LTHAN"}, {SEMICOL, "SEMICOL"}, {COLON, "COLON"}, {ERR, "ERR"}, {DONE, "DONE"}};

    // get token of lexitem
    Token currentToken = tok.GetToken();

    // get string from token
    string GetTokenInString = TokenToString.find(currentToken)->second;

    out << GetTokenInString;

    if (GetTokenInString == "IDENT" || GetTokenInString == "ICONST" || GetTokenInString == "RCONST")
    {
        out << "(" << tok.GetLexeme() << ")";
    }
    else if (GetTokenInString == "SCONST")
    {
        string lexeme = tok.GetLexeme();
        string withoutQuotes = lexeme.substr(1, lexeme.size() - 2);
        out << "(" << withoutQuotes << ")";
    }

    return out;
}