#pragma once
#include <map>
#include <set>
#include "NFA.h"

typedef std::set<std::shared_ptr<State>> EpsilonClosure;

class DFA
{
public:
	DFA(NFA& nfa);
	void minmize();
	std::unordered_map<char, std::map<char, char>> cleanTable();
	void drawTable(std::unordered_map<char, std::map<char, char>> cleanTable);
	std::map<EpsilonClosure, std::unordered_map<char, EpsilonClosure>> getTable() { return m_Table; }
private:
	EpsilonClosure getEpsilonClosure(std::shared_ptr<State> initialState);
	void subsetConstruction(std::shared_ptr<State> initialState);
private:
	std::map<EpsilonClosure, std::unordered_map<char, EpsilonClosure>> m_Table;
	std::vector<std::shared_ptr<State>> m_TerminalStates;
};
