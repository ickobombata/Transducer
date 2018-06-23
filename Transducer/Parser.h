#pragma once
#include <iostream>
#include <fstream>
#include "Automata.h"
#include "AutomataFactory.h"

#define STAR '*'
#define UNION '|'
#define CONCATENATION '^'

#define DEBUG false

class Parser
{
private:
	std::ifstream inFile;
	std::string sequence;
	int sequenceIndex;
	bool isFile;
public:

	Parser(const char* path, bool isFile)
	{
		Parser(std::string(path), isFile);
	}

	Parser(std::string path, bool isFile)
	{
		this->isFile = isFile;
		if (isFile) {
			inFile.open(path);
		}
		else {
			sequence = std::string(path);
			sequenceIndex = 0;
		}

	}

	Automata* parse() {
		char c;
		while ((c = getNext()) == ' ') { // include all whitespaces
			if (DEBUG) std::cout << c;
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
		if (DEBUG) std::cout << "<";
		while ((c = getNext()) != ':') { // we asume that the file is in the right format
			if (c != ' ') {
				if (DEBUG) std::cout << c;
				input.push_back(c);
			}
		}
		if (DEBUG) std::cout << ":";
		while ((c = getNext()) != '>') { // we asume that the file is in the right format
			if (c != ' ') {
				if (DEBUG) std::cout << c;
				output.push_back(c);
			}
		}
		if (DEBUG) std::cout << ">\n";
		return Transition(input, output);
	}

	char getNext() {
		if (this->isFile) {
			if (!this->inFile.good()) {
				throw 41;
			}
			return this->inFile.get();
		} else {
			if (this->sequenceIndex >= this->sequence.size()) {
				throw 42;
			}
			return this->sequence[sequenceIndex++];
		}
	}
};