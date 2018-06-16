#include <iostream>
#include "Automata.h"
#include "AutomataFactory.h"
#include "Parser.h"

Output createConn(std::string a, std::string b, int o) {
	return Output(Transition(a, b), o);
}

void printAutomata(Automata* automata) {
	if (automata == nullptr) {
		std::cout << "NULL automata" << std::endl;
		return;
	}
	std::cout << "Name: " << automata->name << std::endl;
	std::cout << "Init: [ ";
	for (const int& state : automata->init) std::cout << state << ", ";
	std::cout << "]\nFin: [ ";
	for (const int& state : automata->fin) std::cout << state << ", ";
	std::cout << "]\nTransitions: \n\n";
	for (Transitions::iterator it = automata->trans.begin(); it != automata->trans.end(); ++it) {
		std::cout << it->first << ":\n";
		for (const Output& prehod : it->second) {
			std::cout << "(" << it->first << ", <" << prehod.first.first << ", " << prehod.first.second << ">, " << prehod.second << "),\n";
		}
	}
	std::cout << "---------------------------------------------\n";
}

void Test_Parser(const char* filePath) {
	Parser parser = Parser(filePath);
	Automata* A = parser.parse();
	printAutomata(A);
}

void Test_AutomataFactory() {
	Automata* A = AutomataFactory::createAutomata(Transition("aaaa", "bbbb"));
	printAutomata(A);
}

void Test_AutomataOperations() {
	Transitions a;
	std::vector<Output> A0;
	A0.push_back(Output(Transition("a", "a"), 1));
	A0.push_back(Output(Transition("b", "b"), 2));
	std::vector<Output> A1;
	std::vector<Output> A2;
	a[0] = A0;
	a[1] = A1;
	a[2] = A2;
	Transitions b;
	std::vector<Output> B0;
	B0.push_back(Output(Transition("a", "b"), 1));
	std::vector<Output> B1;
	B1.push_back(Output(Transition("a", "a"), 2));
	std::vector<Output> B2;
	b[0] = B0;
	b[1] = B1;
	b[2] = B2;

	Transitions c;
	std::vector<Output> C0;
	C0.push_back(Output(Transition("a", "b"), 1));
	std::vector<Output> C1;
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
	A = A->starFSA()->unionFSA(B)->concatFSA(C);
	printAutomata(A);
}

void Test_StrongComponents() {
	Transitions t;
	Transition e(Epsilon, Epsilon);
	Transition a("a", "c");
	Transition b("b", "d");
	t[0] = std::vector<Output>();
	t[1] = std::vector<Output>();
	t[2] = std::vector<Output>();
	t[3] = std::vector<Output>();
	t[4] = std::vector<Output>();
	t[5] = std::vector<Output>();
	t[0].push_back(Output(e, 1));
	t[1].push_back(Output(e, 4));
	t[1].push_back(Output(a, 2));
	t[1].push_back(Output(a, 3));
	t[2].push_back(Output(e, 5));
	t[3].push_back(Output(e, 2));
	t[4].push_back(Output(e, 0));
	t[4].push_back(Output(b, 3));
	t[5].push_back(Output(b, 4));
	t[5].push_back(Output(e, 3));

	Automata* A = new Automata(0, 5, t);

	printAutomata(A);

	std::vector< std::unordered_set<int> > strong = A->findStronglyConnectedComponents(true);
	for (const std::unordered_set<int>& component : strong) {
		std::cout << "[";
		for (const int& el : component) {
			std::cout << el << ", ";
		}
		std::cout << "]" << std::endl;
	}
}

void Test_RemoveEpsilonCycles() {
	Transitions t;
	Transition e(Epsilon, Epsilon);
	Transition a("a", "c");
	Transition b("b", "d");
	t[0] = std::vector<Output>();
	t[1] = std::vector<Output>();
	t[2] = std::vector<Output>();
	t[3] = std::vector<Output>();
	t[4] = std::vector<Output>();
	t[5] = std::vector<Output>();
	t[0].push_back(Output(e, 1));
	t[1].push_back(Output(e, 4));
	t[1].push_back(Output(a, 2));
	t[1].push_back(Output(a, 3));
	t[2].push_back(Output(e, 5));
	t[3].push_back(Output(e, 2));
	t[4].push_back(Output(e, 0));
	t[4].push_back(Output(b, 3));
	t[5].push_back(Output(b, 4));
	t[5].push_back(Output(e, 3));

	Automata* A = new Automata(0, 5, t);
	printAutomata(A);
	A = A->removeEpsilonCycles();
	printAutomata(A);
}

int main(int argc, char argv[]) {
	try {
		//Test_AutomataOperations();
		//Test_AutomataFactory();
		//Test_Parser("./resources/test1.txt");
		//Test_StrongComponents();
		Test_RemoveEpsilonCycles();
	}
	catch (int i) {
		std::cout << i << std::endl;
	}
	system("PAUSE");
	return 0;
}
/*
<abb, cca>&(<cc, ab>*<a,a>)


*/