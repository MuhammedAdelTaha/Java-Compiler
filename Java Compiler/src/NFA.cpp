#include "NFA.h"
#include <iostream>
#include <stack>

NFA::NFA(std::shared_ptr<State> startState, std::shared_ptr<State> terminalState)
	: m_StartState(startState), m_TerminalState(terminalState)
{
}

std::shared_ptr<State> NFA::getStartState()
{
	return m_StartState;
}

std::shared_ptr<State> NFA::getTerminalState()
{
	return m_TerminalState;
}

void NFA::concatenate(NFA& other)
{
	m_TerminalState->addTransition(other.m_StartState, 0);
	m_TerminalState = other.m_TerminalState;
}

void NFA::unionize(NFA& other)
{	
	std::shared_ptr<State> newStart = std::make_shared<State>();
	std::shared_ptr<State> newTerminal = std::make_shared<State>();

	newStart->addTransition(m_StartState, 0);
	newStart->addTransition(other.m_StartState, 0);
	
	m_TerminalState->addTransition(newTerminal, 0);
	other.m_TerminalState->addTransition(newTerminal, 0);

	m_StartState = newStart;
	m_TerminalState = newTerminal;
}

void NFA::kleeneClosure()
{
	std::shared_ptr<State> newStart = std::make_shared<State>();
	std::shared_ptr<State> newTerminal = std::make_shared<State>();

	newStart->addTransition(m_StartState, 0);
	newStart->addTransition(newTerminal, 0);

	m_TerminalState->addTransition(m_StartState, 0);
	m_TerminalState->addTransition(newTerminal, 0);

	m_StartState = newStart;
	m_TerminalState = newTerminal;
}

void NFA::positiveClosure()
{
	std::shared_ptr<State> newStart = std::make_shared<State>();
	std::shared_ptr<State> newTerminal = std::make_shared<State>();

	newStart->addTransition(m_StartState, 0);

	m_TerminalState->addTransition(m_StartState, 0);
	m_TerminalState->addTransition(newTerminal, 0);

	m_StartState = newStart;
	m_TerminalState = newTerminal;
}

void NFA::traverse()
{
	std::stack<std::pair<std::shared_ptr<State>, int>> s;
	s.push(std::make_pair(m_StartState, 0));

	int index = 0;
	while (!s.empty())
	{
		std::pair<std::shared_ptr<State>, int> state = s.top();
		s.pop();

		for (int i = 0; i < state.second; i++)
		{
			std::cout << "\t";
		}

		std::cout << "State " << state.first->getId() << ":\n";

		for (auto x : state.first->getTransitions())
		{

			for (int i = 0; i < state.second + 1; i++)
			{
				std::cout << "\t";
			}

			std::cout << "t[" << x.first << "]" << ":\n" << std::endl;

			for (auto y : x.second)
			{
				s.push(std::make_pair(y, state.second + 1));
			}
		}
	}
}
