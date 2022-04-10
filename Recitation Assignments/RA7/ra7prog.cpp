#include <iostream>
#include <string>
#include <stack>

using namespace std;

int findPrecedence(char character)
{
	if (character == '+' || character == '-')
		return 1;
	if (character == '*' || character == '/')
		return 2;
	if (character == '^')
		return 3;
	else
		return 0;
}

void infToPostfix(string instr)
{
	string postFix;
	stack<char> stack;
	for (int i = 0; i < instr.length(); i++)
	{
		char current = instr[i];

		if (isspace(current))
		{
			continue;
		}

		if (isalpha(current))
		{
			postFix += current;
		}
		else if (stack.empty())
		{
			stack.push(current);
		}
		else if (current == '{' || current == '(' || current == '[')
		{
			stack.push(current);
		}
		else if (findPrecedence(current) > findPrecedence(stack.top()))
		{
			stack.push(current);
		}
		else if (current == ')')
		{
			while (stack.top() != '(')
			{
				postFix += stack.top();
				stack.pop();
			}
		}
		else
		{
			postFix += stack.top();
			stack.pop();
			stack.push(current);
		}
	}

	if (!stack.empty())
	{
		while (!stack.empty())
		{
			if (stack.top() == '+' || stack.top() == '-' || stack.top() == '*' || stack.top() == '/' || stack.top() == '^')
			{
				postFix += stack.top();
				stack.pop();
			}
			else
			{
				stack.pop();
			}
		}

		for (int i = 0; i < postFix.length(); i++)
		{
			cout << postFix[i] << " ";
		}
		cout << endl;
	}
}

int main()
{
	string instr;
	cout << "Please enter an infix notation expression using single lowercase characters:" << endl;
	getline(cin, instr);
	infToPostfix(instr);

	return 0;
}