/*
 * Generator.h
 *
 *  Created on: Oct 29, 2018
 *      Author: Andrey Moiseenko
 */

#ifndef GENERATOR_H_
#define GENERATOR_H_

#include "Field.h"
#include <memory>

class Generator {
	std::shared_ptr<Field<char>> field;
	char startCount[] = {20,24,28,33};

public:
	enum Level {
		Easy, Medium, Hard, Sumury,
	};
	Generator():field(new Field<char>) {}
	std::shared_ptr<Field<char>> get_field() {return field;}
	std::shared_ptr<Field<char>> generate(Level l);
};

#endif /* GENERATOR_H_ */
