#pragma once
#include <string>
#include <map>
#include <vector>
#include "DFA.h"

class LexicalAnalyzerGenerator 
{
public:
    LexicalAnalyzerGenerator() = default;
    LexicalAnalyzerGenerator(const std::string& filePath);

    inline std::vector<std::string> getTokensPrecedence() const { return m_TokensPrecedence; }
	inline std::map<std::string, std::string> getRegularDefinitions() const { return m_RegularDefinitions; }
	inline std::map<std::string, std::string> getRegularExpressions() const { return m_RegularExpressions; }
	inline std::vector<std::string> getKeywords() const { return m_Keywords; }
	inline std::vector<std::string> getPunctuations() const { return m_Punctuations; }
	inline std::map<std::string, DFA> getTokenDFAs() const { return m_TokenDFAs; }

//private:
    void generateDFAs();
    void readRulesFromFile();
    void processRegularDefinition(const std::string& line);
    void processRegularExpression(const std::string& line);
    void processKeywords(const std::string& line);
    void processPunctuations(const std::string& line);
	NFA convertDefToNFA(const std::string& def);
    NFA convertRegexToNFA(const std::string& regex);
    NFA convertSymbolToNFA(const std::string& sym);

private:
	std::string m_FilePath;
    std::vector<std::string> m_TokensPrecedence;
    std::map<std::string, std::string> m_RegularDefinitions;
    std::map<std::string, std::string> m_RegularExpressions;
    std::vector<std::string> m_Keywords;
    std::vector<std::string> m_Punctuations;
    std::map<std::string, DFA> m_TokenDFAs;
};