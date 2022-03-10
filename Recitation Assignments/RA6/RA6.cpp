#include <iostream>
#include <string>
using namespace std;

int maxValue;

void PrintRevNums(int sum)
{

    // get user input
    int num;
    cin >> num;

    // base case. exit if input is 0
    if (num == 0)
    {
        return;
    }

    // if input is greater than current maximum value, then make this input the new maximum value
    if (num > maxValue)
    {
        maxValue = num;
    }

    // add input to total
    sum += num;

    // call function again recursively
    PrintRevNums(sum);

    // print input and total in reverse order
    cout << num << " Total: " << sum << endl;

    // if we get back to the first element, print max value
    if (sum == num)
    {
        cout << "The maximum value is: " << maxValue << endl;
    }
}

int main()
{

    cout << "Enter any sequence of positive integer numbers, then enter 0 to end: " << endl;
    int sum = 0;
    PrintRevNums(sum);
    return 0;
}