#include "State.h"

State::State()
	: m_Id(nextStateId++)
{
}

State::State(uint32_t id)
	: m_Id(id)
{
}

// Isn't m_Transitions is a map with keys of type "char"? Then, why the used key here is of type "uint8_t"?
// I think the name "input" would be clearer than the name "transition".
void State::addTransition(std::shared_ptr<State> state, uint8_t transition)
{
	m_Transitions[transition].push_back(state);
}

void State::addTransitions(std::vector<std::shared_ptr<State>> states, uint8_t transition)
{
	m_Transitions[transition].insert(m_Transitions[transition].end(), states.begin(), states.end());
}
