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

	Automata(std::string name, States init, States fin, Transitions trans);

	Automata(States init, States fin, Transitions trans);

	Automata(int init, int fin, Transitions trans);

	Automata(Automata* other);

	Automata* concatFSA(Automata* other);

	Automata* unionFSA(Automata* other);

	Automata* starFSA();

	/* Returns true if ∃α ∈ Σ* | {m | <α, m> ∈ L(T) } | = ∞
	<-> There Exists ∃ loop with <ε, α> for α ≠ ε.
	*/
	bool isInfinitlyAmbiguous(); //безкрайно многозначен
								  // Mark all the vertices as not visited and not part of recursion
								  // stack

	bool isFunctional();

	Automata* trim();

	/**
	* Constructs the automata inpalce
	*/
	std::pair<Automata*, std::pair<std::vector<std::string>, bool> > makeRealTimeAutomata();

	Automata* removeEpsilonToEpsilonTransitions();

	Automata* removeEpsilonCycles();
	
	Automata* getSquaredAutomata();
	
	/*
	* Traverse an Avtomata.
	*/
	bool traverse2(std::unordered_set<std::string>& result);

	Outputs getReachableByEpsilon(int state);

public:		
	std::pair<Automata*, std::pair<std::vector<std::string>, bool> > removeUpperEpsilon();

	std::pair<StateStateOutputs, bool> inputEpsilonCloser();
	
	States getAllReachableStates();

	Automata* expand();

protected:
	bool isCyclicOnOutputUtil(int v, bool visited[], bool *recStack, bool accOutput);

	std::vector< std::unordered_set<int> > findStronglyConnectedComponents(bool epsilonTransitions);

	// A recursive function to print DFS starting from v
	std::unordered_set<int> DFSUtil(int v, bool visited[], bool epsilonTransition);
private:
	Transition w(Transition uv, Transition xy);

	bool isBalanceable(Transition uv);

	void transitiveEpsilonClosureUtil(int s, int v, bool** tc, std::pair<StateStateOutputs, bool>& result, std::string accOutput);
	
	std::vector<std::pair<int, std::string>> getPathsAndFinalStates(StateStateOutputs& C);

	bool getAllReachableStatesFromStateUtil(int v, bool* visited, States& reachable);


private:
	std::vector<int> kNewStates(Transitions& t, int k);

	int remap(int state);

	States unionStates(States& s1, States& s2);

	Transitions unionTransitions(Transitions& t1, Transitions& t2);

	States remapStates(States& states);

	Transitions remapTransitions(Transitions& other);

	// A recursive function used by topologicalSort
	void topologicalSortUtil(Automata* automata, int v, bool visited[],
		std::stack<int> &Stack, bool epsilonTransitions);

	std::stack<int> sortStatesTopologically(Automata* automata, bool epsilonTransitions);

	/**
	* O(V + E)
	*/
	Automata* getTranspose();

	std::unordered_map<int, std::vector<std::pair<int, std::string>>> proj1_23(StateStateOutputs& C);

	std::unordered_map<int, std::vector<std::pair<int, std::string>>> proj2_13(StateStateOutputs& C);


public: 
	void printAutomata();
};
