#pragma once
#include <unordered_map>
#include <string>
#include <stack>
#include <vector>
#include "LexicalAnalyzer.h"
#include "Utilities.h"

class Parser
{
public:
	Parser(LexicalAnalyzer& lexicalAnalyzer, const std::unordered_map<std::string, std::unordered_map<std::string, std::vector<Symbol>>>& parseTable, Symbol startSymbol);
	void parseNextToken();
	bool isFinished() { return m_Finished; }
	std::vector<std::string> getOutputs() { return m_Outputs; }
private:
	std::vector<Symbol> getProduction(std::string& nonTerminal, std::string& terminal);
private:
	std::unordered_map<std::string, std::unordered_map<std::string, std::vector<Symbol>>> m_ParseTable;
	LexicalAnalyzer m_LexicalAnalyzer;
	std::stack<Symbol> m_Stack;
	std::vector<std::string> m_Outputs;
	bool m_Finished;
};
