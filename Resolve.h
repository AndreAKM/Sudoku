/*
 * resolve.h
 *
 *  Created on: Oct 29, 2018
 *      Author: Andrey Moiseenko
 */

#ifndef RESOLVE_H_
#define RESOLVE_H_
#include <memory>
#include <exception>
#include <array>
#include <map>
#include <set>
#include <functional>
#include "Field.h"


class Resolver {
public:
	using DecisionAreaItem = std::set<char>;
	using DecisionArea = Field<DecisionAreaItem>;

	enum Status{
		NOTREDY,
		RESOLVED,
		NO_ONE_SOLVE,
		TOO_MORE_SOLVE,
	};
	Resolver(std::shared_ptr<Field<char>> field): field(field), decisionArea(new DecisionArea()) {}
	using posibleValueSet = std::array<char, 10>;
	static posibleValueSet imposibleValues(std::shared_ptr<Field<char>> field, int x, int y);
	static posibleValueSet posibleValues(std::shared_ptr<Field<char>> field, int x, int y);
	Status resolve();
	std::shared_ptr<Field<char>> getField() {
		return field;
	}

private:
	static std::tuple<int, int> neighbors(int c);
	int fillDecisionArea();
	int ruleResolve(int x, int y);
	int groupResolve(int x, int y);
	int updateDecision(int x, int y, char decision);
	void reorder();
	Status enumeration();
	using Algorithm = int (Resolver::*)(int, int);
	int resolveStep(Algorithm alg);
	DecisionAreaItem& cros(DecisionAreaItem& s, const DecisionAreaItem& d);
	std::shared_ptr<Field<char>> field;
	Status status = NOTREDY;
	std::multimap<int, std::tuple<int, int>> certaintyOrder;
	std::shared_ptr<DecisionArea> decisionArea;
};



#endif /* RESOLVE_H_ */
