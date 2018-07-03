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


	//std::cout << "Test runs " << expectTestInfo.size() << std::endl;
	//TestInfo actualTestInfo;
	//for (TestInfo::iterator it = expectTestInfo.begin(); it != expectTestInfo.end(); ++it) {
	//	std::cout << "Running tests for : " << it->first << std::endl;
	//	actualTestInfo[it->first] = std::vector<int>();
	//	Parser parser(it->first, false);
	//	Automata* A = parser.parseReversePolish();
	//	actualTestInfo[it->first].push_back(A->isInfinitlyAmbiguous());
	//}
	//int failures = 0;
	//for (TestInfo::iterator it = expectTestInfo.begin(); it != expectTestInfo.end(); ++it) {
	//	if (it->second[0] != actualTestInfo[it->first][0]) {
	//		std::cout << "ERROR: '" << it->first << "' expected infAmb " << it->second[0] << " actual " << actualTestInfo[it->first][0] << "\n";
	//		++failures;
	//	}
	//}
	//std::cout << "Passed: " << expectTestInfo.size() - failures << " , Failed: " << failures << std::endl;
}
#include <set>
void AutomataTest::executeAllTests() {
	//executeE2ETests();
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
	//Test_RealTimeAutomata();
	//Test_SquaredOutputTransducer();
	//Test_SquaredOutputTransducer1();
	//Test_SquaredOutputTransducer2();
	//Test_w();
	//Test_ReachableStates();
	//Test_Functional1();
	//Test_Traverse();
	//Test_FilterCoReachableStates();
	//ComplexTest00();
	//ComplexTest03();
	ComplexTest02();
	//ComplexTest01();
	//ComplexTest1();
	//ComplexTest2();
	//ComplexTest3();
	//Test_EpsilonIntersection();
}

void AutomataTest::ComplexTest03() {
	Parser parser = Parser();
	Automata* A = parser.parseReversePolish("./resources/test03.txt");
	//A->printAutomata();
	std::cout << "Test 00" << std::endl;
	std::cout << "states: " << A->trans.size() << "\n";

	std::cout << "IsAmb: " << A->isInfinitlyAmbiguous() << std::endl;
	std::cout << "IsFUn: " << A->isFunctional() << std::endl;

	std::cout << "WORDS" << std::endl;
	Automata* C = parser.parseReversePolish("./resources/test03.txt");
	C->expand();
	//C->printAutomata();
	Automata* B = AutomataFactory::epsilonIntersection(C, AutomataFactory::createAutomata("CDXI"));
	std::unordered_set<std::string> res;
	if (B->traverse2(res)) {
		for (auto & x : res) {
			std::cout << x << std::endl;
		}
	}
	else {
		std::cout << " NO WORDS the set is infinite" << std::endl;
	}
	delete A;
	delete C;
	delete B;
}

void AutomataTest::ComplexTest02() {
	Parser parser = Parser();
	Automata* A = parser.parseReversePolish("./resources/test02.txt");
	//A->printAutomata();
	std::cout << "Test 00" << std::endl;
	std::cout << "states: " << A->trans.size() << "\n";

	std::cout << "IsAmb: " << A->isInfinitlyAmbiguous() << std::endl;
	std::cout << "IsFUn: " << A->isFunctional() << std::endl;

	std::cout << "WORDS" << std::endl;
	Automata* C = parser.parseReversePolish("./resources/test02.txt");
	C->expand();
	//C->printAutomata();
	Automata* B = AutomataFactory::epsilonIntersection(C, AutomataFactory::createAutomata("MMXVIII"));
	std::unordered_set<std::string> res;
	if (B->traverse2(res)) {
		for (auto & x : res) {
			std::cout << x << std::endl;
		}
	}
	else {
		std::cout << " NO WORDS the set is infinite" << std::endl;
	}
	delete A;
	delete C;
	delete B;
}

