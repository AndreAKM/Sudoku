/*
 * resolve.cpp
 *
 *  Created on: Oct 29, 2018
 *      Author: Andrey Moiseenko
 */

#include "Resolve.h"
#include <cstring>
#include <cstdio>
#include <iostream>
#include <future>
#include "to_string.h"
#include "logger.h"

std::tuple<int, int> Resolver::neighbors(int c) {
	int shift = c % 3;
		switch(shift) {
		case 0:
			return {1 + c, 2 + c};
		case 1:
			return {c - 1,  1 + c};
		default:
			return {c - 1,  c - 2};
		}
}

Resolver::posibleValueSet Resolver::imposibleValues(std::shared_ptr<Field<char>> field, int x, int y) {
	posibleValueSet variants = {0, 0,0,0, 0,0,0, 0,0,0};
	int xn1, yn1, xn2, yn2;
	std::tie(xn1, xn2) = neighbors(x);
	std::tie(yn1, yn2) = neighbors(y);
	if (field->get(x, yn1) != 0) {
		for( auto& v: variants) ++v;
	} else for(auto r: getRow(xn1, yn1, field)) {
		++variants[r];
	}
	if(field->get(xn1, y) != 0) {
		for( auto& v: variants) ++v;
	} else for(auto r: getColumn(xn1, yn1, field)) {
		++variants[r];
	}
	if(field->get(x, yn2) != 0) {
		for( auto& v: variants) ++v;
	} else for(auto r: getRow(xn2, yn2, field)) {
		++variants[r];
	}
	if(field->get(xn2, y) != 0) {
		for( auto& v: variants) ++v;
	} else for(auto r: getColumn(xn2, yn2, field)) {
		++variants[r];
	}
	return variants;
}
int Resolver::updateDecision(int x, int y, char decision){
	decisionArea->get(x, y).clear();
	for(auto& r: getRow(x, y, decisionArea)) {
		if(r.empty()) continue;
		r.erase(decision);
		if(r.empty()) return -1;
	}
	for(auto& r: getColumn(x, y, decisionArea)) {
		if(r.empty()) continue;
		r.erase(decision);
		if(r.empty()) return -1;
	}
	for(auto& r: getBlock(x, y, decisionArea)) {
		if(r.empty()) continue;
		r.erase(decision);
		if(r.empty()) return -1;
	}
	return 0;
}
Resolver::posibleValueSet Resolver::posibleValues(std::shared_ptr<Field<char>> field, int x, int y) {
	posibleValueSet data = {0, 0,0,0, 0,0,0, 0,0,0};
	for(auto r: getRow(x, y, field)) {
		++data[r];
	}
	for(auto r: getColumn(x, y, field)) {
		++data[r];
	}
	for(auto r: getBlock(x, y, field)) {
		++data[r];
	}
	return data;
}
int Resolver::fillDecisionArea() {
	int count = 0;
	for (int x = 0; x != 9; ++x) {
		for (int y = 0; y != 9; ++y) {
			if (field->get(x,y) != 0) {
				continue;
			}
			auto data = posibleValues(field, x, y);
			auto& l = decisionArea->get(x, y);
			l.clear();
			for(int i = 1; i != 10; ++i) {
				if(data[i] == 0) {
					l.insert(i);
				}
			}
			if(l.size() != 0) {
				certaintyOrder.insert({l.size() , {x,y}});
				count += l.size();
			}
			("insert to certaintyOrder (%d, (%d, %d)) size - %d %d %s\n",
					count, x, y, certaintyOrder.size(), decisionArea->get(x, y).size(), std::to_string(l).c_str());

		}
	}

	LOG( "decisionArea: \n %s\n" ,prettyPrint(*decisionArea).c_str());
	return count;
}

Resolver::DecisionAreaItem& Resolver::cros(DecisionAreaItem& s, const DecisionAreaItem& d) {
	for (auto i: d) {
		s.erase(i);
	}
	return s;
}

