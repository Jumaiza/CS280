#include <iostream>
#include <cctype>
#include <string>
#include "lex.h"
#include <vector>
#include <algorithm>
using namespace std;

LexItem id_or_kw(const string &lexeme, int linenum)
{
    return LexItem(IDENT, "", 5);
}

ostream &operator<<(ostream &out, const LexItem &tok)
{
    return out;
}

int main(int argc, char *argv[])
{

    string input = argv[1];

    return 0;
}