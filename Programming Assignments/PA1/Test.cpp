#include <iostream>
using namespace std;

int main()
{

    int num = 1;

    while (true)
    {
        cout << "after while loop before switch" << endl;

        switch (num)
        {
        case 1:
            cout << "num is 1" << endl;
            num = 3;
            continue;
        case 2:
            cout << "num is 2" << endl;
        case 3:
            cout << "num is 3" << endl;
            num = 4;
            continue;
        case 4:
            cout << "num is 4" << endl;
            break;
        }

        cout << "bottom of while" << endl;
        break;
    }

    cout << "exited while loop";
    return 0;
}