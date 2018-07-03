#pragma once

class AutomataTest {
private: 
	void Test_AutomataFactory();
	void Test_StrongComponents();
	void Test_StrongComponents1();
	void Test_RemoveEpsilonCycles();
	void Test_RemoveEpsilonCycles1();
	void Test_RemoveEpsilonTransitions();
	void Test_IsDummyInfinitlyAmbiguous();
	void Test_TestAmbiguous();
	void Test_RemoveUpperEpsilon();
	void Test_Epand();
	void Test_RealTimeAutomata();
	void Test_SquaredOutputTransducer();
	void Test_SquaredOutputTransducer1();
	void Test_SquaredOutputTransducer2();
	void Test_ReachableStates();
	void Test_Functional1();
	void Test_Traverse();
	void Test_EpsilonIntersection();
public:
	void executeAllTests();
	void executeE2ETests();
	void ComplexTest00();
	void ComplexTest03();
	void ComplexTest02();
	void ComplexTest01();
	void ComplexTest1();
	void ComplexTest2();
	void ComplexTest3();

private: // not included in the pipeline testing
	void Test_AutomataOperations();
	void Test_Parser(const char* filePath, bool isFile);
};