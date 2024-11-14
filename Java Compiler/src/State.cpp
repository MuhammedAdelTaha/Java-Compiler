#include "State.h"
#include <ostream>

void State::addTransition(std::shared_ptr<State> state, uint8_t transition)
{
	m_Transitions[transition].push_back(state);
}
