#pragma once
#include <set>
#include "State.h"

class NFA
{
public:
	NFA() = default;
	NFA(std::shared_ptr<State> startState, std::set<std::shared_ptr<State>> terminalStates);
	inline std::shared_ptr<State> getStartState() const { return m_StartState; }
	inline std::set<std::shared_ptr<State>> getTerminalStates() const { return m_TerminalStates; }
	void concatenate(NFA& other);
	void unionize(NFA& other);
	void kleeneClosure();
	void positiveClosure();

private:
	std::shared_ptr<State> combineTerminalStates(std::set<std::shared_ptr<State>>& terminalStates);

private:
	std::shared_ptr<State> m_StartState;
	std::set<std::shared_ptr<State>> m_TerminalStates;
};

