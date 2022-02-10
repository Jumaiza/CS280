#include <iostream>
#include <sstream>

using namespace std;

int main()
{
    int x = 4;

    int *y = &x;

    int &yRef = *y;

    int num = *y * yRef;

    cout << num << endl;

    ostringstream os;
    os << "There are ";
    os << 101 << " boxes";
    string combo = os.str();

    cout << combo;

    return 0;
}