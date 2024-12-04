#include <iostream>
#include <sstream>
#include <set>
#include "NFA.h"
#include "DFA.h"
#include "Utilities.h"
#include "LexicalAnalyzer.h"
#include "LexicalAnalyzerGenerator.h"

int main()
{
	const std::string rulesFilePath = "files\\rules.txt";
	const std::string programFilePath = "files\\program.txt";

	LexicalAnalyzerGenerator generator(rulesFilePath);
	const std::string program = readFileToString(programFilePath);
	
	DFA dfa = generator.getDFA();
	std::set<std::string> keywords = generator.getKeywords();
	std::set<std::string> punctuations = generator.getPunctuations();
	std::map<std::string, int> tokensPrecedence = generator.getTokensPrecedence();
	std::map<std::string, std::set<EpsilonClosure>> regexEpsilonClosures = generator.getRegexEpsilonClosures();
	
	LexicalAnalyzer lexicalAnalyzer
	(
		program,
		dfa,
		keywords,
		punctuations,
		tokensPrecedence,
		regexEpsilonClosures
	);

	std::pair<std::string, std::string> token;
	std::map<std::string, std::string> memo;
	std::ostringstream outputProgram;

	while ((token = lexicalAnalyzer.getNextToken()) != std::make_pair("", ""))
	{
		if (token.first == "error" && token.second == "error")
			continue;

		if (memo.count(token.first))
		{
			outputProgram << memo[token.first] << ' ';
			continue;
		}

		if (token.second != "")
		{
			memo[token.first] = token.second;
			outputProgram << token.second << ' ';
		}
		else
			outputProgram << token.first << ' ';
	}

	std::cout << outputProgram.str() << std::endl;

	return 0;
}
