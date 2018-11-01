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
public:
	Generator();
	virtual ~Generator();
};

#endif /* GENERATOR_H_ */
