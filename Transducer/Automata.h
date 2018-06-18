﻿#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stack>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

#define N 10000
#define Epsilon ""



typedef std::pair<std::string, std::string> Transition;
typedef std::unordered_set<int> States;
typedef std::pair< Transition, int> Output; 

struct output_hash {
	std::size_t operator()(const Output& output) const {
		std::string s = output.first.first;
		s.append(output.first.second);
		s.append(std::to_string(output.second));
		return std::hash<std::string>()(s);
	}
};

typedef std::unordered_set< Output, output_hash> Outputs;
typedef std::unordered_map< int, Outputs> Transitions;

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

	Automata(States init, States fin, Transitions trans) {
		this->name = "";
		this->init = init;
		this->fin = fin;
		this->trans = trans;
	}

	Automata(int init, int fin, Transitions trans) {
		this->name = "";
		this->init.insert(init);
		this->fin.insert(fin);
		this->trans = trans;
	}

	Automata* concatFSA(Automata* other) {
		States remapedInits = remapStates(other->init);
		States remapedFins = remapStates(other->fin);
		unionTransitions(this->trans, other->trans);

		int newState = this->trans.size();
		Outputs exits;
		for (const int& state : remapedInits) {
			exits.insert(Output(Transition(Epsilon, Epsilon), state));
		}
		this->trans[newState] = exits;

		for (const int& state : this->fin) {
			exits.insert(Output(Transition(Epsilon, Epsilon), state));
			trans[state].insert(Output(Transition(Epsilon, Epsilon), newState));
		}
		this->fin = remapedFins;

		return this;
	}

	Automata* unionFSA(Automata* other) {
		States remapedInits = remapStates(other->init);
		unionStates(fin, other->fin);
		unionTransitions(this->trans, other->trans);

		int newState = this->trans.size();
		Outputs exits;
		for (const int& state: init) {
			exits.insert(Output(Transition(Epsilon, Epsilon), state));	
		}
		for (const int& state: remapedInits) {
			exits.insert(Output(Transition(Epsilon, Epsilon), state));	
		}
		this->trans[newState] = exits;
		States newInitStates;
		newInitStates.insert(newState);
		this->init = newInitStates;

		return this;
	}

	Automata* starFSA() {
		int newState = this->trans.size();
		Outputs exits;
		for (const int& state : this->init) {
			exits.insert(Output(Transition(Epsilon, Epsilon), state));
		}
		this->trans[newState] = exits;

		for (const int& state : this->fin) {
			exits.insert(Output(Transition(Epsilon, Epsilon), state));
			this->trans[state].insert(Output(Transition(Epsilon, Epsilon), newState));
		}
		States newInit;
		newInit.insert(newState);
		States newFin;
		newFin.insert(newState);
		this->init = newInit;
		this->fin = newFin;
		return this;
	}

	Automata* removeState(int state) {
		this->trans.erase(state);
		return this;
	}

	/* Returns true if ∃α ∈ Σ* | {m | <α, m> ∈ L(T) } | = ∞
	   <-> There Exists ∃ loop with <ε, α> for α ≠ ε.
	*/
	bool isInfinitlyAmbiguous() { //безкрайно многозначен
		Automata* automata = automataWithRemovedEpsilonInputTransitions();
		
		std::stack<int> topologicallySortedStaes = sortStatesTopologically(automata, false);
		// Mark all the vertices as not visited
		bool *visited = new bool[automata->trans.size()];
		for (int i = 0; i < automata->trans.size(); i++)
			visited[i] = false;

		bool foundVertexWithNotVisitedOutput = false;
		
		// loop through the states and add to visited one by one.
		// if there is a state with transition to a non visited state then there is a cycle
		while (!topologicallySortedStaes.empty()) {
			int v = topologicallySortedStaes.top();
			topologicallySortedStaes.pop();

			for (const Output& output: automata->trans[v]) {
				if (visited[output.second] == false && output.first.second != Epsilon) { // the check here for the second to be true is trivial and allways true.
					foundVertexWithNotVisitedOutput = true;
					break;
				}
			}
			if (foundVertexWithNotVisitedOutput)
				break;
			visited[v] = true;
		}

		return foundVertexWithNotVisitedOutput;
	}

	Automata* removeEpsilonToEpsilonTransitions() {
		return nullptr;
	}

	std::vector< std::unordered_set<int> > findStronglyConnectedComponents(bool epsilonTransitions) {
		std::stack<int> topoSorted;

		// Mark all the vertices as not visited (For first DFS)
		bool *visited = new bool[this->trans.size()];
		for (int i = 0; i < this->trans.size(); i++)
			visited[i] = false;

		// Fill vertices in stack according to their finishing times
		for (int i = 0; i < this->trans.size(); i++)
			if (visited[i] == false)
				topoSorted = sortStatesTopologically(this, epsilonTransitions);

		// Create a reversed graph
		Automata* transposed = getTranspose();

		// Mark all the vertices as not visited (For second DFS)
		for (int i = 0; i < this->trans.size(); i++)
			visited[i] = false;



		std::vector< std::unordered_set<int> > stronglyConnectedComponents;
		// Now process all vertices in order defined by Stack
		while (topoSorted.empty() == false)
		{
			// Pop a vertex from stack
			int v = topoSorted.top();
			topoSorted.pop();

			// Print Strongly connected component of the popped vertex
			if (visited[v] == false)
			{
				stronglyConnectedComponents.push_back(transposed->DFSUtil(v, visited, epsilonTransitions));
			}
		}
		return stronglyConnectedComponents;
	}

	Automata* removeEpsilonCycles() {
		std::vector< std::unordered_set<int> > stronglyConnectedComponents = findStronglyConnectedComponents(true);

		std::unordered_map<int, std::unordered_set<int>* > scc;
		std::unordered_set<int>* aa;
		std::unordered_map<int, int> metaState;
		std::vector<int> minimalStates(this->trans.size()); for (int i = 0; i < minimalStates.size(); ++i) minimalStates[i] = -1;
		int minimalStatesCounter = 0;
		for (const std::unordered_set<int>& component : stronglyConnectedComponents) {
			if (component.empty()) {
				continue;
			}
			int minState = *component.begin();
			for (std::unordered_set<int>::iterator it = component.begin(); it != component.end(); ++it) {
				if (minState > *it) minState = *it;
			}
			aa = new std::unordered_set<int>(component);
			for (std::unordered_set<int>::iterator it = component.begin(); it != component.end(); ++it) {
				scc[*it] = aa;
				metaState[*it] = minState;
				minimalStates[minState] = minimalStatesCounter;
			}
			minimalStatesCounter++;
		}

		for (int i = 0; i < minimalStates.size(); ++i) {
			metaState[i] = minimalStates[metaState[i]];
		}

		std::unordered_set<int> toRemove;
		Transitions transitions;
		for (Transitions::iterator it = this->trans.begin(); it != this->trans.end(); ++it) {
			if (toRemove.find(it->first) != toRemove.end()) {
				continue;
			}
			transitions[metaState[it->first]] = Outputs();

			std::unordered_set<int>* component = scc[it->first];
			for (std::unordered_set<int>::iterator usIt = component->begin(); usIt != component->end(); ++usIt) {
				for (const Output& output : this->trans[*usIt]) {
					toRemove.insert(*usIt);
					if (output.first.first == Epsilon && output.first.second == Epsilon && 
						scc[it->first]->find(output.second) != scc[it->first]->end()) {
						continue;
					}
					transitions[metaState[it->first]].insert(Output(output.first, metaState[output.second]));
				}
			}

		}
		this->trans = transitions;

		States remapedInits;
		States remapedFins;
		for (const int& i : this->init) {
			remapedInits.insert(metaState[i]);
		}
		for (const int& f : this->fin) {
			remapedFins.insert(metaState[f]);
		}
		this->init = remapedInits;
		this->fin = remapedFins;

		return this;
	}
	
	// A recursive function to print DFS starting from v
	std::unordered_set<int> DFSUtil(int v, bool visited[], bool epsilonTransition)
	{
		std::unordered_set<int> stronglyConnectedStates;
		visited[v] = true;
		stronglyConnectedStates.insert(v);

		for (const Output& output : this->trans[v]) {
			if (!visited[output.second]) {
				if (epsilonTransition) {
					if (output.first.first == Epsilon && output.first.second == Epsilon) {
						std::unordered_set<int> scs = DFSUtil(output.second, visited, epsilonTransition);
						stronglyConnectedStates.insert(scs.begin(), scs.end());
					}
				} else {
					std::unordered_set<int> scs = DFSUtil(output.second, visited, epsilonTransition);
					stronglyConnectedStates.insert(scs.begin(), scs.end());
				}
			}
		}
		return stronglyConnectedStates;
	}
