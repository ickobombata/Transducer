#include <iostream>
#include "Parser.h"
#include "DummyAutomata.h"
#include "Automata.h"
#include "AutomataTest.h"
// regex, isInfAmb, isFunc.
typedef std::unordered_map<std::string, std::vector<int>> TestInfo;

std::vector<int> c(int infAmb, int func) {
	std::vector<int> vec;
	vec.push_back(infAmb);
	vec.push_back(func);
	return vec;
}

void AutomataTest::executeE2ETests() {
	TestInfo expectTestInfo;
	expectTestInfo["<:>"] = c(0, 1);
	expectTestInfo["<a:b>"] = c(0, 1);
	expectTestInfo["<abbc:zxy>* <a:b>^"] = c(0, 1);
	expectTestInfo[" <:aa>* <a:b> ^"] = c(1, 1);
	expectTestInfo[" <:aa> <ba:>| <a:b> ^"] = c(0, 1);
	expectTestInfo[" <:>* <:>* ^<a:b> ^"] = c(0, 1);
	expectTestInfo[" <a:b> <b:a> | <:aa>* ^*<a:b> ^"] = c(1, 1);
	expectTestInfo[" <:a> <b:>*| <c:>^ <:aa>* |*<a:b> ^*"] = c(1, 1);
	expectTestInfo["<a:b><:a><:a>^|*"] = c(1, 1);


	std::cout << "Test runs " << expectTestInfo.size() << std::endl;
	TestInfo actualTestInfo;
	for (TestInfo::iterator it = expectTestInfo.begin(); it != expectTestInfo.end(); ++it) {
		std::cout << "Running tests for : " << it->first << std::endl;
		actualTestInfo[it->first] = std::vector<int>();
		Parser parser(it->first, false);
		Automata* A = parser.parseReversePolish();
		actualTestInfo[it->first].push_back(A->isInfinitlyAmbiguous());
	}
	int failures = 0;
	for (TestInfo::iterator it = expectTestInfo.begin(); it != expectTestInfo.end(); ++it) {
		if (it->second[0] != actualTestInfo[it->first][0]) {
			std::cout << "ERROR: '" << it->first << "' expected infAmb " << it->second[0] << " actual " << actualTestInfo[it->first][0] << "\n";
			++failures;
		}
	}
	std::cout << "Passed: " << expectTestInfo.size() - failures << " , Failed: " << failures << std::endl;
}
#include <set>
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
	Test_RealTimeAutomata();
	

}
void AutomataTest::Test_RealTimeAutomata() {
	Transitions t;
	Transition e(Epsilon, Epsilon);
	Transition a("aa", "aaa");
	Transition ea(Epsilon, "aa");
	Transition b("bbb", "b");
	Transition ce("cc", Epsilon);
	t[0] = Outputs();
	t[1] = Outputs();
	t[2] = Outputs();
	t[3] = Outputs();
	t[4] = Outputs();
	t[0].insert(Output(ea, 1));
	t[1].insert(Output(b, 2));
	t[1].insert(Output(ea, 4));
	t[3].insert(Output(e, 1));
	t[3].insert(Output(ce, 2));
	t[4].insert(Output(a, 0));

	Automata* A = new Automata(0, 2, t);

	A->printAutomata();
	std::pair<Automata*, std::pair<std::vector<std::string>, bool> > res = A->makeRealTimeAutomata();
	Automata* B = res.first;
	B->printAutomata();
	std::cout << "W = [";
	for (auto& s : res.second.first) {
		std::cout << s << ", ";
	}
	std::cout << "]" << std::endl << "inf = " << res.second.second << std::endl;
}
void AutomataTest::Test_Epand() {
	Transitions t;
	Transition e("bbb", "q");
	Transition a("aaa", "aa");
	Transition b(Epsilon, "ccc");
	t[0] = Outputs();
	t[1] = Outputs();
	t[2] = Outputs();
	t[3] = Outputs();
	t[0].insert(Output(a, 1));
	t[1].insert(Output(b, 2));
	t[1].insert(Output(e, 3));

	Automata* A = new Automata(0, 2, t);
	A->printAutomata();
	Automata* B = A->expand();
	B->printAutomata();
}

void AutomataTest::Test_RemoveUpperEpsilon() {
	
}

void AutomataTest::Test_Parser(const char* filePath, bool isFile) {
	Parser parser = Parser(filePath, isFile);
	Automata* A = parser.parsePolish();
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