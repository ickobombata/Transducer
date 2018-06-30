#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stack>
#include <queue>
#include <deque>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

#define N 10000
#define Epsilon ""

struct stateStateOutput_hash {
	std::size_t operator()(std::pair<int, std::pair<int, std::string>> p) const {
		std::string s = std::to_string(p.first);
		s.append(std::to_string(p.second.first));
		s.append(p.second.second);
		return std::hash<std::string>()(s);
	}
};

struct positionByState_hash {
	std::size_t operator()(std::pair<int, int> p) const {
		std::string s = std::to_string(p.first);
		s.append(std::to_string(p.second));
		return std::hash<std::string>()(s);
	}
};

typedef std::pair<int, std::string> StateOutput;
typedef std::pair<int, StateOutput> StateStateOutput;
typedef std::unordered_set<StateStateOutput, stateStateOutput_hash> StateStateOutputs;

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

	Automata(Automata* other) {
		this->name = other->name;
		this->init = States(other->init);
		this->fin = States(other->fin);
		this->trans = Transitions(other->trans);
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

	Automata* m_starFSA() {

	}

	Automata* removeState(int state) {
		this->trans.erase(state);
		return this;
	}

	/* Returns true if ∃α ∈ Σ* | {m | <α, m> ∈ L(T) } | = ∞
	<-> There Exists ∃ loop with <ε, α> for α ≠ ε.
	*/
	bool isInfinitlyAmbiguous() { //безкрайно многозначен
								  // Mark all the vertices as not visited and not part of recursion
								  // stack
		bool *visited = new bool[this->trans.size()];
		bool *recStack = new bool[this->trans.size()];
		for (int i = 0; i < this->trans.size(); i++)
		{
			visited[i] = false;
			recStack[i] = false;
		}

		// Call the recursive helper function to detect cycle in different
		// DFS trees
		for (int i = 0; i < this->trans.size(); i++)
			if (isCyclicOnOutputUtil(i, visited, recStack, false))
				return true;

		return false;
	}

	bool isFunctional() {
		Transition emptyTransition = Transition(Epsilon, Epsilon);
		//printAutomata();
		std::pair<Automata*, std::pair<std::vector<std::string>, bool> > realTimeAutomata = makeRealTimeAutomata();
		//printAutomata();
		getSquaredAutomata();
		//printAutomata();
		this->trim();
		//printAutomata();

		if (realTimeAutomata.second.second == true) return false;

		// initialize
		std::unordered_map<int, Transition> Adm;
		bool functional = true;
		std::queue<int> toBeProcessed;
		for (auto& i : this->init) {
			Adm[i] = Transition(Epsilon, Epsilon);
			toBeProcessed.push(i);
		}

		while (!toBeProcessed.empty() && functional == true) {
			int q = toBeProcessed.front(); toBeProcessed.pop();
			Transition h = Adm[q];
			
			std::vector<std::pair<int, Transition>> Dq;
			for (auto& output : this->trans[q]) {
				Dq.push_back(std::pair<int, Transition>(output.second, w(h, output.first)));
			}

 			for (auto& qh : Dq) {
				functional &= isBalanceable(qh.second);
				if (this->fin.find(qh.first) != this->fin.end()) {
					if (qh.second != emptyTransition) {
						functional &= false;
					}
				}

				if (Adm.find(qh.first) != Adm.end()) {
					if (qh.second != Adm[qh.first]) {
						functional &= false;
					}
				}
			}
			for (auto& qh : Dq) {
				if (Adm.find(qh.first) == Adm.end()) {
					toBeProcessed.push(qh.first);
					Adm[qh.first] = qh.second;
				}
			}
		}
		
		return !realTimeAutomata.second.second && (realTimeAutomata.second.first.size() <= 1) && functional;
	}

	Transition w(Transition uv, Transition xy) {
		Transition res = Transition("", "");

		Transition temp = uv;
		temp.first.append(xy.first);
		temp.second.append(xy.second);

		int count = 0;
		while (count < temp.first.size() && count < temp.second.size()
			&& temp.first[count] == temp.second[count]) {
			count++;
		}

		int newCount = count;
		while (newCount < temp.first.size()) {
			res.first.append(std::string(1, temp.first[newCount++]));
		}

		newCount = count;
		while (newCount < temp.second.size()) {
			res.second.append(std::string(1, temp.second[newCount++]));
		}

		return res;
	}

	bool isBalanceable(Transition uv) {  
		return uv.first == Epsilon || uv.second == Epsilon;
	}

	Automata* trim() {
		States reachableStates = getAllReachableStates();

		std::unordered_map<int, int> map;
		int next = 0;
		int i = 0;
		while (next < this->trans.size()) {
			while (reachableStates.find(next) == reachableStates.end() && next < this->trans.size()) {
				++next;
			}
			map[next++] = i++;
		}

		Transitions newTransitions;
		for (auto& it : this->trans) {
			if (reachableStates.find(it.first) != reachableStates.end()) {
				newTransitions[map[it.first]] = Outputs();
				for (auto& output : it.second) {
					if (reachableStates.find(output.second) != reachableStates.end()) {
						newTransitions[map[it.first]].insert(Output(output.first, map[output.second]));
					}
				}
			}
		}

		States newInit;
		for (auto& i : this->init) {
			if (reachableStates.find(i) != reachableStates.end()) {
				newInit.insert(map[i]);
			}
		}
		this->init = newInit;

		States newFin;
		for (auto& i : this->fin) {
			if (reachableStates.find(i) != reachableStates.end()) {
				newFin.insert(map[i]);
			}
		}
		this->fin = newFin;

		this->trans = newTransitions;

		return this;
	}

	/**
	* Constructs the automata inpalce
	*/
	std::pair<Automata*, std::pair<std::vector<std::string>, bool> > makeRealTimeAutomata() {
		removeEpsilonCycles();
		trim();
		removeEpsilonToEpsilonTransitions();
		expand();
		
		return removeUpperEpsilon();
	}

	Automata* removeEpsilonToEpsilonTransitions() {
		std::stack<int> topoSorted = sortStatesTopologically(this, true);
		std::stack<int> reversed;
		while (!topoSorted.empty()) {
			reversed.push(topoSorted.top());
			topoSorted.pop();
		}
		while (!reversed.empty()) {
			const int state = reversed.top();
			reversed.pop();

			Outputs additional;
			Outputs epsilons;
			for (const Output& output : this->trans[state]) {
				if (output.first.first == Epsilon && output.first.second == Epsilon) {
					additional.insert(trans[output.second].begin(), trans[output.second].end());
					epsilons.insert(output);
				}
			}
			if (additional.size() > 0) {
				trans[state].insert(additional.begin(), additional.end());
				for (Outputs::iterator it = epsilons.begin(); it != epsilons.end(); ++it) {
					trans[state].erase(*it);
				}
			}
		}

		return this;
	}

	Automata* removeEpsilonCycles() {
		std::vector< std::unordered_set<int> > stronglyConnectedComponents = findStronglyConnectedComponents(true);

		std::unordered_map<int, std::unordered_set<int>* > scc;
		std::unordered_set<int>* aa;
		std::unordered_map<int, int> metaState;
		std::vector<int> minimalStates(this->trans.size());
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
	
	Automata* getSquaredAutomata() {
		//initialize
		std::unordered_map<std::pair<int, int>, int, positionByState_hash> posByState;
		std::vector<std::pair<int, int>> newStates; //P
		for (auto& i : this->init) {
			for (auto& j: this->init) {
				newStates.push_back(std::pair<int, int>(i, j));
				posByState[std::pair<int, int>(i, j)] = newStates.size() - 1;
			}
		}

		int count = 0;
		Transitions newTransitions;
		while (count < newStates.size()) {
			posByState[newStates[count]] = count;
			// add new states(adjesion to this ones) according to N;
			Outputs left = this->trans[newStates[count].first];
			Outputs right = this->trans[newStates[count].second];

			std::vector < std::pair< std::pair<std::string, std::string >, std::pair<int, int>>> tranState;
			for (auto& l : left) {
				for (auto& r : right) {
					if (l.first.first == r.first.first) {
						tranState.push_back(std::pair< std::pair<std::string, std::string >, std::pair<int, int>>(
							std::pair<std::string, std::string>(
								l.first.second, r.first.second), 
							std::pair<int, int>(
								l.second, r.second)));
					}
				}
			}

			for (auto& ts : tranState) {
				if (posByState.find(ts.second) == posByState.end() ) {
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
			if (this->init.find(it->first.first) != this->init.end() && this->init.find(it->first.second) != this->init.end()) {
				newInit.insert(it->second);
			}
		}

		States newFin;
		for (std::unordered_map<std::pair<int, int>, int>::iterator it = posByState.begin(); it != posByState.end(); ++it) {
			if (this->fin.find(it->first.first) != this->fin.end() && this->fin.find(it->first.second) != this->fin.end()) {
				newFin.insert(it->second);
			}
		}
		this->init = newInit;
		this->fin = newFin;
		this->trans = newTransitions;
		return this;
	}
	

	struct Info {
		int state;
		std::string output;

		Info(int state, std::string output) : state(state), output(output) {}
		
		bool operator==(const Info& rhs)
		{
			return (this->state == rhs.state) && (this->output == rhs.output);
		}

		bool operator!=(const Info& rhs)
		{
			return !(*this == rhs);
		}
	};

	/*
	* Traverse an Avtomata.
	*/
	bool traverse2(std::unordered_set<std::string>& result) {
		if (isInfinitlyAmbiguous()) {
			return false;
		}
		this->removeEpsilonCycles();
		this->removeEpsilonToEpsilonTransitions();
		this->trim();
		std::queue < std::pair<int, std::string>> qu;
		for (auto& i : this->init) {
			qu.push(std::pair<int, std::string>(i, Epsilon));
		}

		while (!qu.empty()) {
			std::pair<int, std::string> cur = qu.front();
			qu.pop();

			if (this->fin.find(cur.first) != this->fin.end()) {
				result.insert(cur.second);
			}

			for (auto& output : this->trans[cur.first]) {
				qu.push(std::pair<int, std::string>(output.second, cur.second + output.first.second));
			}
		}
		
		return true;
	}

	bool traverse(std::string src, std::vector<std::string>& result) {
		this->removeEpsilonToEpsilonTransitions();
		this->expand();
		this->trim();
		Info NONE(-1, Epsilon);
		std::deque<Info> qu;
		for (auto& i : this->init) {
			qu.push_back(Info(i, Epsilon));
		}

		for (auto& x : src) {
			qu.push_back(NONE);
			std::unordered_map<int, std::unordered_set<std::string> >visitedOnLevel;
			while (!qu.empty() && qu.front() != NONE) {
				Info cur = qu.front();
				qu.pop_front();
				visitedOnLevel[cur.state].insert(cur.output);
				for (auto& output : this->trans[cur.state]) {
					Info toAdd(output.second, cur.output + output.first.second);

					if (output.first.first == Epsilon) {
						if (visitedOnLevel.find(output.second) != visitedOnLevel.end()) {
							if (visitedOnLevel[output.second].find(output.first.second) != visitedOnLevel[output.second].end()) {
								//return false;
							}
							continue;
						}
						qu.push_front(toAdd);
						visitedOnLevel[toAdd.state].insert(toAdd.output);
					} else if (x == output.first.first[0]) {
						qu.push_back(toAdd);
					}
				}
			}

			if (qu.empty()) return false;
			qu.pop_front();
		}
		while (!qu.empty()) {
			Info cur = qu.front();
			qu.pop_front();
			if (isStateCoReachableByEpsilon(cur.state)) {
				result.push_back(cur.output);
			}
		}
		return true;
	}

	bool isStateCoReachableByEpsilon(int state) {
		if (this->fin.find(state) != this->fin.end())
			return true;
		bool* visited = new bool[this->trans.size()];
		for (int i = 0; i < this->trans.size(); ++i) visited[i] = false;
		std::queue<int> qu;
		qu.push(state);
		
		while (!qu.empty()) {
			int cur = qu.front();
			qu.pop();
			visited[cur] = true;

			for (auto& o : this->trans[cur]) {
				if (!visited[o.second] && o.first.first == Epsilon) {
					if (this->fin.find(o.second) != this->fin.end()) {
						return true;
					} else {
						qu.push(o.second);
					}
				}
			}
		}

		return false;
	}

	Outputs getReachableByEpsilon(int state) {
		bool* visited = new bool[this->trans.size()];
		for (int i = 0; i < this->trans.size(); ++i) visited[i] = false;
		std::queue<std::pair<int, std::string>> qu;
		qu.push(std::pair<int, std::string>(state, ""));

		Outputs outputs;
		while (!qu.empty()) {
			std::pair<int, std::string> cur = qu.front();
			qu.pop();
			visited[cur.first] = true;

			for (auto& o : this->trans[cur.first]) {
				if (o.first.first == Epsilon) {
					if (visited[o.second]) {
						outputs.insert(Output(Transition(Epsilon, cur.second + o.first.second), o.second));
					} else if (!visited[o.second]) {
						outputs.insert(Output(Transition(Epsilon, cur.second + o.first.second), o.second));
						qu.push(std::pair<int, std::string>(o.second, cur.second + o.first.second));
					}
				}
			}
		}

		return outputs;
	}

public:
	// Returns true if the graph contains a cycle, else false
	bool isCyclic(Automata* automata) {
		// Mark all the vertices as not visited and not part of recursion
		// stack
		bool *visited = new bool[automata->trans.size()];
		bool *recStack = new bool[automata->trans.size()];
		for (int i = 0; i < automata->trans.size(); i++)
		{
			visited[i] = false;
			recStack[i] = false;
		}

		// Call the recursive helper function to detect cycle in different
		// DFS trees
		for (int i = 0; i < automata->trans.size(); i++)
			if (isCyclicUtil(automata, i, visited, recStack))
				return true;

		return false;
	}
		
	std::pair<Automata*, std::pair<std::vector<std::string>, bool> > removeUpperEpsilon() {
		std::pair<StateStateOutputs, bool> C = inputEpsilonCloser();
		std::vector<std::pair<int, std::string>> WF = getPathsAndFinalStates(C.first);
		std::vector<std::string> W;
		for (auto& wf : WF) {
			W.push_back(wf.second);
			this->fin.insert(wf.first);
		}

		auto Ce = proj1_23(C.first);
		auto eC = proj2_13(C.first);

		// optimize split the delta in two the first one with first Epsilon and the others without epsilon.
		Transitions newTransitions;
		for (auto& t : this->trans) {
			for (auto& output : t.second) {
				if (output.first.first != Epsilon) {
					for (auto& qu : eC[t.first]) {
						for (auto& rw : Ce[output.second]) {
							if (newTransitions.find(qu.first) == newTransitions.end()) {
								newTransitions[qu.first] = Outputs();
							}
							std::string uvw = qu.second; 
							uvw.append(output.first.second);
							uvw.append(rw.second);
							newTransitions[qu.first].insert(Output(Transition(output.first.first, uvw), rw.first));
						}
					}
				}
			}
		}
		this->trans = newTransitions;

		return std::pair<Automata*, std::pair<std::vector<std::string>, bool> >(
			this, 
			std::pair < std::vector<std::string>, bool>(W, C.second));
	}

	std::vector<std::pair<int, std::string>> getPathsAndFinalStates(StateStateOutputs& C) {
		std::vector<std::pair<int, std::string>> WF;
		for (auto& t : C) {
			if (this->init.find(t.first) != this->init.end() && this->fin.find(t.second.first) != this->fin.end()) {
				WF.push_back(std::pair<int, std::string>(t.first, t.second.second));
			}
		}
		return WF;
	}

	std::pair<StateStateOutputs, bool> inputEpsilonCloser() {
		// Call the recursive helper function to print DFS
		// traversal starting from all vertices one by one
		bool** tc; // To store transitive closure
		std::pair<StateStateOutputs, bool> result;
		tc = new bool*[this->trans.size()];
		for (int i = 0; i<this->trans.size(); i++)
		{
			tc[i] = new bool[this->trans.size()];
			memset(tc[i], false, this->trans.size() * sizeof(bool));
		}
		for (int i = 0; i < this->trans.size(); i++) {
			transitiveEpsilonClosureUtil(i, i, tc, result, Epsilon);
		}

		result.second = isInfinitlyAmbiguous();
		return result;
	}

	void transitiveEpsilonClosureUtil(int s, int v, bool** tc, std::pair<StateStateOutputs, bool>& result, std::string accOutput)
	{
		tc[s][v] = true;
		result.first.insert(StateStateOutput(s, StateOutput(v, accOutput)));
		for (const Output& output : this->trans[v]) {
			if (output.first.first == Epsilon) {
				std::string newAccOut = accOutput;
				newAccOut.append(output.first.second);
				if (s == output.second) {
					result.first.insert(StateStateOutput(s, StateOutput(s, newAccOut)));
				} else if(tc[s][output.second] == false) {
					transitiveEpsilonClosureUtil(s, output.second, tc, result, newAccOut);
				}
			}
		}
	}
	
	States getAllReachableStates() {
		bool *visited = new bool[this->trans.size()];
		for (int i = 0; i < this->trans.size(); i++)
		{
			visited[i] = false;
		}

		States reachable;
		for (auto& i : this->init) {
			getAllReachableStatesFromStateUtil(i, visited, reachable);
		}

		return reachable;
	}

	bool getAllReachableStatesFromStateUtil(int v, bool* visited, States& reachable) {
		bool hasReachableFinState = this->fin.find(v) != this->fin.end() || reachable.find(v) != reachable.end();
		if (visited[v] == false) {
			visited[v] = true;

			for (const Output& output : this->trans[v]) {
				if (getAllReachableStatesFromStateUtil(output.second, visited, reachable)) {
					hasReachableFinState = true;
				}
			}

			if (hasReachableFinState) {
				reachable.insert(v);
			}
		} 
		return hasReachableFinState;
	}

	Automata* expand() {
		Transitions newTransitions;
		for (auto& t : this->trans) {
			newTransitions[t.first] = Outputs();
		}
		for (auto& t : this->trans) {
			for (auto& output : t.second) {
				if (output.first.first.length() > 1) {
					std::vector<int> newStates = kNewStates(newTransitions, output.first.first.length() - 1);
					for (int i = 0; i < newStates.size() - 1; ++i) {
						Output intermediateState(Transition(std::string(1, output.first.first[i + 1]), Epsilon), newStates[i + 1]);
						newTransitions[newStates[i]].insert(intermediateState);
					}
					Output lastState(Transition(std::string(1, output.first.first[output.first.first.length() - 1]), Epsilon), output.second);
					newTransitions[newStates[newStates.size() - 1]].insert(lastState);
					Output firstState(Transition(std::string(1, output.first.first[0]), output.first.second), newStates[0]);
					newTransitions[t.first].insert(firstState);
				} else {
					newTransitions[t.first].insert(output);
				}
			}
		}
		this->trans = newTransitions;
		return this;
	}

protected:
	bool isCyclicOnOutputUtil(int v, bool visited[], bool *recStack, bool accOutput) {
		if (visited[v] == false) {
			// Mark the current node as visited and part of recursion stack
			visited[v] = true;
			recStack[v] = true;
			// Recur for all the vertices adjacent to this vertex
			for (const Output& output : this->trans[v]) {
				accOutput |= output.first.second != Epsilon;
				if (output.first.first != Epsilon)
					continue;
				if (!visited[output.second] && isCyclicOnOutputUtil(output.second, visited, recStack, accOutput))
					return true;
				else if (recStack[output.second] && accOutput)
					return true;
			}
		}
		recStack[v] = false;  // remove the vertex from recursion stack
		return false;
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
				}
				else {
					std::unordered_set<int> scs = DFSUtil(output.second, visited, epsilonTransition);
					stronglyConnectedStates.insert(scs.begin(), scs.end());
				}
			}
		}
		return stronglyConnectedStates;
	}
private:
	std::vector<int> kNewStates(Transitions& t, int k) {
		std::vector<int> res;
		int cur = t.size();
		for (int i = 0; i < k; ++i) {
			t[cur] = Outputs();
			res.push_back(cur++);
		}
		return res;
	}

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

	bool isCyclicUtil(Automata* automata, int v, bool visited[], bool *recStack) {
		if (visited[v] == false) {
			// Mark the current node as visited and part of recursion stack
			visited[v] = true;
			recStack[v] = true;

			// Recur for all the vertices adjacent to this vertex
			for (const Output& output : automata->trans[v]) {
				if (!visited[output.second] && isCyclicUtil(automata, output.second, visited, recStack))
					return true;
				else if (recStack[output.second])
					return true;
			}
		}
		recStack[v] = false;  // remove the vertex from recursion stack
		return false;
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

	std::unordered_map<int, std::vector<std::pair<int, std::string>>> proj1_23(StateStateOutputs& C) {
		std::unordered_map<int, std::vector<std::pair<int, std::string>>> res;
		for (auto& s : this->trans) {
			res[s.first] = std::vector<std::pair<int, std::string>>();
		}
		for (auto& t : C) {
			res[t.first].push_back(t.second);
		}
		return res;
	}

	std::unordered_map<int, std::vector<std::pair<int, std::string>>> proj2_13(StateStateOutputs& C) {
		std::unordered_map<int, std::vector<std::pair<int, std::string>>> res;
		for (auto& s : this->trans) {
			res[s.first] = std::vector<std::pair<int, std::string>>();
		}
		for (auto& t : C) {
			res[t.second.first].push_back(std::pair<int, std::string>(t.first, t.second.second));
		}
		return res;
	}


public: 
	void printAutomata() {
		std::cout << "Name: " << this->name << std::endl;
		std::cout << "Init: [ ";
		for (const int& state : this->init) std::cout << state << ", ";
		std::cout << "]\nFin: [ ";
		for (const int& state : this->fin) std::cout << state << ", ";
		std::cout << "]\nTransitions: \n\n";
		for (Transitions::iterator it = this->trans.begin(); it != this->trans.end(); ++it) {
			std::cout << it->first << ":\n";
			for (const Output& prehod : it->second) {
				std::cout << "(" << it->first << ", <" << prehod.first.first << ", " << prehod.first.second << ">, " << prehod.second << "),\n";
			}
		}
		std::cout << "---------------------------------------------\n";
	}
};
