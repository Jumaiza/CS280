#include <iostream>
using namespace std;

int main()
{

    cout << "Welcome to CS 280.\n";

    cout << "What is your first and last names?\n";

    string name;
    getline(std::cin, name);

    cout << "Hello, " << name;

    return 0;
}