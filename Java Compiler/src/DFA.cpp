#include <iostream>
#include <stack>
#include "DFA.h"

DFA::DFA(NFA& nfa)
{
	subsetConstruction(nfa.getStartState());
}

EpsilonClosure DFA::getEpsilonClosure(std::shared_ptr<State> initialState)
{
	EpsilonClosure set;
	std::stack<std::shared_ptr<State>> stack;
	stack.push(initialState);

	while (!stack.empty())
	{
		std::shared_ptr<State> state = stack.top();
		stack.pop();
		if (set.contains(state))
			continue;

		set.insert(state);

		for (auto neighbouringState : state->getTransitions()[0])
			stack.push(neighbouringState);
	}

	return set;
}

void DFA::subsetConstruction(std::shared_ptr<State> initialState)
{
	std::set<EpsilonClosure> set;
	std::stack<EpsilonClosure> stack;
	stack.push(getEpsilonClosure(initialState));

	while (!stack.empty())
	{
		auto epsilonClosure = stack.top();
		stack.pop();

		set.insert(epsilonClosure);

		std::map<char, EpsilonClosure> mamaNagia;
		for (auto state : epsilonClosure)
		{
			for (auto& [transition, neighbouringStates] : state->getTransitions())
			{
				if (transition == 0)
					continue;

				for (auto neighbouringState : neighbouringStates)
				{
					EpsilonClosure childClosure = getEpsilonClosure(neighbouringState);
					mamaNagia[transition].insert(childClosure.begin(), childClosure.end());
				}
			}
		}

		for (auto [transition, closure] : mamaNagia)
		{
			m_Table[epsilonClosure][transition] = closure;
			if (set.contains(closure))
				continue;

			set.insert(closure);
			stack.push(closure);
		}
	}
}

std::unordered_map<char, std::map<char, char>> DFA::cleanTable() {
	std::map<EpsilonClosure, char> mappedStates;
	char x = 'A';
	for (auto [clousre, _] : m_Table)
	{
		if (mappedStates.contains(clousre))
			continue;

		mappedStates[clousre] = x;
		x++;
	}

	std::unordered_map<char, std::map<char, char>> cleanedTable;
	for (auto [fromClousre, Transitions] : m_Table)
		for (auto [t, toClousre] : Transitions)
			cleanedTable[mappedStates[fromClousre]][t] = mappedStates[toClousre];

	return cleanedTable;
}

void DFA::drawTable(std::unordered_map<char, std::map<char, char>> cleanTable){
	std::cout << "------------- Transition Table -------------\n";

	for (auto& [fromState, transitions] : cleanTable)
	{
		std::cout << fromState <<  ":\n";
		for (auto& [t, toState] : transitions)
		{
			std::cout << "-(" << t << ")-> " << toState;
			std::cout << std::endl;
		}
		std::cout << "-----------------\n";
	}
}

void DFA::minmize()
{

}
