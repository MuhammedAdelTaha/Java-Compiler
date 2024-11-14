#include "NFA.h"
#include <functional>
#include <stdexcept>

State NFA::addState()
{
	m_StatesTransitions.push_back(StateTransitions());
	return m_StatesTransitions.size() - 1;
}

void NFA::addTranstition(State from, State to, Transition transition)
{
	if (from >= m_StatesTransitions.size() || to >= m_StatesTransitions.size())
		throw std::exception("State not in NFA");

	m_StatesTransitions[from][transition].push_back(to);
}

void NFA::setStartState(State state)
{
	if (state >= m_StatesTransitions.size())
		throw std::exception("State not in NFA");

	m_StartStates.push_back(state);
}

void NFA::setTerminalState(State state)
{
	if (state >= m_StatesTransitions.size())
		throw std::exception("State not in NFA");

	m_TerminalStates.push_back(state);
}
