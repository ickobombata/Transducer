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

	Automata* parseReversePolish() {
		std::stack<Automata*> constructedAutomata;

		while (hasNext()) {
			char c;
			while ((c = getNext()) == ' ') { // include all whitespaces
				if (DEBUG) std::cout << "Reading space\n";
			}
			if (c == STAR) {
				if (constructedAutomata.size() <= 0)
					throw 41;
				constructedAutomata.top()->starFSA();
			}
			else if (c == UNION) {
				if (constructedAutomata.size() <= 1)
					throw 41;
				Automata* last = constructedAutomata.top();
				constructedAutomata.pop();
				constructedAutomata.top()->unionFSA(last);
				delete last;
			}
			else if (c == CONCATENATION) {
				if (constructedAutomata.size() <= 1)
					throw 41;
				Automata* last = constructedAutomata.top();
				constructedAutomata.pop();
				constructedAutomata.top()->concatFSA(last);
				delete last;
			}
			else if (c != '<') {
				throw 42;
			}
			else {
				Transition transition = readTransition();
				constructedAutomata.push(AutomataFactory::createAutomata(transition));
			}
		}
		if (constructedAutomata.size() > 1 || constructedAutomata.size() <= 0)
			throw 41;
		return constructedAutomata.top();
	}

	Automata* parsePolish() {
		char c;
		while ((c = getNext()) == ' ') { // include all whitespaces
			if (DEBUG) std::cout << "Reading space\n";
		}

		Automata* automata;
		if (c == STAR) {
			automata = parsePolish();
			automata->starFSA();
		} else if (c == UNION) {
			automata = parsePolish();
			automata->unionFSA(parsePolish());
		} else if (c == CONCATENATION) {
			automata = parsePolish();
			automata->concatFSA(parsePolish());
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

	bool hasNext() {
		if (this->isFile) {
			return !this->inFile.eof();
		} else {
			return sequenceIndex < sequence.size();
		}
	}

	char getNext() {
		if (!hasNext()) {
			throw 41;
		}
		if (this->isFile) {
			return this->inFile.get();
		} else {
			return this->sequence[sequenceIndex++];
		}
	}
};