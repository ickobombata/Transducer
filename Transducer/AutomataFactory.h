#pragma once
#include "Automata.h"

class AutomataFactory {
private:
public:
	/**
	* Creates an Automata from a transition T
	* Cretes two staes A and B and 
	* adds the link A--T-->B.
	*/
	static Automata* createAutomata(Transition transition) {
		std::vector<Output> A; // state 0
		std::vector<Output> B; // state 1
		A.push_back(Output(transition, 1));
		Transitions transitions;
		transitions[0] = A;
		transitions[1] = B;
		Automata* automata = new Automata(0, 1, transitions);
		return automata;
	}
};