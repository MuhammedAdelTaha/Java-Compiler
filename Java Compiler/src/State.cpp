#include "State.h"

static uint32_t nextStateId = 0;

State::State()
	: m_Id(nextStateId++)
{
}

void State::addTransition(std::shared_ptr<State> state, uint8_t transition)
{
	m_Transitions[transition].push_back(state);
}
