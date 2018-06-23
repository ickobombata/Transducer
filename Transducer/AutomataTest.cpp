#include <iostream>
#include "Parser.h"
#include "DummyAutomata.h"
#include "Automata.h"
#include "AutomataTest.h"
// regex, isInfAmb, isFunc.
typedef std::unordered_map<std::string, std::vector<int>> TestInfo;

void AutomataTest::executeE2ETests() {
	TestInfo testInfo;
	std::string a = "<:>";  testInfo[a] = std::vector<int>(); testInfo[a].push_back(0); testInfo[a].push_back(1);
	a = "<a:b>";  testInfo[a] = std::vector<int>(); testInfo[a].push_back(0); testInfo[a].push_back(1);
	a = "^<a:b> *<abbc:zxy>";  testInfo[a] = std::vector<int>(); testInfo[a].push_back(0); testInfo[a].push_back(1);
	a = "^<a:b> *<:aa>";  testInfo[a] = std::vector<int>(); testInfo[a].push_back(1); testInfo[a].push_back(1);

	std::cout << "Test runs " << testInfo.size() << std::endl;
	std::vector<std::string> failed;
	for (TestInfo::iterator it = testInfo.begin(); it != testInfo.end(); ++it) {
		Parser parser(it->first, false);
		Automata* A = parser.parse();

		if (A->isInfinitlyAmbiguous() != it->second[0]) {
			failed.push_back(it->first);
		}
	}
	std::cout << "Passed: " << testInfo.size() - failed.size() << " , Failed: " << failed.size() << " [";
	for (std::string failure : failed) {
		std::cout << failure << ", ";
	}
	std::cout << "]" << std::endl;
}

void AutomataTest::executeAllTests() {
	executeE2ETests();
	//Test_AutomataOperations();
	//Test_AutomataFactory();
	//Test_Parser("./resources/test1.txt", true);
	//Test_StrongComponents();
	//Test_StrongComponents1();
	//Test_RemoveEpsilonCycles();
	//Test_RemoveEpsilonCycles1();
	//Test_RemoveEpsilonTransitions();
	//Test_IsInfinitlyAmbiguous();
	//Test_IsDummyInfinitlyAmbiguous();
	//Test_TestAmbiguous();
	//Test_Parser("^*<aa:b>^<ab:ba>^|<aa:b><ab:ba>*<aa:b>", false);
}

void AutomataTest::Test_Parser(const char* filePath, bool isFile) {
	Parser parser = Parser(filePath, isFile);
	Automata* A = parser.parse();
	A->printAutomata();
}

void AutomataTest::Test_AutomataFactory() {
	Automata* A = AutomataFactory::createAutomata(Transition("aaaa", "bbbb"));
	A->printAutomata();
}

void AutomataTest::Test_AutomataOperations() {
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

	A->printAutomata();
	B->printAutomata();
	C->printAutomata();
	A = A->starFSA()->unionFSA(B)->concatFSA(C);
	A->printAutomata();
}

void AutomataTest::Test_StrongComponents() {
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

	DummyAutomata* A = new DummyAutomata(0, 5, t);

	A->printAutomata();

	std::vector< std::unordered_set<int> > strong = A->findStronglyConnectedComponents(true);
	for (const std::unordered_set<int>& component : strong) {
		std::cout << "[";
		for (const int& el : component) {
			std::cout << el << ", ";
		}
		std::cout << "]" << std::endl;
	}
}

void AutomataTest::Test_StrongComponents1() {
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

	DummyAutomata* A = new DummyAutomata(0, 1, t);
	A->printAutomata();
	std::vector< std::unordered_set<int> > strong = A->findStronglyConnectedComponents(true);
	for (const std::unordered_set<int>& component : strong) {
		std::cout << "[";
		for (const int& el : component) {
			std::cout << el << ", ";
		}
		std::cout << "]" << std::endl;
	}
}

void AutomataTest::Test_RemoveEpsilonCycles() {
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
	A->printAutomata();
	A = A->removeEpsilonCycles();
	A->printAutomata();
}

