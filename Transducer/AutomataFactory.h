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
		Outputs A; // state 0
		Outputs B; // state 1
		A.insert(Output(transition, 1));
		Transitions transitions;
		transitions[0] = A;
		transitions[1] = B;
		Automata* automata = new Automata(0, 1, transitions);
		return automata;
	}

	static Automata* createAutomata(std::string seq) {
		if (seq.empty()) {
			return createAutomata(Transition(Epsilon, Epsilon));
		} else {
			Transitions tr;
			int init = 0;
			int fin = seq.length();

			for (int i = 0; i < seq.length(); ++i) {
				tr[i].insert(Output(Transition(std::string(1, seq[i]), Epsilon), i+1));
			}
			tr[seq.length()] = Outputs();

			return new Automata(init, fin, tr);
		}
	}

	static Automata* epsilonIntersection(Automata* transducer, Automata* automata) {
		//initialize
		std::vector<std::pair<int, int>> newStates; //P
		std::unordered_map<std::pair<int, int>, int, positionByState_hash> posByState;
		for (auto& i : transducer->init) {
			newStates.push_back(std::pair<int, int>(i, 0));
			posByState[std::pair<int, int>(i, 0)] = newStates.size() - 1;
		}

		int count = 0;
		Transitions newTransitions;
		while (count < newStates.size()) {
			posByState[newStates[count]] = count;
			// add new states(adjesion to this ones) according to N;
			Outputs left = transducer->trans[newStates[count].first];
			Outputs right = automata->trans[newStates[count].second];

			Outputs epsilonReachable = transducer->getReachableByEpsilon(newStates[count].first);
			std::vector < std::pair< Transition, std::pair<int, int>>> tranState;

			for (auto& er : epsilonReachable) {
				tranState.push_back(std::pair< Transition, std::pair<int, int>>(
					er.first,
					std::pair<int, int>(er.second, newStates[count].second)));
			}

			for (auto& r : right) { // this is single	
				for (auto& l : left) {
					if (l.first.first == r.first.first) {
						tranState.push_back(std::pair< std::pair<std::string, std::string >, std::pair<int, int>>(
							l.first, std::pair<int, int>(l.second, r.second)));
					}
				}
			}

			for (auto& ts : tranState) {
				if (posByState.find(ts.second) == posByState.end()) {
					newStates.push_back(ts.second);
					posByState[ts.second] = posByState.size();
				}
			}
			//add trans here
			newTransitions[count] = Outputs();
			for (auto& ts : tranState) {
				newTransitions[count].insert(Output(ts.first, posByState[ts.second]));
			}
			++count;
		}

		States newInit;
		for (std::unordered_map<std::pair<int, int>, int>::iterator it = posByState.begin(); it != posByState.end(); ++it) {
			if (transducer->init.find(it->first.first) != transducer->init.end() && automata->init.find(it->first.second) != automata->init.end()) {
				newInit.insert(it->second);
			}
		}

		States newFin;
		for (std::unordered_map<std::pair<int, int>, int>::iterator it = posByState.begin(); it != posByState.end(); ++it) {
			if (transducer->fin.find(it->first.first) != transducer->fin.end() && automata->fin.find(it->first.second) != automata->fin.end()) {
				newFin.insert(it->second);
			}
		}
		Automata* B = new Automata(newInit, newFin, newTransitions);
		B->trim();
		return B;
	}
};