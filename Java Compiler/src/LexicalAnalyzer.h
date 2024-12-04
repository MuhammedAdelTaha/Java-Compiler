#pragma once
#include <iostream>
#include <string>
#include "DFA.h"

class LexicalAnalyzer 
{
public:
    LexicalAnalyzer
    (
        const std::string& inputText,
        DFA& dfa,
		std::set<std::string>& keywords,
		std::set<std::string>& punctuations,
        std::map<std::string, int>& tokensPrecedence,
        std::map<std::string, std::set<EpsilonClosure>>& regexEpsilonClosures
    );

    std::pair<std::string, std::string> getNextToken();

private:
    void skipWhitespaces();

private:
	size_t m_CurrentPosition;
    std::string m_InputText;
    EpsilonClosure m_StartClosure;
    std::set<EpsilonClosure> m_TerminalClosures;
    std::map<EpsilonClosure, std::unordered_map<char, EpsilonClosure>> m_Table;
    std::set<std::string> m_Keywords;
    std::set<std::string> m_Punctuations;
    std::map<std::string, int> m_TokensPrecedence;
    std::map<std::string, std::set<EpsilonClosure>> m_RegexEpsilonClosures;
};