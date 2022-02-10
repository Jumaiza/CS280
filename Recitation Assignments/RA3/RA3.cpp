#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>

using namespace std;

int main(int argc, char *argv[])
{

    // cout << argc << endl;
    int numLines = 0;
    int nonBlankLines = 0;
    int numWords = 0;
    int numNames = 0;
    int numIntegers = 0;

    if (argc < 2)
    {
        cout << "No file name is found" << endl;
        return 0;
    }
    string filename = argv[1];
    ifstream inFile;

    //
    inFile.open(filename);

    if (!inFile.is_open())
    {
        cout << "File cannot be opened: " << filename << endl;
        return 0;
    }
    else if (inFile.peek() == std::ifstream::traits_type::eof())
    {
    }
    else
    {

        string line;
        while (getline(inFile, line))
        {

            numLines++;
            nonBlankLines++;

            if (line.empty())
            {
                nonBlankLines -= 1;
                continue;
            }

            else
            {

                istringstream split(line);
                string word;

                while (split >> word)
                {
                    numWords++;

                    if (isalpha(word[0]) && word.size() > 2)
                    {
                        numNames++;
                    }
                    else if (!(word.find_first_not_of("0123456789") != std::string::npos))
                    {
                        numIntegers++;
                    }
                }
            }
        }
    }

    cout << "Total Number of Lines: " << numLines << endl;
    cout << "Number of non-blank lines: " << nonBlankLines << endl;
    cout << "Number of Words: " << numWords << endl;
    cout << "Number of Integers: " << numIntegers << endl;
    cout << "Number of Names: " << numNames << endl;

    return 0;
}