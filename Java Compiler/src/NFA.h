#pragma once
#include <unordered_map>
#include <vector>
#include <cstdint>

typedef std::unordered_map<char, std::vector<uint32_t>> StateTransitions;
typedef uint32_t State;
typedef uint8_t Transition;

class NFA
{
public:
	uint32_t addState();
	void addTranstition(State from, State to, Transition transition);
	void setStartState(State state);
	void setTerminalState(State state);
private:
	std::vector<State> m_StartStates;
	std::vector<State> m_TerminalStates;
	std::vector<StateTransitions> m_StatesTransitions;
};
