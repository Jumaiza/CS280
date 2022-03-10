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
			break;
		}
		else if (arg == "-iconst")
		{
			iconstFlag = true;
			break;
		}
		else if (arg == "-rconst")
		{
			rconstFlag = true;
			break;
		}
		else if (arg == "-sconst")
		{
			sconstFlag = true;
			break;
		}
		else if (arg == "-ident")
		{
			identFlag = true;
			break;
		}
		else if (arg[0] == '-')
		{
			cout << "UNRECOGNIZED FLAG " << arg << endl;
			return 0;
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

	map<string, LexItem> identMap;
	map<int, LexItem> iconstMap;
	map<double, LexItem> rconstMap;
	map<string, LexItem> sconstMap;

	int LineCounter = 0;
	int tokenCounter = 0;

	LexItem tok;

	while ((tok = getNextToken(file, LineCounter)) != DONE && tok != ERR)
	{

		tokenCounter++;

		if (vFlag)
		{
			cout << tok << endl;
		}

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

	if (identFlag)
	{
		tokenCounter++;
		LineCounter++;
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
			cout << itThree->first << endl;
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
