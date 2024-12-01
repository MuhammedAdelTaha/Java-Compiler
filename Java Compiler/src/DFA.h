#pragma once
#include <map>
#include <set>
#include "NFA.h"

typedef std::set<std::shared_ptr<State>> EpsilonClosure;

class DFA
{
public:
	DFA() = default;
	DFA(NFA& nfa);
	void minimize();
	std::unordered_map<char, std::map<char, char>> cleanTable();
	void drawTable(std::unordered_map<char, std::map<char, char>> table);
	inline EpsilonClosure getStartClosure() const { return m_StartClosure; }
	inline std::set<EpsilonClosure> getTerminalClosures() const { return m_TerminalClosures; }
	inline std::map<EpsilonClosure, std::unordered_map<char, EpsilonClosure>> getTable() const { return m_Table; }

private:
	std::set<std::set<EpsilonClosure>> initializePartitions();
	EpsilonClosure getEpsilonClosure(std::shared_ptr<State> startState);
	std::set<std::set<EpsilonClosure>> splitPartition(std::set<EpsilonClosure> partition);
	void subsetConstruction(std::shared_ptr<State> startState, std::shared_ptr<State> terminalState);

private:
	EpsilonClosure m_StartClosure;
	std::set<EpsilonClosure> m_TerminalClosures;
	std::map<EpsilonClosure, std::unordered_map<char, EpsilonClosure>> m_Table;
};
