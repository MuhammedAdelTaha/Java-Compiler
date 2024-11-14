#include <iostream>
#include <memory>
#include "NFA.h"

int main()
{
	auto i = std::make_shared<State>();
	auto f = std::make_shared<State>();

	i->addTransition(f, 'a');

	auto i1 = std::make_shared<State>();
	auto f1 = std::make_shared<State>();

	i1->addTransition(f1, 'b');
	
	NFA nfa(i, f);
	NFA nfa1(i1, f1);

	//nfa.concatenate(nfa1);
	nfa.unionize(nfa1);

	nfa.traverse();
}
