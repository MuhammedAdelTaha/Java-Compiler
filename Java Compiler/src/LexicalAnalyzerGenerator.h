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

	inline DFA getDFA() const { return m_DFA; }
    inline std::map<std::string, int> getTokensPrecedence() const { return m_TokensPrecedence; }
	inline std::map<std::string, std::set<EpsilonClosure>> getRegexEpsilonClosures() const { return m_RegexEpsilonClosures; }
    inline std::set<std::string> getKeywords() const { return m_Keywords; }
	inline std::set<std::string> getPunctuations() const { return m_Punctuations; }

private:
    void readRulesFromFile();
    void processRegularDefinition(const std::string& line);
    void processRegularExpression(const std::string& line);
    void processKeywords(const std::string& line);
    void processPunctuations(const std::string& line);
    void generateDFA();
    NFA combineNFAs(const std::vector<NFA>& nfas);
	NFA convertDefToNFA(const std::string& def);
    NFA convertRegexToNFA(const std::string& regex);
    NFA convertSymbolToNFA(const std::string& sym);

private:
    DFA m_DFA;
    std::map<std::string, int> m_TokensPrecedence;
	std::map<std::string, std::set<EpsilonClosure>> m_RegexEpsilonClosures;
	
    std::string m_FilePath;
    std::map<std::string, std::string> m_RegularDefinitions;
    std::map<std::string, std::string> m_RegularExpressions;
    std::set<std::string> m_Keywords;
    std::set<std::string> m_Punctuations;
};