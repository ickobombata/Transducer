#pragma once
#include <iostream>
#include <vector>
#include <map>

#define N 10000
#define Epsilon ' '

#typedef std::pair<char*, char*> Arrow;
#typedef std::vector<int> States;
#typedef std::pair< Arrow, int> > Exit;
#typedef std::map<int, std::vector< Exit > Transitions
class Automata {
public:
	States states;
	States init;
	States fin;
	std::map<int, std::vector< std::pair< Arrow, int> > trans;

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
		States remapedInits = remapStates(other->init);
		unionStates(fin, other->fin);
		unionTransitions(trans, other->trans);

		int newState = trans.size;
		std::vector< Exit > exits;
		for (const int& state: init) {
			exits.push_back(Exit(Arrow(Epsilon, Epsilon), state));	
		}
		for (const int& state: remapedInits) {
			exits.push_back(Exit(Arrow(Epsilon, Epsilon), state));	
		}
		trans[newState] = exits;
	}

	void starFSA() {
		
	}

private:
	int remap(int state) {
		return state + trans.size;
	}

	States unionStates(States s1, States s2) {
		States remaped = remapStates(s2);
		for (const int& state: remaped) {
			s1.push_back(state);
		}
		return s1;
	}

	Transitions unionTransitions(Transitions t1, Transitions t2) {
		Transitions remaped = remapTransitions(t2->trans);
		for (Transitions::iterator it = other.begin(); it != other.end(); ++it) {
			t1[it->first] = it->second;
		}
		return t1;
	}

	States remapStates(States states) {
		States remapedState;
		for (const int& state: states) {
			remapedState.push_back(remap(state));
		}
		return remapedState;
	}

	Transitions remapTransitions(Transitions other) {
		Transitions remapedTransitions;
		for (Transitions::iterator it = other.begin(); it != other.end(); ++it) {
			std::vector< Exit > neighbours;
			for(const Exit& as: it->second) {
				neighbours.push_back(Exit(as.first, remap(as.second)));
			}
			remapedTransitions[remap(it->first)] = neighbours; 
		}
		return remapedTransitions;
	}
};
