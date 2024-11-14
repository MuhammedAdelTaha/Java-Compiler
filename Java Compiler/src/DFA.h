#pragma once
#include "NFA.h"

class DFA
{
public:
	DFA(NFA& nfa);
    void minmize();
private:
    std::vector<std::shared_ptr<State>> m_TerminalStates;
    
};
