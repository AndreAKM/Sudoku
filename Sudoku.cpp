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
#include "to_string.h"

static const char* GENERIC = "GENERIC";
static const char* RESOLVE = "RESOLVE";
int main(int argc, char* argv[]) {
	if (argc < 2) {
	    printf("Usage the following format: %s %s|%s FILE_NAME \n", argv[0], GENERIC, RESOLVE);
		return 1;
	}
	char* fileName = argv[2];
	int cmpres = strcmp(argv[1], RESOLVE);
	int cmpgen = strcmp(argv[1], GENERIC);
	if(cmpres != 0 && cmpgen != 0) {
		fileName = argv[1];
		cmpres = strcmp(argv[2], RESOLVE);
		cmpgen = strcmp(argv[2], GENERIC);
	}
	Parser p(fileName);
	try {
		if(cmpres == 0) {
			Resolver r = p.makeResolver();
			std::cout << "Task: \n" << std::to_string(*r.getField()) << std::endl;

			auto s = r.resolve();
			switch (s) {
				case Resolver::RESOLVED:
					std::cout << "Solve: \n" << std::to_string(*r.getField()) << std::endl;

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
		}
		else if(cmpgen == 0) {
			std::cout << argv[0] << " GENERIC has not been implemented yet " << std::endl;
		}
		else {
			printf("Usage the following format: %s %s|%s FILE_NAME \n", argv[0], GENERIC, RESOLVE);
			return 1;
		}
	} catch (ParserException& e) {

		std::cerr << e.what() << std::endl;
	}
	// Print the user's name:


	return 0;
}
