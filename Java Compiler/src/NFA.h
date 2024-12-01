#pragma once
#include "State.h"

class NFA
{
public:
	NFA() = default;
	NFA(std::shared_ptr<State> startState, std::shared_ptr<State> terminalState);
	inline std::shared_ptr<State> getStartState() const { return m_StartState; }
	inline std::shared_ptr<State> getTerminalState() const { return m_TerminalState; }
	void concatenate(NFA& other);
	void unionize(NFA& other);
	void unionize(NFA& other, uint32_t id1, uint32_t id2);
	void kleeneClosure();
	void kleeneClosure(uint32_t id1, uint32_t id2);
	void positiveClosure();
	void traverse();

private:
	std::shared_ptr<State> m_StartState;
	std::shared_ptr<State> m_TerminalState;
};

