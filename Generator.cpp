/*
 * Generator.cpp
 *
 *  Created on: Oct 29, 2018
 *      Author: Andrey Moiseenko
 */

#include "Generator.h"
#include "to_string.h"
#include "logger.h"
const char Generator::startCount[] = {33, 28, 24,20};
const int Generator::rendomGenTotal = 15;
std::shared_ptr<Field<char>> Generator::generate(Generator::Level l) {
	rendomFill(l);
	Resolver::Status s;
	int count = rendomGenTotal;
	do {
		Resolver rc(std::make_shared<Field<char>>(*field));
		s = rc.resolve();
		if(s == Resolver::TOO_MORE_SOLVE) {
			auto[x, y, v1, v2] = rc.variantsResolve();
			field->set(x, y, v1);
			++count;
			LOG("get too more result so set (%d, %d) - %d\n", x, y, v1);
		}
		if(s == Resolver::NO_ONE_SOLVE) {
			LOG("NO_ONE_SOLVE of GenTask \n%s\n", prettyPrint(*rc.getField()).c_str());
			rendomFill(l);
		}
		if(s == Resolver::RESOLVED) {
			LOG("RESOLVED of GenTask \n%s\n", prettyPrint(*rc.getField()).c_str());
			while (count < startCount[l]) {
				int x = genCoordinate(generator);
				int y = genCoordinate(generator);
				if(field->get(x, y) == 0) {
					field->set(x, y, rc.getField()->get(x, y));
					++count;
				}
			}
		}
	} while(s != Resolver::RESOLVED);
	return field;
}

void Generator::rendomFill(Generator::Level l) {
	field = std::make_shared<Field<char>>();
	for (int i = 0; i != rendomGenTotal; ++i) {
		int x, y;
		do {
			x = genCoordinate(generator);
			y = genCoordinate(generator);
		} while (field->get(x, y) != 0);
		int el = genValue(generator);
		auto variants = Resolver::posibleValues(field, x, y);
		int result = el;
		while(variants[result] != 0) {
			++result;
			if (result > 9) result = 1;
		}
		LOG("gen point (%d, %d) - %d \n", x, y, result);
		field->set(x, y, result);

	}
	LOG("RENDOM GENERATED seted %d \n%s\n",rendomGenTotal, prettyPrint(*field).c_str());
}
