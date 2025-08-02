// CFGParser.h
#pragma once

#include "Utilities.h"
#include <map>
#include <vector>
#include <string>

class CFGParser {
public:
        using Production = std::vector<Symbol>;
        using Grammar = std::map<std::string, std::vector<Production>>;

        CFGParser(const Grammar& grammar);

        void eliminateLeftRecursion();
        void leftFactor();
        Grammar getGrammar() const;
private:
        std::string generateNewNonTerminal(const std::string& nonTerminal);
        bool isNonTerminal(const std::string& symbol) const;
        std::vector<std::string> getNonTerminalOrder() const;
        std::vector<std::string> leftFactorNonTerminal(const std::string& nonTerminal);
private:
        Grammar m_Grammar;
};
