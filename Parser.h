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
#include "Field.h"

class ParserException : public std::exception {
	std::string mess;
public:
	ParserException(std::string base, std::string data): mess(base + data){}
	virtual const char*
	what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_USE_NOEXCEPT {
		return  mess.c_str();
	}
};

class UnsupportedFileFormat: public ParserException {
public:
	UnsupportedFileFormat(std::string&& wrongData):ParserException("We couldn't pars the following line: ", wrongData) {}
};

class CannotOpenFile: public ParserException {
public:
	CannotOpenFile(std::string fileName):ParserException("We couldn't open file: ", fileName) {}
};

class Parser {
	const char* fileName;
public:
	Parser(const char* fileName): fileName(fileName){}
	Resolver makeResolver();
	void saveTask(const Field<char>& field);
};

#endif /* PARSER_H_ */
