#include <iostream>
#include "Automata.h"

typedef std::pair<std::string, std::string> Arrow;
typedef std::vector<int> States;
typedef std::pair< Arrow, int > Exit;
typedef std::map< int, std::vector< Exit > > Transitions;

Exit createConn(std::string a, std::string b, int o) {
	return Exit(Arrow(a, b), o);
}

void printAutomata(Automata* automata) {
	std::cout << "Name: " << automata->name << std::endl;
	std::cout << "Init: [ ";
	for (const int& state : automata->init) std::cout << state << ", ";
	std::cout << "]\nFin: [ ";
	for (const int& state : automata->fin) std::cout << state << ", ";
	std::cout << "]\nTransitions: \n\n";
	for (Transitions::iterator it = automata->trans.begin(); it != automata->trans.end(); ++it) {
		std::cout << it->first << ":\n";
		for (const Exit& prehod : it->second) {
			std::cout << "(" << it->first << ", <" << prehod.first.first << ", " << prehod.first.second << ">, " << prehod.second << "),\n";
		}
	}
	std::cout << "---------------------------------------------\n";
}

int main(int argc, char argv[]) {

	Transitions a;
	std::vector<Exit> A0;
		A0.push_back(Exit(Arrow("a", "a"), 1));
		A0.push_back(Exit(Arrow("b", "b"), 2));
	std::vector<Exit> A1;
	std::vector<Exit> A2;
	a[0] = A0;
	a[1] = A1;
	a[2] = A2;
	Transitions b;
	std::vector<Exit> B0;
		B0.push_back(Exit(Arrow("a", "b"), 1));
	std::vector<Exit> B1;
		B1.push_back(Exit(Arrow("a", "a"), 2));
	std::vector<Exit> B2;
	b[0] = B0;
	b[1] = B1;
	b[2] = B2;

	Transitions c;
	std::vector<Exit> C0;
	C0.push_back(Exit(Arrow("a", "b"), 1));
	std::vector<Exit> C1;
	c[0] = C0;
	c[1] = C1;

	States initA; initA.push_back(0);
	States initB; initB.push_back(0);
	States finA; finA.push_back(2);
	States finB; finB.push_back(2);
	States initC; initC.push_back(0);
	States finC; finC.push_back(1);

	Automata* A = new Automata("A", initA, finA, a);
	Automata* B = new Automata("B", initB, finB, b);
	Automata* C = new Automata("C", initC, finC, c);

	printAutomata(A);
	printAutomata(B);
	printAutomata(C);
	A = A->starFSA();
	printAutomata(A);

	system("PAUSE");
	return 0;
}

/*
<abb, cca>&(<cc, ab>*<a,a>)


*/