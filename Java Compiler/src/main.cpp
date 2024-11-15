#include <iostream>
#include <memory>
#include <set>
#include "NFA.h"
#include "DFA.h"

int main()
{
	auto s2 = std::make_shared<State>(2);
	auto s3 = std::make_shared<State>(3);
	auto s4 = std::make_shared<State>(4);
	auto s5 = std::make_shared<State>(5);
	auto s7 = std::make_shared<State>(100);
	auto s8 = std::make_shared<State>(8);
	auto s9 = std::make_shared<State>(101);
	auto s10 = std::make_shared<State>(9);
	auto s11 = std::make_shared<State>(102);
	auto s12 = std::make_shared<State>(10);

	s2->addTransition(s3, 'a');
	s4->addTransition(s5, 'b');

	NFA nfa1(s2, s3);
	NFA nfa2(s4, s5);

	nfa1.unionize(nfa2, 1, 6);
	nfa1.kleeneClosure(0, 7);

	s7->addTransition(s8, 'a');
	s9->addTransition(s10, 'b');
	s11->addTransition(s12, 'b');

	NFA nfa3(s7, s8);
	NFA nfa4(s9, s10);
	NFA nfa5(s11, s12);

	nfa3.concatenate(nfa4);
	nfa3.concatenate(nfa5);

	nfa1.concatenate(nfa3);

	DFA dfa(nfa1);

	auto table = dfa.getTable();
	auto cleanTable = dfa.cleanTable();
	dfa.drawTable(cleanTable);

	// A -> B
	// B -> 
	// C -> 
	// D -> C
	// E -> A
}
