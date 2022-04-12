#include <iostream>
#include <string>
#include <vector>
using namespace std;

#include "val.h"

void ExecuteOp(const Value &val1, const Value &val2)
{
	cout << val1 << " + " << val2 << " is " << val1 + val2 << endl;
	cout << val1 << " * " << val2 << " is " << val1 * val2 << endl;
	cout << val1 << " == " << val2 << " is " << (val1 == val2) << endl;
}

int main(int argc, char *argv[])
{
	Value errorVal;
	Value intVal(5);
	float num = 6.25;
	Value floatVal(num);
	string str1 = "hello";
	bool flag = false;
	Value strVal1(str1);
	Value boolVal(flag);
	string str2 = "World";
	Value strVal2(str2);
	vector<Value> vals({errorVal, intVal, floatVal, strVal1, boolVal, strVal2});

	cout << "errorVal=" << errorVal << endl;
	cout << "intVal=" << intVal << endl;
	cout << "floatVal=" << floatVal << endl;
	cout << "strVal1=" << strVal1 << endl;
	cout << "boolVal=" << boolVal << endl;
	cout << "strVal2=" << strVal2 << endl;

	for (int i = 0; i < vals.size(); i++)
		for (int j = 0; j < vals.size(); j++)
			if (i != j)
				ExecuteOp(vals[i], vals[j]);

	return 0;
}
