#pragma once
#include <iostream>
#include <vector>

#define N 10000

class Automata {
public:
	std::vector<int> states;
	std::vector<int> init;
	std::vector<int> fin;
	std::vector< std::vector<int> > trans;

	void concatFSA(Automata* other) {
		std::vector<int> remapedStates = remapStates(other->states);
		
		for (int i = 0; i < remapedStates.size; ++i) {
			states.push_back(remapedStates[i]);
		}

		for (int i = 0; i < other->trans.size; ++i) {
			int remapedI = remap(i);
			std::vector<int> remapedInnerStates = remapStates(other->trans[i]);
			trans.push_back(remapedInnerStates);
		}

		std::vector<int> remapedInit = remapStates(other->init);
		trans.push_back(remapedInit);

		int lastEpsilonState = trans.size - 1;
		for (int i = 0; i < fin.size; ++i) {
			trans[i].push_back(lastEpsilonState);
		}

		fin = remapStates(other->fin);
	}

	void unionFSA(Automata* other) {
		
	}

	void starFSA() {
		
	}

private:
	int remap(int state) {
		return state + states.size;
	}

	std::vector<int> remapStates(std::vector<int> states) {
		std::vector<int> remapedState;
		for (int state; state < states.size; ++state) {
			remapedState.push_back(remap(state));
		}
		return remapedState;
	}
};
