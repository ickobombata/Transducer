#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>

#define N 10000
#define Epsilon " "

typedef std::pair<std::string, std::string> Arrow;
typedef std::vector<int> States;
typedef std::pair< Arrow , int > Exit;
typedef std::map< int, std::vector< Exit > > Transitions;

class Automata {
public:
	std::string name;
	States init;
	States fin;
	Transitions trans;

	Automata(std::string name, States init, States fin, Transitions trans) {
		this->name = name;
		this->init = init;
		this->fin = fin;
		this->trans = trans;
	}

	Automata* concatFSA(Automata* other) {
		States remapedInits = remapStates(other->init);
		States remapedFins = remapStates(other->fin);
		unionTransitions(this->trans, other->trans);

		int newState = this->trans.size();
		std::vector< Exit > exits;
		for (const int& state : remapedInits) {
			exits.push_back(Exit(Arrow(Epsilon, Epsilon), state));
		}
		this->trans[newState] = exits;

		for (const int& state : this->fin) {
			exits.push_back(Exit(Arrow(Epsilon, Epsilon), state));
			trans[state].push_back(Exit(Arrow(Epsilon, Epsilon), newState));
		}
		this->fin = remapedFins;

		return this;
	}

	Automata* unionFSA(Automata* other) {
		States remapedInits = remapStates(other->init);
		unionStates(fin, other->fin);
		unionTransitions(this->trans, other->trans);

		int newState = this->trans.size();
		std::vector< Exit > exits;
		for (const int& state: init) {
			exits.push_back(Exit(Arrow(Epsilon, Epsilon), state));	
		}
		for (const int& state: remapedInits) {
			exits.push_back(Exit(Arrow(Epsilon, Epsilon), state));	
		}
		this->trans[newState] = exits;
		States newInitStates;
		newInitStates.push_back(newState);
		this->init = newInitStates;

		return this;
	}

	Automata* starFSA() {
		int newState = this->trans.size();
		std::vector< Exit > exits;
		for (const int& state : this->init) {
			exits.push_back(Exit(Arrow(Epsilon, Epsilon), state));
		}
		this->trans[newState] = exits;

		for (const int& state : this->fin) {
			exits.push_back(Exit(Arrow(Epsilon, Epsilon), state));
			this->trans[state].push_back(Exit(Arrow(Epsilon, Epsilon), newState));
		}
		std::vector<int> newInit;
		newInit.push_back(newState);
		std::vector<int> newFin;
		newFin.push_back(newState);
		this->init = newInit;
		this->fin = newFin;
		return this;
	}

private:
	int remap(int state) {
		return state + trans.size();
	}

	States unionStates(States& s1, States& s2) {
		States remaped = remapStates(s2);
		for (const int& state: remaped) {
			s1.push_back(state);
		}
		return s1;
	}

	Transitions unionTransitions(Transitions& t1, Transitions& t2) {
		Transitions remaped = remapTransitions(t2);
		for (Transitions::iterator it = remaped.begin(); it != remaped.end(); ++it) {
			t1[it->first] = it->second;
		}
		return t1;
	}

	States remapStates(States& states) {
		States remapedState;
		for (const int& state: states) {
			remapedState.push_back(remap(state));
		}
		return remapedState;
	}

	Transitions remapTransitions(Transitions& other) {
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