void AutomataTest::ComplexTest01() {
	Parser parser = Parser();
	Automata* A = parser.parseReversePolish("./resources/test01.txt");
	//A->printAutomata();
	std::cout << "Test 00" << std::endl;
	std::cout << "states: " << A->trans.size() << "\n";

	std::cout << "IsAmb: " << A->isInfinitlyAmbiguous() << std::endl;
	std::cout << "IsFUn: " << A->isFunctional() << std::endl;

	std::cout << "WORDS" << std::endl;
	Automata* C = parser.parseReversePolish("./resources/test01.txt");
	C->expand();
	//C->printAutomata();
	Automata* B = AutomataFactory::epsilonIntersection(C, AutomataFactory::createAutomata("MMXVIII"));
	std::unordered_set<std::string> res;
	if (B->traverse2(res)) {
		for (auto & x : res) {
			std::cout << x << std::endl;
		}
	}
	else {
		std::cout << " NO WORDS the set is infinite" << std::endl;
	}
	delete A;
	delete C;
	delete B;
}

void AutomataTest::Test_EpsilonIntersection() {
	Transitions t;
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
	t[11] = Outputs();
	t[12] = Outputs();
	//t[13] = Outputs();
	Transition e0("", "0");
	Transition e3("", "3");
	Transition a1("a", "1");
	Transition ee("", "");
	Transition a2("a", "2");
	Transition b0("b", "0");
	Transition e4("", "4");
	Transition a13("a", ".3");
	Transition a12("a", ".2");
	Transition b7("b", "7");
	Transition e5("", "5");
	Transition a6("a", "6");
	Transition a9("a", "9");
	Transition e8("", "8");
	Transition b1("b", "1");
	Transition b10("b", ".0");
	Transition e11("", ".1");
	t[0].insert(Output(e3, 1));
	t[0].insert(Output(a2, 0));
	t[0].insert(Output(a1, 2));
	t[0].insert(Output(ee, 5));
	t[0].insert(Output(a2, 3));
	t[0].insert(Output(b0, 4));

	t[1].insert(Output(e4, 2));

	t[2].insert(Output(b10, 6));

	t[3].insert(Output(b7, 9));

	t[4].insert(Output(e0, 3));

	t[5].insert(Output(e5, 7));
	t[5].insert(Output(b0, 8));

	t[7].insert(Output(e11, 0));
	t[7].insert(Output(a9, 6));
	t[7].insert(Output(a6, 9));

	t[9].insert(Output(b0, 10));
	t[9].insert(Output(e8, 11));
	t[9].insert(Output(b1, 12));

	t[13].insert(Output(a13, 1));
	t[13].insert(Output(a12, 2));

	States init;
	init.insert(0);
	init.insert(13);
	States fin;
	fin.insert(11);
	fin.insert(12);
	fin.insert(6);
	Automata* A = new Automata(init, fin, t);
	A->printAutomata();
	
	Automata* B = AutomataFactory::epsilonIntersection(A, AutomataFactory::createAutomata("ab"));
	B->printAutomata();
	B->trim();
	B->printAutomata();

	std::unordered_set<std::string> result;
	B->traverse2(result);

	std::cout << "\n";
	for (auto& r : result) {
		std::cout << r << "\n";
	}
}

void AutomataTest::ComplexTest00() {
	Parser parser = Parser();
	Automata* A = parser.parseReversePolish("./resources/test00.txt");
	//A->printAutomata();
	std::cout << "Test 00" << std::endl;
	std::cout << "states: " << A->trans.size() << "\n";

	std::cout << "IsAmb: " << A->isInfinitlyAmbiguous() << std::endl;
	std::cout << "IsFUn: " << A->isFunctional() << std::endl;

	std::cout << "WORDS" << std::endl;
	Automata* C = parser.parseReversePolish("./resources/test00.txt");
	C->expand();
	//C->printAutomata();
	Automata* B = AutomataFactory::epsilonIntersection(C, AutomataFactory::createAutomata("MMXVIII"));
	std::unordered_set<std::string> res;
	if (B->traverse2(res)) {
		for (auto & x : res) {
			std::cout << x << std::endl;
		}
	}
	else {
		std::cout << " NO WORDS the set is infinite" << std::endl;
	}
	delete A;
	delete C;
	delete B;
}

