#pragma once
#include <string>

class RegexHandler
{
public:
	static std::string infixToPostfix(const std::string& regex);

private:
	static bool isOperator(char c);
	static bool isOperand(char c);
	static int getPrecedence(char op);
};