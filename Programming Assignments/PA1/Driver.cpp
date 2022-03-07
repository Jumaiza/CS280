#include <iostream>
#include <map>
#include <fstream>
#include <vector>
#include "lex.h"
#include <algorithm>

#include <cctype>
#include <map>
using std::map;

using namespace std;

int main(int argc, char *argv[])
{

	int LineCounter = 0;
	int tokenCounter = 0;

	bool vFlag = false;
	bool iconstFlag = false;
	bool rconstFlag = false;
	bool sconstFlag = false;
	bool identFlag = false;

	string fileName;

	bool fileNameGiven = false;

	ifstream file;

	if (argc < 2)
	{
		cout << "NO SPECIFIED INPUT FILE NAME." << endl;
		return 0;
	}
	for (int i = 1; i < argc; i++)
	{

		string arg(argv[i]);

		if (arg == "-v")
		{
			vFlag = true;
		}
		else if (arg == "-iconst")
		{
			iconstFlag = true;
		}
		else if (arg == "-rconst")
		{
			rconstFlag = true;
		}
		else if (arg == "-sconst")
		{
			sconstFlag = true;
		}
		else if (arg == "-ident")
		{
			identFlag = true;
		}
		else if (arg[0] == '-')
		{
			cout << "UNRECOGNIZED FLAG" << arg << endl;
		}
		else if (!fileNameGiven)
		{
			fileName = arg;
			file.open(fileName);
			if (!file.is_open())
			{
				cout << "CANNOT OPEN the File " << arg << endl;
				return 0;
			}
			fileNameGiven = true;
		}
		else if (fileNameGiven)
		{
			cout << "ONLY ONE FILE NAME ALLOWED." << endl;
			return 0;
		}
	}

	cout << fileName << endl;

	map<string, LexItem> identMap;
	map<int, LexItem> iconstMap;
	map<double, LexItem> rconstMap;
	map<string, LexItem> sconstMap;

	while (true)
	{
		LexItem tok = getNextToken(file, LineCounter);

		if (tok.GetToken() == DONE)
		{
			break;
		}
		else if (tok.GetToken() == ERR)
		{
			return 0;
		}
		else
		{
			tokenCounter++;
		}

		if (vFlag)
		{
			cout << tok << endl;
		}
		else
		{

			string newLexeme = tok.GetLexeme();

			if (tok == IDENT && identFlag)
			{
				identMap[newLexeme] = tok;
			}
			if (tok == ICONST && iconstFlag)
			{
				iconstMap[stoi(newLexeme)] = tok;
			}
			if (tok == RCONST && rconstFlag)
			{
				rconstMap[stod(newLexeme)] = tok;
			}
			if (tok == SCONST && sconstFlag)
			{
				sconstMap[newLexeme] = tok;
			}
		}
	}

	cout << "Lines: " << LineCounter << endl;

	if (tokenCounter != 0)
	{
		cout << "Tokens: " << tokenCounter << endl;
	}

	if (iconstFlag && !(iconstMap.empty()))
	{
		cout << "INTEGERS:" << endl;

		map<int, LexItem>::iterator it;

		for (it = iconstMap.begin(); it != iconstMap.end(); it++)
		{
			cout << it->first << endl;
		}
	}
	if (rconstFlag && !(rconstMap.empty()))
	{
		cout << "REALS:" << endl;

		map<double, LexItem>::iterator itTwo;

		for (itTwo = rconstMap.begin(); itTwo != rconstMap.end(); itTwo++)
		{
			cout << itTwo->first << endl;
		}
	}
	if (sconstFlag && !(sconstMap.empty()))
	{
		cout << "STRINGS:" << endl;

		map<string, LexItem>::iterator itThree;

		for (itThree = sconstMap.begin(); itThree != sconstMap.end(); itThree++)
		{
			cout << "'" << itThree->first << "'" << endl;
		}
	}
	if (identFlag && !(identMap.empty()))
	{
		cout << "IDENTIFIERS:" << endl;

		map<string, LexItem>::iterator itFour;

		for (itFour = identMap.begin(); itFour != --identMap.end(); itFour++)
		{
			cout << itFour->first << ", ";
		}
		cout << itFour->first << endl;
	}

	return 0;
}
