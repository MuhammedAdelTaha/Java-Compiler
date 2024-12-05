#include <iostream>
#include <stack>
#include "DFA.h"

DFA::DFA(NFA& nfa)
	: m_NFATerminalStates(nfa.getTerminalStates())
{
	subsetConstruction(nfa.getStartState(), nfa.getTerminalStates());
	minimize();
}

// Returns all the states that could be reached from a given state using only epsilon transitions.
EpsilonClosure DFA::getEpsilonClosure(std::shared_ptr<State> startState)
{
	EpsilonClosure epsilonClosure;

	// Initialize the stack with the given start state.
	std::stack<std::shared_ptr<State>> stack;
	stack.push(startState);

	while (!stack.empty())
	{
		// If the epsilon closure already contains the state then continue to the next state on the stack.
		std::shared_ptr<State> state = stack.top();
		stack.pop();
		if (epsilonClosure.contains(state))
			continue;

		// Add the state to the epsilon closure.
		epsilonClosure.insert(state);

		// Loop over the states that could be reached via epsilon transitions and push them into the stack.
		for (auto neighboringState : state->getTransitions()[0])
			stack.push(neighboringState);
	}

	return epsilonClosure;
}

// Constructs the transitions table and the terminal states of a DFA from the start and final states of an NFA.
void DFA::subsetConstruction(std::shared_ptr<State> startState, std::set<std::shared_ptr<State>> terminalStates)
{
	// The start state of the DFA is the epsilon closure of the given start state in the NFA.
	m_StartClosure = getEpsilonClosure(startState);

	// A set to store all the epsilon closures that we have visited so far.
	std::set<EpsilonClosure> epsilonClosures;

	// Initialize the stack with the epsilon closure of the given start state.
	std::stack<EpsilonClosure> stack;
	stack.push(m_StartClosure);

	while (!stack.empty())
	{
		// Get the epsilon closure that is on top of the stack and insert it into our epsilon closures.
		auto epsilonClosure = stack.top();
		stack.pop();
		epsilonClosures.insert(epsilonClosure);

		// If the epsilon closure contains a terminal state, then add it to the terminal closures.
		for (auto state : epsilonClosure)
		{
			if (terminalStates.contains(state))
			{
				m_TerminalClosures.insert(epsilonClosure);
				break;
			}
		}
		
		// Get all possible transitions of the current epsilon closure.
		std::map<char, EpsilonClosure> transitions;
		for (auto state : epsilonClosure)
		{
			for (auto [input, neighboringStates] : state->getTransitions())
			{
				if (input == 0)
					continue;

				for (auto neighboringState : neighboringStates)
				{
					EpsilonClosure childClosure = getEpsilonClosure(neighboringState);
					transitions[input].insert(childClosure.begin(), childClosure.end());
				}
			}
		}

		// Iterate over all the transitions and add them to the DFA table.
		for (auto [input, closure] : transitions)
		{
			m_Table[epsilonClosure][input] = closure;
			if (epsilonClosures.contains(closure))
				continue;

			epsilonClosures.insert(closure);
			if (!m_Table.contains(closure))
				m_Table.emplace(closure, std::unordered_map<char, EpsilonClosure>());
			stack.push(closure);
		}
	}
}

// Returns a cleaner transition table that uses symbols, which represent every closure.
std::unordered_map<std::string, std::map<char, std::string>> DFA::cleanTable() {
	std::map<EpsilonClosure, std::string> mappedStates;
	char x = 'A';
	for (auto [fromClosure, transitions] : m_Table)
	{
		if (!mappedStates.contains(fromClosure))
			mappedStates[fromClosure] = std::string(1, x++);

		for (auto [_, toClosure] : transitions)
			if (!mappedStates.contains(toClosure))
				mappedStates[toClosure] = std::string(1, x++);
	}

	// Put (start) beside the start state.
	mappedStates[m_StartClosure] += " (start)";

	// Put (terminal) beside the terminal states.
	for (auto terminalClosure : m_TerminalClosures)
		mappedStates[terminalClosure] += " (terminal)";

	std::unordered_map<std::string, std::map<char, std::string>> cleanedTable;
	for (auto [fromClosure, transitions] : m_Table)
	{
		if (transitions.size() == 0)
		{
			cleanedTable.emplace(mappedStates[fromClosure], std::map<char, std::string>());
		}

		for (auto [t, toClosure] : transitions)
		{
			cleanedTable[mappedStates[fromClosure]][t] = mappedStates[toClosure];
		}
	}

	return cleanedTable;
}

// Draws the transition table.
void DFA::drawTable(std::unordered_map<std::string, std::map<char, std::string>> table){
	std::cout << "------------- Transition Table -------------\n";

	for (auto [fromState, transitions] : table)
	{
		if (transitions.size() == 0)
			continue;
		
		std::cout << fromState <<  ":\n";
		for (auto [t, toState] : transitions)
		{
			std::cout << "-(" << t << ")-> " << toState;
			std::cout << std::endl;
		}
		std::cout << "-----------------\n";
	}
}

