//
//  main.cpp
//  lexdriver.cpp
//
//  Created by Sandeep Singh on 3/5/22.
//
#include <iostream>
#include "lex.h"
#include <map>
#include <string>
#include <fstream>
#include <cctype>
#include <sstream>

using namespace std;

int main(int argc, char *argv[])
{
	// Variables
	ifstream file;
	string fileName;
	int lineNumber = 0;
	int tokencounter = 0;
	int filecounter = 0;
	bool v = false;
	bool iconst = false;
	bool rconst = false;
	bool sconst = false;
	bool ident = false;

	// No file name is given
	if (argc < 2)
	{
		cout << "NO SPECIFIED INPUT FILE NAME." << endl;
		return 0;
	}

	for (int i = 1; i < argc; i++)
	{
		string newarg(argv[i]);
		if (argv[i][0] == '-')
		{
			if (newarg == "-v")
				v = true;
			else if (newarg == "-iconst")
			{
				iconst = true;
			}
			else if (newarg == "-rconst")
			{
				rconst = true;
			}
			else if (newarg == "-sconst")
			{
				sconst = true;
			}
			else if (newarg == "-ident")
			{
				ident = true;
			}
			else
			{
				cout << "UNRECOGNIZED FLAG " << newarg << endl;
				return 0;
			}
		}
		else if (filecounter == 0)
		{
			filecounter++;
			fileName = newarg;
		}
		else if (filecounter > 0)
		{
			cout << "ONLY ONE FILE NAME ALLOWED." << endl;
			return 0;
		}
	}

	file.open(fileName);

	if (file.is_open())
	{
		map<int, LexItem> iconstflag;
		map<double, LexItem> rconstflag;
		map<string, LexItem> sconstflag;
		map<string, LexItem> identflag;

		while (true)
		{
			LexItem tok = getNextToken(file, lineNumber);

			if (tok != DONE)
				tokencounter += 1;

			if (tok.GetToken() == DONE)
			{
				break;
			}

			if (tok.GetToken() == ERR)
			{
				return 0;
			}

			if (v)
			{
				cout << tok << endl;
			}

			// if flag then put into map
			string nLexeme = tok.GetLexeme();
			if (iconst && tok == ICONST)
				iconstflag[stoi(nLexeme)] = tok;
			if (rconst && tok == RCONST)
				rconstflag[stod(nLexeme)] = tok;
			if (sconst && tok == SCONST)
				sconstflag[nLexeme] = tok;
			if (ident && tok == IDENT)
				identflag[nLexeme] = tok;
		}
		cout << "Lines: " << lineNumber << endl;

		if (tokencounter != 0)
			cout << "Tokens: " << tokencounter << endl;

		// sconst flag output
		if (sconst && !sconstflag.empty())
		{
			cout << "STRINGS:" << endl;
			map<string, LexItem>::iterator itr3;
			for (itr3 = sconstflag.begin(); itr3 != sconstflag.end(); itr3++)
			{
				cout << itr3->first << endl;
			}
		}

		// iconst flag output
		if (iconst && !iconstflag.empty())
		{
			cout << "INTEGERS:" << endl;
			map<int, LexItem>::iterator itr;
			for (itr = iconstflag.begin(); itr != iconstflag.end(); itr++)
			{
				cout << itr->first << endl;
			}
		}

		// rconst flag output
		if (rconst && !rconstflag.empty())
		{
			cout << "REALS:" << endl;
			map<double, LexItem>::iterator itr2;
			for (itr2 = rconstflag.begin(); itr2 != rconstflag.end(); itr2++)
			{
				cout << itr2->first << endl;
			}
		}

		// ident flag output
		if (ident && !identflag.empty())
		{
			cout << "IDENTIFIERS:" << endl;
			map<string, LexItem>::iterator itr4;
			for (itr4 = identflag.begin(); itr4 != --identflag.end(); itr4++)
			{
				cout << itr4->first << ", ";
			}
			cout << itr4->first << endl;
		}
	}

	else
	{
		cout << "CANNOT OPEN the File " << fileName << endl;
	}
	return 0;
}
