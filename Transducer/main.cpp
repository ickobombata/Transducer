#include <iostream>
#include "AutomataTest.h"


int main(int argc, char argv[]) {
	try {
		AutomataTest tester;
		tester.executeAllTests();
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