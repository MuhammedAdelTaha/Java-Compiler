#include <iostream>
#include <memory>
#include <set>
#include "NFA.h"
#include "DFA.h"
#include "LexicalAnalyzerGenerator.h"

int main()
{
	const std::string relativeFilePath = "files\\rules.txt";
	LexicalAnalyzerGenerator generator(relativeFilePath);
	
	auto tokensPrecedence = generator.getTokensPrecedence();
	auto regularDefinitions = generator.getRegularDefinitions();
	auto regularExpressions = generator.getRegularExpressions();
	auto keywords = generator.getKeywords();
	auto punctuations = generator.getPunctuations();

	// Print them
	std::cout << "Tokens Precedence:\n";
	for (const auto& token : tokensPrecedence)
		std::cout << token << std::endl;

	std::cout << "\nRegular Definitions:\n";
	for (const auto& [name, definition] : regularDefinitions)
		std::cout << name << " -> " << definition << std::endl;

	std::cout << "\nRegular Expressions:\n";
	for (const auto& [name, regex] : regularExpressions)
		std::cout << name << " -> " << regex << std::endl;

	std::cout << "\nKeywords:\n";
	for (const auto& keyword : keywords)
		std::cout << keyword << std::endl;

	std::cout << "\nPunctuations:\n";
	for (const auto& punct : punctuations)
		std::cout << punct << std::endl;

	auto tokenDFAs = generator.getTokenDFAs();
	for (auto [name, dfa] : tokenDFAs)
	{
		std::cout << "\nDFA for " << name << ":\n";
		dfa.drawTable(dfa.cleanTable());
	}

	return 0;
}