void AutomataTest::ComplexTest1() {
	Parser parser = Parser();
	Automata* A = parser.parseReversePolish("./resources/test1.txt");
	//A->printAutomata();
	std::cout << "Test 1" << std::endl;
	std::cout << "states: " << A->trans.size() << "\n";

	std::cout << "IsAmb: " << A->isInfinitlyAmbiguous() << std::endl;
	std::cout << "IsFUn: " << A->isFunctional() << std::endl;

	std::cout << "WORDS" << std::endl;
	Automata* C = parser.parseReversePolish("./resources/test1.txt");
	C->expand();
	//C->printAutomata();
	Automata* B = AutomataFactory::epsilonIntersection(C, AutomataFactory::createAutomata("MMXVIII"));
	std::unordered_set<std::string> res;
	if (B->traverse2(res)) {
		for (auto & x : res) {
			std::cout << x << std::endl;
		}
	}
	else {
		std::cout << " NO WORDS the set is infinite" << std::endl;
	}
	delete A;
	delete C;
	delete B;
}

void AutomataTest::ComplexTest2() {
	Parser parser = Parser();
	Automata* A = parser.parseReversePolish("./resources/test2.txt");
	//A->printAutomata();
	std::cout << "Test 2" << std::endl;
	std::cout << "states: " << A->trans.size() << "\n";

	std::cout << "IsAmb: " << A->isInfinitlyAmbiguous() << std::endl;
	std::cout << "IsFUn: " << A->isFunctional() << std::endl;

	std::cout << "WORDS" << std::endl;
	Automata* C = parser.parseReversePolish("./resources/test2.txt");
	C->expand();
	//C->printAutomata();
	Automata* B = AutomataFactory::epsilonIntersection(C, AutomataFactory::createAutomata("CCCC"));
	std::unordered_set<std::string> res;
	if (B->traverse2(res)) {
		for (auto & x : res) {
			std::cout << x << std::endl;
		}
	}
	else {
		std::cout << " NO WORDS the set is infinite" << std::endl;
	}

	delete A;
	delete C;
	delete B;
}

void AutomataTest::ComplexTest3() {
	Parser parser = Parser();
	Automata* A = parser.parseReversePolish("./resources/test3.txt");
	//A->printAutomata();
	std::cout << "Test 3" << std::endl;
	std::cout << "states: " << A->trans.size() << "\n";

	std::cout << "IsAmb: " << A->isInfinitlyAmbiguous() << std::endl;
	std::cout << "IsFUn: " << A->isFunctional() << std::endl;

	std::cout << "WORDS" << std::endl;
	Automata* C = parser.parseReversePolish("./resources/test3.txt");
	C->expand();
	//C->printAutomata();
	Automata* B = AutomataFactory::epsilonIntersection(C, AutomataFactory::createAutomata("MMCDXVIII"));
	std::unordered_set<std::string> res;
	if (B->traverse2(res)) {
		for (auto & x : res) {
			std::cout << x << std::endl;
		}
	}
	else {
		std::cout << " NO WORDS the set is infinite" << std::endl;
	}

	delete A;
	delete C;
	delete B;
}

void AutomataTest::Test_Traverse() {
	Transitions t;
	Transition e(Epsilon, Epsilon);
	Transition aa("a", "a");
	Transition aq("a", "q");
	Transition bb("b", "b");
	Transition cc("c", "c");
	Transition mm("m", "m");
	t[0] = Outputs();
	t[1] = Outputs();
	t[2] = Outputs();
	t[0].insert(Output(aq, 0));
	t[0].insert(Output(aa, 1));
	t[0].insert(Output(e, 1));
	t[1].insert(Output(bb, 2));

	Automata* A = new Automata(0, 2, t);
	A->printAutomata();
	std::vector<std::string> res;

}

