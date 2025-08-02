#include <iostream>
#include <sstream>
#include <set>
#include "NFA.h"
#include "DFA.h"
#include "Utilities.h"
#include "LexicalAnalyzer.h"
#include "LexicalAnalyzerGenerator.h"
#include "Parser.h"
#include "ParseTableGenerator.h"

void printGrammar(const std::map<std::string, std::vector<std::vector<Symbol>>>& grammar)
{
	std::cout << "GRAMMAR: \n";
	for (const auto& [nonTerminal, productions] : grammar)
	{
		std::cout << nonTerminal << " -> ";
		for (size_t i = 0; i < productions.size(); ++i)
		{
			const auto& prod = productions[i];
			if (prod.empty())
			{
				std::cout << "epsilon"; // Represent epsilon
			}
			else
			{
				for (const auto& symbol : prod)
				{
					std::cout << symbol.name << " ";
				}
			}
			if (i != productions.size() - 1)
			{
				std::cout << "| ";
			}
		}
		std::cout << "\n";
	}
	std::cout << "==========================================\n";
}

int main()
{
	const std::string lexicalRules = "files\\lexical_rules.txt";
	const std::string syntaxRules = "files\\syntax_rules.txt";
	const std::string sourceProgram = "files\\source_program.txt";

	// Step 1: Generating the lexical analyzer from the lexical rules.
	LexicalAnalyzerGenerator lexicalAnalyzerGenerator(lexicalRules);
	const std::string program = readFileToString(sourceProgram);

	DFA dfa = lexicalAnalyzerGenerator.getDFA();
	std::set<std::string> keywords = lexicalAnalyzerGenerator.getKeywords();
	std::set<std::string> punctuations = lexicalAnalyzerGenerator.getPunctuations();
	std::map<std::string, int> tokensPrecedence = lexicalAnalyzerGenerator.getTokensPrecedence();
	std::map<std::string, std::set<EpsilonClosure>> regexEpsilonClosures = lexicalAnalyzerGenerator.getRegexEpsilonClosures();

	LexicalAnalyzer lexicalAnalyzer = LexicalAnalyzer
	(
		program,
		dfa,
		keywords,
		punctuations,
		tokensPrecedence,
		regexEpsilonClosures
	);

	// Step 2: Generating the parser from the syntax rules.
	ParseTableGenerator parseTableGenerator(syntaxRules);
	Symbol startSymbol = parseTableGenerator.getStartSymbol();

	parseTableGenerator.generateFirstTable();
	parseTableGenerator.generateFollowTable(startSymbol);
	parseTableGenerator.generateParseTable();

	printGrammar(parseTableGenerator.getGrammar());
	parseTableGenerator.printParseTable();

	std::vector<std::vector<std::string>> outputs;
	Parser parser(lexicalAnalyzer, parseTableGenerator.getParseTable(), startSymbol);
	while (!parser.isFinished())
		parser.parseNextToken();
}
