#pragma once
#include <string>

class RegexHandler
{
public:
	std::string infixToPostfix(const std::string& regex);

private:
	bool isOperator(char c) const;
	bool isOperand(char c) const;
	int getPrecedence(char op) const;
};