/*
 * Parser.cpp
 *
 *  Created on: Oct 29, 2018
 *      Author: Andrey Moiseenko
 */

#include "Parser.h"


// reading a text file
#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include "Field.h"

using namespace std;

Resolver Parser::makeResolver() {
	std::shared_ptr<Field<char>> field(new Field<char>());
	string line;
	ifstream myfile (fileName);
	if (myfile.is_open())
	{
		int y = 0;
		while ( getline (myfile,line) )
		{
			if (line.size() != 9 || y >= 9) {
				throw UnsupportedFileFormat(std::move(line));
			}
			for(int x = 0; x != 9; ++x){
				if(line[x]>'0' && line[x] <='9') {
					field->set(x,y, line[x] - '0');
				}
				else {
					field->set(x,y, 0);
				}
			}
			++y;
		}
		myfile.close();
	}
	else {
		throw CannotOpenFile(fileName);
	}

	return Resolver(field);
}