int Resolver::groupResolve(int x, int y) {
	auto s = decisionArea->get(x, y);
	if(s.size() == 1) {
		int id = *s.begin();
		//LOG("set (%d, %d) - %d", x, y, id);
		field->set(x, y, id);
		return updateDecision(x, y, id);
	}
	auto coor = std::make_tuple(x, y);
	auto row = getRow(x, y, decisionArea);
	for(auto it = row.begin(); it != row.end(); ++it ) {
		if (it.coordinate() != coor) {
			s = cros(s, *it);
		}
	}
	auto column = getColumn(x, y, decisionArea);
	for(auto it = column.begin(); it != column.end(); ++it ) {
		if (it.coordinate() != coor) {
			s = cros(s, *it);
		}
	}
	auto block = getBlock(x, y, decisionArea);
	for(auto it = block.begin(); it != block.end(); ++it ) {
		if (it.coordinate() != coor) {
			s = cros(s, *it);
		}
	}
	if(s.size() != 1) return decisionArea->get(x, y).size();
	int id = *s.begin();
	//LOG("set (%d, %d) - %d", x, y, id);
	field->set(x, y, id);
	return updateDecision(x, y, id);;
}

int Resolver::ruleResolve(int x, int y) {
	int id = -1;
	const auto& l = decisionArea->get(x, y);
	if(l.size() == 0){
		//LOG("no variants for (%d, %d) - %s \n", x, y, std::to_string(l).c_str());
		return -1;
	}
	if(l.size() == 1) {
		id = *l.begin();
	}
	else {
	}
	if (id != -1) {
		//LOG("set (%d, %d) - %d from variants %s\n", x, y, id, std::to_string(l).c_str());
		field->set(x, y, id);
		return updateDecision(x, y, id);
	}
	return decisionArea->get(x, y).size();
}

void Resolver::reorder() {
	certaintyOrder.clear();
	for(int x = 0; x != 9; ++x) {
		for(int y = 0; y != 9; ++y) {
			int s = decisionArea->get(x, y).size();
			if(s != 0){
				certaintyOrder.insert({s, {x, y}});
			}
		}
	}
	LOG( "decisionArea: \n%s\n" ,prettyPrint(*decisionArea).c_str());
	LOG( "pre result: \n%s\n" ,prettyPrint(*field).c_str());
}

int Resolver::resolveStep(Resolver::Algorithm alg) {
	int count = 0;

	for(auto p: certaintyOrder) {
		auto [x, y] = p.second;
		if(field->get(x, y) != 0) {
			updateDecision(x, y, field->get(x, y));
			continue;
		}
		int sr = (this->*alg)(x, y);
		//LOG("resolve result %d to certaintyOrder (%d, %d) certaintyOrder size - %d \n", sr, x, y, certaintyOrder.size());

		if(sr == -1){
			status = NO_ONE_SOLVE;
			return -1;
		}
		count += sr;
	}
	return count;
}

Resolver::Status Resolver::enumeration() {
	auto [x, y] = certaintyOrder.begin()->second;
	const auto& l =  decisionArea->get(x, y);
	Field<char> copyF(*field);
	for (auto c: l) {
		auto fc = std::make_shared<Field<char>>(copyF);
		LOG("try %d for (%d, %d) from fariants %s \n", c, x, y, std::to_string(l).c_str());
		fc->set(x, y, c);
		Resolver rc(fc);
		auto st = rc.resolve();//std::async(std::launch::async, &Resolver::resolve, rc);
		LOG("get %d for (%d, %d) - %s \n", c, x, y, std::to_string(st).c_str());
		if (st == RESOLVED && status == RESOLVED) {
			std::get<2>(variants) = c;
			status = TOO_MORE_SOLVE;
			return status;
		}
		if (st == RESOLVED) {
			*field = *fc;
			variants = {x, y, c, 0};
			LOG("RESOLVER:\n%s", prettyPrint(*field).c_str());
			status = RESOLVED;
		}
		if (st == TOO_MORE_SOLVE) {
			variants = rc.variantsResolve();
			status = rc.status;
			return status;
		}
	}

	return status;
}

Resolver::Status Resolver::resolve() {
	int count = fillDecisionArea();;
	int prevCount;
	do {
		prevCount = count;
		count = resolveStep(&Resolver::ruleResolve);
		if (count == -1) break;
		reorder();
		count = resolveStep(&Resolver::groupResolve);
		reorder();
	}while(count != 0 && prevCount != count && status == NOTREDY);
	if (count > 0) {
		return enumeration();
	}
	else if (count == 0){
		status = RESOLVED;
	} else {
		status = NO_ONE_SOLVE;
	}
	return status;
}
