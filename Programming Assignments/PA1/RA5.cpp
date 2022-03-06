#include <iostream>
#include <map>
#include <fstream>
#include <vector>
#include "lex.h"

using namespace std;

LexItem id_or_kw(const string &lexeme, int linenum)
{

    // converts string to token
    map<string, Token> StringToToken =
        {{"IDENT", IDENT}, {"ICONST", ICONST}, {"RCONST", RCONST}, {"SCONST", SCONST}, {"PLUS", PLUS}, {"MINUS", MINUS}, {"MULT", MULT}, {"PROGRAM", PROGRAM}, {"WRITELN", WRITELN}, {"INTEGER", INTEGER}, {"BEGIN", BEGIN}, {"END", END}, {"IF", IF}, {"REAL", REAL}, {"STRING", STRING}, {"VAR", VAR}, {"ELSE", ELSE}, {"FOR", FOR}, {"THEN", THEN}, {"DO", DO}, {"TO", TO}, {"DOWNTO", DOWNTO}, {"DIV", DIV}, {":=", ASSOP}, {"LPAREN", LPAREN}, {"RPAREN", RPAREN}, {"COMMA", COMMA}, {"EQUAL", EQUAL}, {"GTHAN", GTHAN}, {"LTHAN", LTHAN}, {"SEMICOL", SEMICOL}, {"COLON", COLON}, {"ERR", ERR}, {"DONE", DONE}};

    // base
    Token currentToken = IDENT;

    // iterates through map boolean value
    map<string, Token>::iterator MapIterator = StringToToken.find(lexeme);

    // determines if it is a keyword or identifier
    if (StringToToken.end() != MapIterator)
    {
        currentToken = MapIterator->second;
    }

    // return lexitem with new token
    return LexItem(currentToken, lexeme, linenum);
}

ostream &operator<<(ostream &out, const LexItem &tok)
{

    // convert token to string
    map<Token, string> TokenToString = {{IDENT, "IDENT"}, {ICONST, "ICONST"}, {RCONST, "RCONST"}, {SCONST, "SCONST"}, {PLUS, "PLUS"}, {MINUS, "MINUS"}, {MULT, "MULT"}, {PROGRAM, "PROGRAM"}, {WRITELN, "WRITELN"}, {INTEGER, "INTEGER"}, {BEGIN, "BEGIN"}, {END, "END"}, {IF, "IF"}, {REAL, "REAL"}, {STRING, "STRING"}, {VAR, "VAR"}, {ELSE, "ELSE"}, {FOR, "FOR"}, {THEN, "THEN"}, {DO, "DO"}, {TO, "TO"}, {DOWNTO, "DOWNTO"}, {DIV, "DIV"}, {ASSOP, "ASSOP"}, {LPAREN, "LPAREN"}, {RPAREN, "RPAREN"}, {COMMA, "COMMA"}, {EQUAL, "EQUAL"}, {GTHAN, "GTHAN"}, {LTHAN, "LTHAN"}, {SEMICOL, "SEMICOL"}, {COLON, "COLON"}, {ERR, "ERR"}, {DONE, "DONE"}};

    // get token of lexitem
    Token currentToken = tok.GetToken();

    // get string from token
    string GetTokenInString = TokenToString.find(currentToken)->second;

    if (currentToken == IDENT || currentToken == ICONST || currentToken == SCONST || currentToken == RCONST)
    {
        out << GetTokenInString << " (" << tok.GetLexeme() << ")" << endl;

        return out;
    }

    out << GetTokenInString << endl;

    return out;
}