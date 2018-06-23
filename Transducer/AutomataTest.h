#pragma once

class AutomataTest {
private: 
	void Test_AutomataFactory();
	void Test_StrongComponents();
	void Test_StrongComponents1();
	void Test_RemoveEpsilonCycles();
	void Test_RemoveEpsilonCycles1();
	void Test_RemoveEpsilonTransitions();
	void Test_IsInfinitlyAmbiguous();
	void Test_IsDummyInfinitlyAmbiguous();
	void Test_TestAmbiguous();
public:
	void executeAllTests();
	void executeE2ETests();

private: // not included in the pipeline testing
	void Test_AutomataOperations();
	void Test_Parser(const char* filePath, bool isFile);
};