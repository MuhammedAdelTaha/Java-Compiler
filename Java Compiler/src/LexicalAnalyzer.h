#pragma once
#include <string>
#include <map>
#include "DFA.h"

class LexicalAnalyzer 
{
public:
    LexicalAnalyzer(std::string& inputText, std::map<std::string, DFA>& tokenDFAs);
    //std::pair<std::string, std::string> getNextToken();

private:
    bool matchesTokenDFA(DFA& dfa, std::string& substring);
    void skipWhitespaces();

private:
    std::string input;
    std::map<std::string, DFA> tokenDFAs;
    size_t currentPosition;
};