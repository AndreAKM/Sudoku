/*
 * Generator.h
 *
 *  Created on: Oct 29, 2018
 *      Author: Andrey Moiseenko
 */

#ifndef GENERATOR_H_
#define GENERATOR_H_

#include "Field.h"
#include "Resolve.h"
#include <memory>
#include <random>

class Generator {
	std::shared_ptr<Field<char>> field;
	static const char startCount[] ;
	static const int rendomGenTotal;
	std::default_random_engine generator;
	std::uniform_int_distribution<int> genValue;
	std::uniform_int_distribution<int> genCoordinate;

public:
	enum Level {
		Easy, Medium, Hard, Sumury,
	};
	Generator():field(new Field<char>), generator(std::random_device()()), genValue(1, 9), genCoordinate(0, 8) {}
	std::shared_ptr<Field<char>> get_field() {return field;}
	std::shared_ptr<Field<char>> generate(Level l);

protected:
	void rendomFill(Level l);
};

#endif /* GENERATOR_H_ */
