#include <string>
#include <vector>
#include <stack>
#include <sstream>
#include <iostream>
#include "RegexHandler.h"

std::string RegexHandler::infixToPostfix(const std::string& regex) 
{
    std::stack<char> operators;
    std::ostringstream postfixExp;

	char c = 0;
    for (size_t i = 0; i < regex.length(); i++) 
    {
        c = regex[i];
			
        // Handle special characters.
        if (c == '\\' && i + 1 < regex.length()) 
        {
            postfixExp << regex.substr(i++, 2) << " ";
            continue;
        }
            
        // Handle operands.
        size_t j = i;
        for (; j < regex.length() && isOperand(regex[j]); j++)
            postfixExp << regex[j];
            
        // If there is at least one operand character, then add a whitespace at the end.
        if (j != i)
            postfixExp << " ";

        // End of regex.
        if (j == regex.length())
            break;

        // The current character is an operator.
        i = j;
		c = regex[i];

        // Handle operators.
        if (c == '(')
        {
            operators.push(c);
        }
        else if (c == ')') 
        {
            for (; !operators.empty() && operators.top() != '('; operators.pop())
				postfixExp << operators.top() << " ";

            if (!operators.empty() && operators.top() == '(')
                operators.pop();
        }
        else 
        {
            for (; !operators.empty() && operators.top() != '(' && getPrecedence(operators.top()) >= getPrecedence(c); operators.pop())
				postfixExp << operators.top() << " ";
            operators.push(c);
        }
    }

    // Add remaining operators
    for (; !operators.empty(); operators.pop())
		postfixExp << operators.top() << " ";

    return postfixExp.str();
}

bool RegexHandler::isOperator(char c) 
{
    return c == '*' || c == '+' || c == '.' || c == '|' || c == '(' || c == ')';
}

bool RegexHandler::isOperand(char c) 
{
	return !isOperator(c);
}

int RegexHandler::getPrecedence(char op) 
{
    switch (op) {
        case '(': case ')': return 4;
        case '*': case '+': return 3;
        case '.': return 2;
        case '|': return 1;
        default: return 0;
    }
}