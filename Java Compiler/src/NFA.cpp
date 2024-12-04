#include <iostream>
#include <stack>
#include "NFA.h"

NFA::NFA(std::shared_ptr<State> startState, std::set<std::shared_ptr<State>> terminalStates)
	: m_StartState(startState), m_TerminalStates(terminalStates)
{
}

std::shared_ptr<State> NFA::combineTerminalStates(std::set<std::shared_ptr<State>>& terminalStates)
{
	if (terminalStates.size() == 1)
		return *terminalStates.begin();

	std::shared_ptr<State> combinedTerminalState = std::make_shared<State>();
	for (auto terminalState : terminalStates)
		terminalState->addTransition(combinedTerminalState, 0);

	return combinedTerminalState;
}

void NFA::concatenate(NFA& other)
{
	std::shared_ptr<State> combinedTerminalState = combineTerminalStates(m_TerminalStates);

	for (auto& [c, states] : other.m_StartState->getTransitions())
		combinedTerminalState->addTransitions(states, c);

	m_TerminalStates = other.m_TerminalStates;
}

void NFA::unionize(NFA& other)
{	
	std::shared_ptr<State> newStart = std::make_shared<State>();
	std::shared_ptr<State> newTerminal = std::make_shared<State>();

	std::shared_ptr<State> combinedTerminalState1 = combineTerminalStates(m_TerminalStates);
	std::shared_ptr<State> combinedTerminalState2 = combineTerminalStates(other.m_TerminalStates);

	newStart->addTransition(m_StartState, 0);
	newStart->addTransition(other.m_StartState, 0);
	
	combinedTerminalState1->addTransition(newTerminal, 0);
	combinedTerminalState2->addTransition(newTerminal, 0);

	m_StartState = newStart;
	m_TerminalStates = { newTerminal };
}

void NFA::kleeneClosure()
{
	std::shared_ptr<State> newStart = std::make_shared<State>();
	std::shared_ptr<State> newTerminal = std::make_shared<State>();
	std::shared_ptr<State> combinedTerminalState = combineTerminalStates(m_TerminalStates);

	newStart->addTransition(m_StartState, 0);
	newStart->addTransition(newTerminal, 0);

	combinedTerminalState->addTransition(m_StartState, 0);
	combinedTerminalState->addTransition(newTerminal, 0);

	m_StartState = newStart;
	m_TerminalStates = { newTerminal };
}

void NFA::positiveClosure()
{
	std::shared_ptr<State> newStart = std::make_shared<State>();
	std::shared_ptr<State> newTerminal = std::make_shared<State>();
	std::shared_ptr<State> combinedTerminalState = combineTerminalStates(m_TerminalStates);

	newStart->addTransition(m_StartState, 0);

	combinedTerminalState->addTransition(m_StartState, 0);
	combinedTerminalState->addTransition(newTerminal, 0);

	m_StartState = newStart;
	m_TerminalStates = { newTerminal };
}