void AutomataTest::Test_Functional1() {
	Transitions t;
	Transition e(Epsilon, Epsilon);
	Transition ab("a", "b");
	Transition ac("a", "c");
	Transition ay("a", "y");
	t[0] = Outputs();
	t[1] = Outputs();
	t[0].insert(Output(ac, 0));
	t[0].insert(Output(ab, 1));
	t[1].insert(Output(ay, 1));
	//t[1].insert(Output(ay, 1));

	Automata* A = new Automata(0, 1, t);
	A->printAutomata();
	bool isF = A->isFunctional();
	std::cout << ((isF) ? "yes" : "no") << std::endl;
}

void AutomataTest::Test_ReachableStates() {
	Transitions t;
	Transition e("a", "a");
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
	t[9].insert(Output(e, 0));
	t[0].insert(Output(e, 3));
	t[0].insert(Output(e, 2));
	t[1].insert(Output(e, 2));
	t[2].insert(Output(e, 6));
	t[3].insert(Output(e, 5));
	t[3].insert(Output(e, 7));
	t[4].insert(Output(e, 3));
	t[6].insert(Output(e, 7));
	t[7].insert(Output(e, 8));

	States init; init.insert(0); init.insert(1); init.insert(2);
	States fin; fin.insert(8); fin.insert(7);
	Automata* A = new Automata(init, fin, t);
	A->printAutomata();
	States reachable = A->getAllReachableStates();
	for (auto& r : reachable) {
		std::cout << r << std::endl;
	}

	A->trim();
	A->printAutomata();
}

void AutomataTest::Test_SquaredOutputTransducer2() {
	Transitions t;
	Transition e(Epsilon, Epsilon);
	Transition ab("a", "b");
	Transition ac("a", "c");
	Transition ay("a", "y");
	Transition ax("a", "x");
	t[0] = Outputs();
	t[1] = Outputs();
	t[2] = Outputs();
	t[3] = Outputs();
	t[0].insert(Output(ab, 1));
	t[0].insert(Output(ax, 2));
	t[1].insert(Output(ay, 3));
	t[2].insert(Output(ac, 3));
	
		Automata * A = new Automata(0, 3, t);
	A->printAutomata();
	Automata * B = A->getSquaredAutomata();
	B->printAutomata();
	
}

void AutomataTest::Test_SquaredOutputTransducer1() {
	Transitions t;
	Transition e(Epsilon, Epsilon);
	Transition ab("a", "b");
	Transition ac("a", "c");
	Transition ay("a", "y");
	t[0] = Outputs();
	t[1] = Outputs();
	t[0].insert(Output(ac, 0));
	t[0].insert(Output(ab, 1));
	t[1].insert(Output(ay, 1));

	Automata* A = new Automata(0, 1, t);
	A->printAutomata();
	Automata* B = A->getSquaredAutomata();
	B->printAutomata();
}

void AutomataTest::Test_SquaredOutputTransducer() {
	Transitions t;
	Transition e(Epsilon, Epsilon);
	Transition ab("a", "b");
	Transition ac("a", "c");
	Transition xy("x", "y");
	Transition xz("x", "z");
	Transition pl("p", "l");
	Transition ae("a", "e");
	Transition am("a", "m");
	Transition pq("p", "q");
	Transition an("a", "n");
	t[0] = Outputs();
	t[1] = Outputs();
	t[2] = Outputs();
	t[3] = Outputs();
	t[4] = Outputs();
	t[0].insert(Output(xz, 0));
	t[0].insert(Output(ab, 1));
	t[0].insert(Output(xy, 3));
	t[0].insert(Output(ac, 2));
	t[1].insert(Output(pl, 1));
	t[1].insert(Output(ae, 3));
	t[2].insert(Output(pq, 4));
	t[3].insert(Output(am, 4));
	t[4].insert(Output(an, 4));

	Automata* A = new Automata(0, 4, t);
	A->printAutomata();
	Automata* B = A->getSquaredAutomata();
	B->printAutomata();
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
	//Parser parser = Parser(filePath, isFile);
	//Automata* A = parser.parsePolish();
	//A->printAutomata();
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