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
	std::unordered_map<std::string, std::map<char, std::string>> cleanTable();
	void drawTable(std::unordered_map<std::string, std::map<char, std::string>> table);
	inline EpsilonClosure getStartClosure() const { return m_StartClosure; }
	inline std::set<EpsilonClosure> getTerminalClosures() const { return m_TerminalClosures; }
	inline std::map<EpsilonClosure, std::unordered_map<char, EpsilonClosure>> getTable() const { return m_Table; }

private:
	EpsilonClosure getEpsilonClosure(std::shared_ptr<State> startState);
	void subsetConstruction(std::shared_ptr<State> startState, std::set<std::shared_ptr<State>> terminalStates);
	void minimize();
	std::set<std::set<EpsilonClosure>> initializePartitions();
	std::set<std::set<EpsilonClosure>> splitPartition(std::set<EpsilonClosure> partition, std::map<EpsilonClosure, EpsilonClosure> representativeClosure);

private:
	std::set<std::shared_ptr<State>> m_NFATerminalStates;

	EpsilonClosure m_StartClosure;
	std::set<EpsilonClosure> m_TerminalClosures;
	std::map<EpsilonClosure, std::unordered_map<char, EpsilonClosure>> m_Table;
};