void AutomataTest::Test_RemoveEpsilonCycles1() {
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

	DummyAutomata* A = new DummyAutomata(0, 5, t);
	A->printAutomata();

	std::vector< std::unordered_set<int> > strong = A->findStronglyConnectedComponents(true);
	for (const std::unordered_set<int>& component : strong) {
		std::cout << "[";
		for (const int& el : component) {
			std::cout << el << ", ";
		}
		std::cout << "]" << std::endl;
	}
	std::cout << "======================================\n";
	A->removeEpsilonCycles();
	A->printAutomata();
}

void AutomataTest::Test_RemoveEpsilonTransitions() {
	Transitions t;
	Transition e(Epsilon, Epsilon);
	Transition a("a", Epsilon);
	Transition b(Epsilon, "d");
	t[0] = Outputs();
	t[1] = Outputs();
	t[2] = Outputs();
	t[3] = Outputs();
	t[4] = Outputs();
	t[5] = Outputs();
	t[0].insert(Output(a, 1));
	t[1].insert(Output(e, 4));
	t[1].insert(Output(a, 2));
	t[1].insert(Output(a, 3));
	t[2].insert(Output(e, 5));
	t[3].insert(Output(a, 2));
	t[4].insert(Output(e, 0));
	t[4].insert(Output(b, 3));
	t[5].insert(Output(b, 4));
	t[5].insert(Output(e, 3));

	Automata* A = new Automata(0, 5, t);
	A->printAutomata();
	A = A->removeEpsilonToEpsilonTransitions();
	A->printAutomata();
}

void AutomataTest::Test_IsInfinitlyAmbiguous() {
	Transitions t;
	Transition e(Epsilon, Epsilon);
	Transition a("a", Epsilon);
	Transition b(Epsilon, "d");
	t[0] = Outputs();
	t[1] = Outputs();
	t[2] = Outputs();
	t[3] = Outputs();
	t[4] = Outputs();
	t[5] = Outputs();
	t[0].insert(Output(a, 1));
	t[1].insert(Output(e, 4));
	t[1].insert(Output(a, 2));
	t[1].insert(Output(a, 3));
	t[2].insert(Output(e, 5));
	t[3].insert(Output(a, 2));
	t[4].insert(Output(e, 0));
	t[4].insert(Output(b, 3));
	t[5].insert(Output(b, 4));
	t[5].insert(Output(e, 3));

	DummyAutomata* A = new DummyAutomata(0, 5, t);
	A->printAutomata();
	std::cout << "Is ambiguous " << A->isInfinitlyAmbiguousRemovedCyclesAndEtoE() << std::endl;
}

void AutomataTest::Test_IsDummyInfinitlyAmbiguous() {
	Transitions t;
	Transition e(Epsilon, Epsilon);
	Transition a("a", Epsilon);
	Transition b(Epsilon, "d");
	t[0] = Outputs();
	t[1] = Outputs();
	t[2] = Outputs();
	t[3] = Outputs();
	t[4] = Outputs();
	t[5] = Outputs();
	t[0].insert(Output(a, 1));
	t[1].insert(Output(e, 4));
	t[1].insert(Output(a, 2));
	t[1].insert(Output(a, 3));
	t[2].insert(Output(e, 5));
	t[3].insert(Output(a, 2));
	t[4].insert(Output(e, 0));
	t[4].insert(Output(b, 3));
	t[5].insert(Output(b, 4));

	t[3].insert(Output(e, 5));

	Automata* A = new Automata(0, 5, t);
	A->printAutomata();

	std::cout << "Is ambiguous " << A->isInfinitlyAmbiguous() << std::endl;
}

void AutomataTest::Test_TestAmbiguous() {
	Transitions t;
	Transition e(Epsilon, Epsilon);
	Transition a("a", "c");
	Transition b(Epsilon, "d");
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

	DummyAutomata* A = new DummyAutomata(0, 5, t);
	A->printAutomata();

	std::cout << "Is ambiguous " << A->isInfinitlyAmbiguous() << std::endl;


	std::vector< std::unordered_set<int> > strong = A->findStronglyConnectedComponents(true);
	for (const std::unordered_set<int>& component : strong) {
		std::cout << "[";
		for (const int& el : component) {
			std::cout << el << ", ";
		}
		std::cout << "]" << std::endl;
	}
	std::cout << "======================================\n";
	A->removeEpsilonCycles();
	A->printAutomata();


	std::cout << "Is ambiguous " << A->isInfinitlyAmbiguous() << std::endl;
}