// Initializes the partitions with terminal and non terminal partitions.
std::set<std::set<EpsilonClosure>> DFA::initializePartitions() {
	std::set<EpsilonClosure> nonTerminalPartition;
	std::set<std::set<EpsilonClosure>> partitions;

	// Terminal partitions contains all the closures that are terminal.
	// Every partition of the terminal partitions contains the closures that have the same terminal states from the original NFA.
	for (auto nfaTerminalState : m_NFATerminalStates)
	{
		std::set<EpsilonClosure> terminalPartition;
		for (auto [closure, _] : m_Table)
		{
			if (closure.contains(nfaTerminalState))
				terminalPartition.insert(closure);
		}
		partitions.insert(terminalPartition);
	}

	// Non-terminal partition contains all the closures that are not terminal.
	for (auto [closure, _] : m_Table)
		if (!m_TerminalClosures.contains(closure))
			nonTerminalPartition.insert(closure);
	
	if (!nonTerminalPartition.empty())
		partitions.insert(nonTerminalPartition);

	return partitions;
}

// Splits a partition into sub-partitions.
std::set<std::set<EpsilonClosure>> DFA::splitPartition(std::set<EpsilonClosure> partition, std::map<EpsilonClosure, EpsilonClosure> representativeClosure)
{
	// If partition is empty or has only one closure, return it as is.
	if (partition.size() <= 1)
		return { partition };

	std::map<EpsilonClosure, std::map<char, EpsilonClosure>> transitions;

	for (auto closure : partition)
	{
		transitions[closure] = std::map<char, EpsilonClosure>();
		for (auto [t, nextClosure] : m_Table[closure])
		{
			transitions[closure][t] = representativeClosure[nextClosure];
		}
	}

	std::map<std::map<char, EpsilonClosure>, std::set<EpsilonClosure>> unique;
	for (auto& [closure, t] : transitions)
	{
		if (!unique.contains(t))
		{
			unique[t] = { closure };
		}
		else
		{
			unique[t].emplace(closure);
		}
	}

	std::set<std::set<EpsilonClosure>> split;
	for (auto [_, closureSet] : unique)
	{
		split.insert(closureSet);
	}

	return split;
}

// Minimizes the DFA.
void DFA::minimize()
{
	// If the table is empty or has only one state, no minimization is needed.
	if (m_Table.size() <= 1)
		return;

	std::set<std::set<EpsilonClosure>> partitions = initializePartitions();

	// Iterate until there are no more partition splits.
	bool isPartitionsChanged;
	do
	{
		isPartitionsChanged = false;
		std::set<std::set<EpsilonClosure>> newPartitions;
		std::map<EpsilonClosure, EpsilonClosure> representativeClosure;

		for (auto& partition : partitions)
		{
			EpsilonClosure representative = *partition.begin();
			for (auto& closure : partition)
				representativeClosure[closure] = representative;
		}

		for (auto partition : partitions)
		{
			// If the partition has less than one closure, then insert it as it is (no splits are possible).
			if (partition.size() <= 1)
			{
				newPartitions.insert(partition);
				continue;
			}

			// Split the partition into sub-partitions if possible
			std::set<std::set<EpsilonClosure>> splittedPartitions = splitPartition(partition, representativeClosure);

			if (splittedPartitions.size() > 1)
			{
				isPartitionsChanged = true;
				newPartitions.insert(splittedPartitions.begin(), splittedPartitions.end());
			}
			else
				newPartitions.insert(partition);
		}

		partitions = newPartitions;
	} while (isPartitionsChanged);

	std::map<EpsilonClosure, std::unordered_map<char, EpsilonClosure>> newTable;
	std::set<EpsilonClosure> newTerminalClosures;

	std::map<EpsilonClosure, EpsilonClosure> representativeClosure;
	for (auto& partition : partitions)
	{
		EpsilonClosure representative = *partition.begin();
		for (auto& closure : partition)
			representativeClosure[closure] = representative;
	}

	for (auto partition : partitions)
	{
		EpsilonClosure representative = *partition.begin();

		// Copy the transitions of the representative closure from the original table to the new one.
		std::unordered_map<char, EpsilonClosure> transitions = m_Table[representative];

		for (auto [input, toClosure] : transitions)
		{
			transitions[input] = representativeClosure[toClosure];
		}

		newTable[representative] = transitions;

		// If the partition has a terminal closure, then insert the representative closure of the partition into the terminal closures.
		for (auto terminalClosure : m_TerminalClosures)
		{
			if (partition.contains(terminalClosure))
			{
				newTerminalClosures.insert(representative);
				break;
			}
		}
	}

	// Update the table and the terminal closures.
	m_Table = newTable;
	m_TerminalClosures = std::set<EpsilonClosure>(newTerminalClosures.begin(), newTerminalClosures.end());
}

//int main() {
//	std::shared_ptr<State> s0 = std::make_shared<State>();
//	std::shared_ptr<State> s1 = std::make_shared<State>();
//	s0->addTransition(s1, '+');
//	NFA nfa1(s0, s1);
//	
//	std::shared_ptr<State> s3 = std::make_shared<State>();
//	std::shared_ptr<State> s4 = std::make_shared<State>();
//	s3->addTransition(s4, '-');
//	NFA nfa2(s3, s4);
//
//	nfa2.unionize(nfa1);
//
//	DFA dfa(nfa2);
//	dfa.drawTable(dfa.cleanTable());
//}
