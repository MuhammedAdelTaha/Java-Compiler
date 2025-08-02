#include "Parser.h"
#include <iostream>
#include <format>

Parser::Parser(LexicalAnalyzer& lexicalAnalyzer, const std::unordered_map<std::string, std::unordered_map<std::string, std::vector<Symbol>>>& parseTable, Symbol startSymbol)
	: m_LexicalAnalyzer(lexicalAnalyzer), m_ParseTable(parseTable), m_Stack(std::stack<Symbol>()), m_Finished(false)
{
	m_Stack.push({ .type = Symbol::Type::Terminal, .name = "$" });
	m_Stack.push(startSymbol);
	m_Outputs.push_back(startSymbol.name);
	std::cout << m_Outputs[m_Outputs.size() - 1] << std::endl;
}

void Parser::parseNextToken()
{
	std::string token = m_LexicalAnalyzer.getNextToken().second;

	while (true)
	{
		Symbol symbol = m_Stack.top();
		if (symbol.type == Symbol::Type::NonTerminal)
		{
			std::vector<Symbol> production = getProduction(symbol.name, token);

			// check for empty production (error)
			if (production.size() == 0)
			{
				std::cerr << std::format("ERROR: no production for {} with input {} could be found. Ignoring input {}.", symbol.name, token, token) << std::endl;
				return;
			}
			
			// check for synch
			if (production[0].name == "\\S")
			{
				// synch
				std::cerr << std::format("ERROR: encountered synch symbol for {} with input {}. Popping {} from stack.", symbol.name, token, symbol.name) << std::endl;
				std::string newProduction = m_Outputs[m_Outputs.size() - 1];
				auto first = newProduction.begin() + newProduction.find(symbol.name);
				auto last = newProduction.begin() + newProduction.find(symbol.name) + symbol.name.size();
				m_Outputs.push_back(newProduction.replace(first, last, "\b"));
				std::cout << m_Outputs[m_Outputs.size() - 1] << std::endl;
				m_Stack.pop();
				continue;
			}

			// insert production into outputs vector
			std::string output;
			for (auto symbol : production)
			{
				output += symbol.name;
				output += " ";
			}
			output.erase(output.size() - 1, 1);

			std::string newProduction = m_Outputs[m_Outputs.size() - 1];
			auto first = newProduction.begin() + newProduction.find(symbol.name);
			auto last = newProduction.begin() + newProduction.find(symbol.name) + symbol.name.size();

			if (output == "\\L")
			{
				output = "\b";
			}

			m_Outputs.push_back(newProduction.replace(first, last, output));
			std::cout << m_Outputs[m_Outputs.size() - 1] << std::endl;

			m_Stack.pop();

			// push symbols onto stack in reverse
			for (int i = production.size() - 1; i >= 0; i--)
				m_Stack.push(production[i]);
		}
		else if (symbol.type == Symbol::Type::Terminal)
		{
			if (symbol.name == "\\L")
			{
				m_Stack.pop();
			}
			else if (symbol.name == "$")
			{
				m_Finished = true;
				m_Stack.pop();
				return;
			}
			else if (symbol.name == token)
			{
				m_Stack.pop();
				return;
			}
			else
			{
				// throw error (insert symbol into input and match)
				std::cerr << std::format("ERROR: Terminal symbol {} in input mismatched with stack terminal symbol {}. Inserting {} into input.", token, symbol.name, symbol.name) << std::endl;
				m_Stack.pop();
				return;
			}
		}
	}
}

std::vector<Symbol> Parser::getProduction(std::string& nonTerminal, std::string& terminal)
{
	return m_ParseTable[nonTerminal][terminal];
}
