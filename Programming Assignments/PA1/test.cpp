#include <iostream>
using namespace std;

int main()
{
    int a = 2;
    int b = 5;

    int *bRef = &b;

    cout << bRef;

    return 0;
}