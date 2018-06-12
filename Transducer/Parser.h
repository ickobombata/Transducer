#pragma once
#include <iostream>
#include <fstream>

class Parser
{
private:
	std::ifstream inFile;
public:

	Parser(const char* filePath) 
	{
		inFile.open(filePath);
	}

	int parse() {
		return 0;
	}
};