#pragma once
#include <iostream>
#include <fstream>
#include "Automata.h"
#include "AutomataFactory.h"

#define STAR '*'
#define UNION '|'
#define CONCATENATION '^'

class Parser
{
private:
	std::ifstream inFile;
public:

	Parser(const char* filePath) 
	{
		inFile.open(filePath);
	}

	Automata* parse() {
		if (!inFile.good()) {
			throw 41;
		}
		char c;
		while ((c = inFile.get()) == ' ') { // include all whitespaces
			std::cout << c;
		}

		Automata* automata;
		if (c == STAR) {
			automata = parse();
			automata->starFSA();
		} else if (c == UNION) {
			automata = parse();
			automata->unionFSA(parse());
		} else if (c == CONCATENATION) {
			automata = parse();
			automata->concatFSA(parse());
		} else if (c != '<') {
			throw 42;
		} else {
			Transition transition = readTransition();
			automata = AutomataFactory::createAutomata(transition);
		}

		return automata;
	}

private:
	Transition readTransition() {
		char c;
		std::string input;
		std::string output;
		while ((c = inFile.get()) != ':') { // we asume that the file is in the right format
			if (c != ' ') {
				std::cout << c;
				input.push_back(c);
			}
		}
		while ((c = inFile.get()) != '>') { // we asume that the file is in the right format
			if (c != ' ') {
				std::cout << c;
				output.push_back(c);
			}
		}
		return Transition(input, output);
	}
};