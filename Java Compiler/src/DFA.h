#pragma once
#include <map>
#include <set>
#include "NFA.h"

typedef std::set<std::shared_ptr<State>> EpsilonClosure;

class DFA
{
public:
	DFA(NFA& nfa);
	void minimize();
	std::unordered_map<char, std::map<char, char>> cleanTable();
	void drawTable(std::unordered_map<char, std::map<char, char>> cleanTable);
	std::map<EpsilonClosure, std::unordered_map<char, EpsilonClosure>> getTable() { return m_Table; }
private:
	std::set<std::set<EpsilonClosure>> initializePartitions();
	EpsilonClosure getEpsilonClosure(std::shared_ptr<State> initialState);
	std::set<std::set<EpsilonClosure>> splitPartition(std::set<EpsilonClosure> partition);
	void subsetConstruction(std::shared_ptr<State> initialState, std::shared_ptr<State> terminalState);
private:
	std::map<EpsilonClosure, std::unordered_map<char, EpsilonClosure>> m_Table;
	std::set<EpsilonClosure> m_TerminalClosures;
};
