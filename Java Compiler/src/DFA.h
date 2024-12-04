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
	void minimize(std::map<std::string, std::set<std::shared_ptr<State>>> regexTerminalStates);
	std::unordered_map<std::string, std::map<char, std::string>> cleanTable();
	void drawTable(std::unordered_map<std::string, std::map<char, std::string>> table);
	inline EpsilonClosure getStartClosure() const { return m_StartClosure; }
	inline std::set<EpsilonClosure> getTerminalClosures() const { return m_TerminalClosures; }
	inline std::map<EpsilonClosure, std::unordered_map<char, EpsilonClosure>> getTable() const { return m_Table; }

private:
	std::set<std::set<EpsilonClosure>> initializePartitions(std::map<std::string, EpsilonClosure> regexTerminalStates);
	EpsilonClosure getEpsilonClosure(std::shared_ptr<State> startState);
	std::set<std::set<EpsilonClosure>> splitPartition(std::set<EpsilonClosure> partition, std::map<EpsilonClosure, EpsilonClosure> representativeClosure);
	void subsetConstruction(std::shared_ptr<State> startState, std::set<std::shared_ptr<State>> terminalStates);

private:
	EpsilonClosure m_StartClosure;
	std::set<EpsilonClosure> m_TerminalClosures;
	std::map<EpsilonClosure, std::unordered_map<char, EpsilonClosure>> m_Table;
};
