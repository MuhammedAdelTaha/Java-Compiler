#include <iostream>
#include "NFA.h"

int main()
{
	NFA nfa1;
	State i = nfa1.addState();
	State f = nfa1.addState();

	nfa1.addTranstition(i, f, 'a');

	NFA nfa2;
	State a = nfa2.addState();
	State b = nfa2.addState();

	nfa1.addTranstition(i, f, 'b');
}
