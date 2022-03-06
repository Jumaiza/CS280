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
        case INIDENT:

            if (in.peek() == -1)
            {
                return LexItem(ERR, lexeme, linenumber);
            }
            else if (!(isalpha(character)) && !(character == '_') && !(isdigit(character)))
            {
                in.putback(character);
                lexState = START;
                return LexItem(IDENT, lexeme, linenumber);
                break;
            }
            else
            {
                lexeme += character;
                // lexeme += char(in.peek());
                break;
            }
        case INSTRING:

            if (in.peek() == -1)
            {
                return LexItem(ERR, lexeme, linenumber);
            }
            else if (character != '\'')
            {
                lexeme += character;
                break;
            }
            else
            {
                lexState = START;
                return LexItem(SCONST, lexeme, linenumber);
                break;
            }
        case ININTEGER:

            if (in.peek() == -1)
            {
                return LexItem(ERR, lexeme, linenumber);
            }
            else if (!(isdigit(character)))
            {
                // end
                if (character == '.')
                {
                    lexState = INREAL;
                    break;
                }
                else
                {
                    lexState = START;
                    return LexItem(ICONST, lexeme, linenumber);
                    break;
                }
            }
            {
                lexeme += character;
                break;
            }

        case INREAL:

            if (in.peek() == -1)
            {
                return LexItem(ERR, lexeme, linenumber);
            }
            else if (!isdigit(character))
            {
                in.putback(character);
                lexState = START;
                return LexItem(RCONST, lexeme, linenumber);
                break;
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
                return LexItem(COMMA, lexeme, linenumber);
                break;
            }
            else
            {
                lexeme = +character;
                break;
            }
        }
    }

    return LexItem(DONE, lexeme, linenumber);
}