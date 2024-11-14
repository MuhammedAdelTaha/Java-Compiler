#pragma once
#include "State.h"

class NFA
{
public:
	NFA(std::shared_ptr<State> startState, std::shared_ptr<State> terminalState);
	std::shared_ptr<State> getStartState();
	std::shared_ptr<State> getTerminalState();
	void concatenate(NFA& other);
	void unionize(NFA& other);
	void kleeneClosure();
	void positiveClosure();
	void traverse();
private:
	std::shared_ptr<State> m_StartState;
	std::shared_ptr<State> m_TerminalState;
};