private:
	int remap(int state) {
		return state + trans.size();
	}

	States unionStates(States& s1, States& s2) {
		States remaped = remapStates(s2);
		for (const int& state: remaped) {
			s1.insert(state);
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
			remapedState.insert(remap(state));
		}
		return remapedState;
	}

	Transitions remapTransitions(Transitions& other) {
		Transitions remapedTransitions;
		for (Transitions::iterator it = other.begin(); it != other.end(); ++it) {
			Outputs neighbours;
			for(const Output& as: it->second) {
				neighbours.insert(Output(as.first, remap(as.second)));
			}
			remapedTransitions[remap(it->first)] = neighbours; 
		}
		return remapedTransitions;
	}

	Automata* automataWithRemovedEpsilonInputTransitions() {
		Transitions transitions;

		for (Transitions::iterator it = this->trans.begin(); it != this->trans.end(); ++it) {
			transitions[it->first] = Outputs();
			for (const Output& output : it->second) {
				if (output.first.first != Epsilon) {
					transitions[it->first].insert(output);
				}
			}
		}
		return new Automata(this->init, this->fin, transitions);
	}
	
	// A recursive function used by topologicalSort
	void topologicalSortUtil(Automata* automata, int v, bool visited[],
		std::stack<int> &Stack, bool epsilonTransitions)
	{
		// Mark the current node as visited.
		visited[v] = true;

		// Recur for all the vertices adjacent to this vertex
		for (const Output& output : automata->trans[v]) {
			if (!visited[output.second]) {
				if (epsilonTransitions) {
					if (output.first.first == Epsilon && output.first.second == Epsilon) {
						topologicalSortUtil(automata, output.second, visited, Stack, epsilonTransitions);
					}
				}
				else {
					topologicalSortUtil(automata, output.second, visited, Stack, epsilonTransitions);
				}
			}
		}
		// Push current vertex to stack which stores result
		Stack.push(v);
	}

	std::stack<int> sortStatesTopologically(Automata* automata, bool epsilonTransitions) {
		std::stack<int> topologicallySorted;

		// Mark all the vertices as not visited
		bool *visited = new bool[automata->trans.size()];
		for (int i = 0; i < automata->trans.size(); i++)
			visited[i] = false;

		// Call the recursive helper function to store Topological
		// Sort starting from all vertices one by one
		for (int i = 0; i < automata->trans.size(); i++)
			if (visited[i] == false)
				topologicalSortUtil(automata, i, visited, topologicallySorted, epsilonTransitions);
		return topologicallySorted;
	}

	/**
	* O(V + E)
	*/
	Automata* getTranspose() {
		Transitions transposed;
		for (Transitions::iterator it = this->trans.begin(); it != this->trans.end(); ++it) {
			transposed[it->first] = Outputs();
		}
		for (Transitions::iterator it = this->trans.begin(); it != this->trans.end(); ++it) {
			// Recur for all the vertices adjacent to this vertex
			for (const Output& output : it->second) {
				transposed[output.second].insert(Output(output.first, it->first));
			}
		}
		Automata* automata = new Automata(this->fin, this->init, transposed);
		return automata;
	}
};
