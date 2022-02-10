#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <cctype>
#include <map>

using namespace std;

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        cout << "No file is found." << endl;
        return 0;
    }

    string filename = argv[1];
    ifstream inFile;

    inFile.open(filename);

    map<char, int> charMap;
    int charNum = 0;

    if (!inFile.is_open())
    {
        cout << "File cannot be opened: " << filename << endl;
        return 0;
    }
    else if (inFile.peek() == std::ifstream::traits_type::eof())
    {
        cout << "File is empty." << endl;
        return 0;
    }
    else
    {

        string line;
        while (getline(inFile, line))
        {

            for (char character : line)
            {
                if (isspace(character))
                {
                    continue;
                }
                else if (ispunct(character))
                {
                    charMap[character]++;
                    charNum++;
                }
                else
                {
                    charNum++;
                }
            }
        }
    }

    if (charMap.empty())
    {
        cout << "File is empty." << endl;
        return 0;
    }

    char maxChar;
    int maxCharNum = 0;

    for (auto it = charMap.begin(); it != charMap.end(); it++)
    {
        if (it->second > maxCharNum)
        {
            maxChar = it->first;
            maxCharNum = it->second;
        }
        else
        {
            continue;
        }
    }

    cout << "Number of characters in the file: " << charNum << endl;
    cout << "Punctuation characters in the file and their occurrences:" << endl
         << endl;

    for (auto it = charMap.begin(); it != charMap.end(); it++)
    {
        cout << it->first << ": " << it->second << endl;
    }

    cout << endl;
    cout << "The punctuation character with maximum occurrences is \"" << maxChar << "\" found " << maxCharNum << " times." << endl;

    return 0;
}