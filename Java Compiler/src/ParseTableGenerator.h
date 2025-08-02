#pragma once
#include <string>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "Utilities.h"


class ParseTableGenerator {
public:
	ParseTableGenerator() = default;
    ParseTableGenerator(const std::string& filePath);

	inline Symbol getStartSymbol() { return m_StartSymbol; }
	inline std::unordered_map < std::string, std::unordered_map<std::string, std::vector<Symbol>>> getParseTable() { return m_ParseTable; }
    void printFirstTable();
    void printFollowTable();
    void printParseTable();
    void generateFirstTable();
    void generateFollowTable(Symbol startSymbol);
    void generateParseTable();
    inline std::map<std::string, std::vector<std::vector<Symbol>>> getGrammar() { return m_Grammar; }

private:
    void readRulesFromFile();
    std::vector<std::vector<Symbol>> getProductions(const std::string& str);
    void addFirst(std::string nonTerminal, std::vector<Symbol>& production);
    void generateFirstSet(std::string nonTerminal, std::vector<std::vector<Symbol>>& production);
    void generateFollowSet(std::unordered_map<std::string, std::vector<std::string>>& dependecnies, std::string nonTerminal, Symbol startSymbol);
    void checkConvergance(std::unordered_map<std::string, std::vector<std::string>>& dependecnies);
    bool propagateValues(const std::unordered_set<std::string>& src, std::unordered_set<std::string>& dst);

private:
	std::string m_FilePath;
	Symbol m_StartSymbol;
    std::map<std::string, std::vector<std::vector<Symbol>>> m_Grammar;
    std::unordered_map<std::string, std::unordered_set<std::string>> m_FirstTable;
    std::unordered_map<std::string, std::unordered_set<std::string>> m_FollowTable;
    std::map<std::vector<Symbol>, std::unordered_set<std::string>> m_ProductionFirstTable;
    std::unordered_map<std::string, std::unordered_map<std::string,std::vector<Symbol>>> m_ParseTable;
};