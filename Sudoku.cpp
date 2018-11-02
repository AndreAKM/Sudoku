//============================================================================
// Name        : Sudoku.cpp
// Author      : Andrey Moiseenko
// Version     :
// Copyright   : Created on: Oct 29, 2018
// Description : Generator and Resolver of Sudoku C++, Ansi-style
//============================================================================

#include <iostream>
#include <cstdio>
#include <cstring>
#include "Parser.h"
#include "Resolve.h"
#include "Generator.h"
#include "to_string.h"

static const char* GENERATION = "GENERATION";
static const char* RESOLVE = "RESOLVE";

int resolve(const char* filename) {
	Parser p(filename);
	try {
		Resolver r = p.makeResolver();
		std::cout << "Task: \n" << prettyPrint(*r.getField()) << std::endl;
		auto s = r.resolve();
		switch (s) {
			case Resolver::RESOLVED:
				std::cout << "Solve: \n" << prettyPrint(*r.getField()) << std::endl;

				break;
			case Resolver::NO_ONE_SOLVE:
				std::cout << "Couldn't find a solve.";
				break;
			case Resolver::TOO_MORE_SOLVE:
				std::cout << "the task has more then one solve.";
				break;
			default:
				std::cout << "The task hasn't been resolved yet.";
				break;
		}
	} catch (ParserException& e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}

int generation(const char* filename, int level) {
	Parser p(filename);
	Generator gen;
	if(level > Generator::Sumury) {
		std::cout << "UNKNOWN LEVEL" << std::endl;
		return -1;
	}

	auto field = gen.generate(Generator::Level (level));
	std::cout << "generated task: \n" << prettyPrint(*field) << std::endl;
//	try {
//
//
//	} catch (ParserException& e) {
//		std::cerr << e.what() << std::endl;
//	}
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
	    printf("Usage the following format: %s %s[N_LEVEL]|%s FILE_NAME \n", argv[0], GENERATION, RESOLVE);
		return 1;
	}
	int id = 1;
	int cmpres = strcmp(argv[1], RESOLVE);
	int cmpgen = strcmp(argv[1], GENERATION);

	for (; id != argc; ++id) {
		cmpres = strcmp(argv[id], RESOLVE);
		cmpgen = strcmp(argv[id], GENERATION);
		if(cmpres == 0 || cmpgen == 0) {
			break;
		}
	}
	if(cmpres == 0) {
		resolve(argv[id == 1 ? 2 : 1]);
	}
	else if(cmpgen == 0) {
		int level = std::atoi(argv[++id]);
		generation(argv[id == 3 ? 1 : 3], level);
		std::cout << argv[0] << " GENERIC has not been implemented yet " << std::endl;
	}
	else {
		printf("Usage the following format: %s %s[N_LEVEL]|%s FILE_NAME \n", argv[0], GENERATION, RESOLVE);
		return 1;
	}
	return 0;
}
