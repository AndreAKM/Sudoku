/*
 * Parser.h
 *
 *  Created on: Oct 29, 2018
 *      Author: Andrey Moiseenko
 */

#ifndef PARSER_H_
#define PARSER_H_
#include <exception>
#include <string>
#include "Resolve.h"

class ParserException : public std::exception {

};

class UnsupportedFileFormat: public ParserException {
	std::string data;
public:
	UnsupportedFileFormat(std::string&& wrongData):data(wrongData) {}
	virtual const char*
	what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_USE_NOEXCEPT {
		std::string mess("We couldn't pars the following line: ");
		mess + data;
		return  mess.c_str();
	}
};

class CannotOpenFile: public ParserException {
	std::string data;
public:
	CannotOpenFile(std::string fileName):data(fileName) {}
	virtual const char*
	what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_USE_NOEXCEPT {
		std::string mess("We couldn't open file: ");
		mess + data;
		return  mess.c_str();
	}
};

class Parser {
	const char* fileName;
public:
	Parser(const char* fileName): fileName(fileName){}
	Resolver makeResolver();
};

#endif /* PARSER_H_ */
