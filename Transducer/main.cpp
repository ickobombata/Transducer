﻿#include <iostream>
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
	Outputs A0;
	A0.insert(Output(Transition("a", "a"), 1));
	A0.insert(Output(Transition("b", "b"), 2));
	Outputs A1;
	Outputs A2;
	a[0] = A0;
	a[1] = A1;
	a[2] = A2;
	Transitions b;
	Outputs B0;
	B0.insert(Output(Transition("a", "b"), 1));
	Outputs B1;
	B1.insert(Output(Transition("a", "a"), 2));
	Outputs B2;
	b[0] = B0;
	b[1] = B1;
	b[2] = B2;

	Transitions c;
	Outputs C0;
	C0.insert(Output(Transition("a", "b"), 1));
	Outputs C1;
	c[0] = C0;
	c[1] = C1;

	States initA; initA.insert(0);
	States initB; initB.insert(0);
	States finA; finA.insert(2);
	States finB; finB.insert(2);
	States initC; initC.insert(0);
	States finC; finC.insert(1);

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
	t[0] = Outputs();
	t[1] = Outputs();
	t[2] = Outputs();
	t[3] = Outputs();
	t[4] = Outputs();
	t[5] = Outputs();
	t[0].insert(Output(e, 1));
	t[1].insert(Output(e, 4));
	t[1].insert(Output(a, 2));
	t[1].insert(Output(a, 3));
	t[2].insert(Output(e, 5));
	t[3].insert(Output(e, 2));
	t[4].insert(Output(e, 0));
	t[4].insert(Output(b, 3));
	t[5].insert(Output(b, 4));
	t[5].insert(Output(e, 3));

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

void Test_StrongComponents1() {
	Transitions t;
	Transition e(Epsilon, Epsilon);
	Transition a("a", "c");
	Transition b("b", "d");
	t[0] = Outputs();
	t[1] = Outputs();
	t[2] = Outputs();
	t[0].insert(Output(e, 1));

	t[1].insert(Output(e, 0));
	t[1].insert(Output(a, 2));

	t[2].insert(Output(e, 0));

	Automata* A = new Automata(0, 1, t);
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
	t[0] = Outputs();
	t[1] = Outputs();
	t[2] = Outputs();
	t[3] = Outputs();
	t[4] = Outputs();
	t[5] = Outputs();
	t[0].insert(Output(e, 1));
	t[1].insert(Output(e, 4));
	t[1].insert(Output(a, 2));
	t[1].insert(Output(a, 3));
	t[2].insert(Output(e, 5));
	t[3].insert(Output(e, 2));
	t[4].insert(Output(e, 0));
	t[4].insert(Output(b, 3));
	t[5].insert(Output(b, 4));
	t[5].insert(Output(e, 3));

	Automata* A = new Automata(0, 5, t);
	printAutomata(A);
	A = A->removeEpsilonCycles();
	printAutomata(A);
}

void Test_RemoveEpsilonCycles1() {
	Transitions t;
	Transition e(Epsilon, Epsilon);
	Transition a("a", "c");
	Transition b("b", "d");
	t[0] = Outputs();
	t[1] = Outputs();
	t[2] = Outputs();
	t[3] = Outputs();
	t[4] = Outputs();
	t[5] = Outputs();
	t[6] = Outputs();
	t[7] = Outputs();
	t[8] = Outputs();
	t[9] = Outputs();
	t[10] = Outputs();
	t[0].insert(Output(e, 1));
	t[0].insert(Output(b, 8));

	t[1].insert(Output(e, 2));

	t[2].insert(Output(a, 3));
	t[2].insert(Output(e, 7));
	t[2].insert(Output(e, 8));

	t[3].insert(Output(e, 5));
	t[3].insert(Output(b, 6));

	t[4].insert(Output(e, 3));

	t[5].insert(Output(e, 4));

	t[6].insert(Output(e, 5));
	t[6].insert(Output(a, 7));

	t[7].insert(Output(a, 3));
	t[7].insert(Output(b, 8));
	t[7].insert(Output(e, 10));

	t[8].insert(Output(a, 1));
	t[8].insert(Output(e, 9));

	t[9].insert(Output(e, 0));

	t[10].insert(Output(e, 7));
	t[10].insert(Output(b, 8));

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
	std::cout << "======================================\n";
	A = A->removeEpsilonCycles();
	printAutomata(A);
}

struct pair_hash {
	std::size_t operator()(const std::pair<int, int>& pair) const{
		std::string s = std::to_string(pair.first);
		//s.append(std::to_string(pair.second));
		return std::hash<std::string>()(s);
	}
};

int main(int argc, char argv[]) {
	std::unordered_set < std::pair<int, int>, pair_hash > aa;
	try {
		//Test_AutomataOperations();
		//Test_AutomataFactory();
		//Test_Parser("./resources/test1.txt");
		//Test_StrongComponents();
		//Test_StrongComponents1();
		//Test_RemoveEpsilonCycles();
		Test_RemoveEpsilonCycles1();
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