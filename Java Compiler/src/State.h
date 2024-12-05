#pragma once
#include <unordered_map>
#include <vector>
#include <memory>

class State;
static uint32_t nextStateId = 0;
typedef std::unordered_map<char, std::vector<std::shared_ptr<State>>> Transitions;

class State
{
public:
	State();
	State(uint32_t id);
    void addTransition(std::shared_ptr<State> state, uint8_t transition);
    void addTransitions(std::vector<std::shared_ptr<State>> states, uint8_t transition);
    inline Transitions& getTransitions() { return m_Transitions; }
    inline uint32_t getId() const { return m_Id; }

private:
    uint32_t m_Id;
    Transitions m_Transitions;
};
