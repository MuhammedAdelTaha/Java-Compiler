#include "State.h"

static uint32_t nextStateId = 0;

State::State()
	: m_Id(nextStateId++)
{
}

State::State(uint32_t id)
	: m_Id(id)
{
}

void State::addTransition(std::shared_ptr<State> state, uint8_t transition)
{
	m_Transitions[transition].push_back(state);
}

void State::addTransitions(std::vector<std::shared_ptr<State>> states, uint8_t transition)
{
	m_Transitions[transition].insert(m_Transitions[transition].end(), states.begin(), states.end());
}
