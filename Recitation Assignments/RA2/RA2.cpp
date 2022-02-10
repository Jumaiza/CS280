#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

int main()
{

    char filename[30];
    ifstream inFile;

    cout << "Enter the name of a file to read from: " << endl;
    cin.getline(filename, 30);

    inFile.open(filename);
    if (!inFile.is_open())
    {
        cout << endl
             << "File cannot be opened " << filename << endl;
        return 0;
    }

    int totalLines = 0;
    int commentLines = 0;
    int commandLines = 0;
    int boldLines = 0;
    int italicLines = 0;
    int regularcommands = 0;

    string word;
    while (getline(inFile, word))
    {
        totalLines += 1;

        if (word.substr(0, 1) == "%")
        {

            int regular = word.find("regular");
            int italic = word.find("italic");
            int bold = word.find("bold");

            if (bold > -1)
            {
                boldLines += 1;
            }
            else if (regular > -1)
            {
                regularcommands += 1;
            }
            else if (italic > -1)
            {
                italicLines += 1;
            }
            else
            {
                cout << endl
                     << "Error: Unrecognizable command in line " << totalLines << endl;
            }
            commandLines += 1;
        }

        else if (word.substr(0, 1) == "#")
        {
            commentLines += 1;
        }
    }

    cout << endl
         << "Total lines: " << totalLines << endl;
    cout << "Commented lines: " << commentLines << endl;
    cout << "Command lines: " << commandLines << endl;
    cout << "Bold commands: " << boldLines << endl;
    cout << "Italic commands: " << italicLines << endl;
    cout << "Regular commands: " << regularcommands << endl;

    return 0;
}