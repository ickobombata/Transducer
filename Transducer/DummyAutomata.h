#pragma once

#include "Automata.h"

class DummyAutomata : public Automata {
public:
	DummyAutomata(std::string name, States init, States fin, Transitions trans) : Automata(init, fin, trans) {
	}

	DummyAutomata(States init, States fin, Transitions trans) : Automata(init, fin, trans) {
	}

	DummyAutomata(int init, int fin, Transitions trans) : Automata(init, fin, trans) {
	}

	DummyAutomata(DummyAutomata* other) : Automata(other) {

	}

	bool isCyclicOnOutputUtil(int v, bool visited[], bool *recStack, bool accOutput) {
		return Automata::isCyclicOnOutputUtil(v, visited, recStack, accOutput);
	}

	bool isInfinitlyAmbiguousRemovedCyclesAndEtoE() {
		return Automata::isInfinitlyAmbiguousRemovedCyclesAndEtoE();
	}

	std::vector< std::unordered_set<int> > findStronglyConnectedComponents(bool epsilonTransitions) {
		return Automata::findStronglyConnectedComponents(epsilonTransitions);
	}
};