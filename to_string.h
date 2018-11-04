/*
 * to_string.h
 *
 *  Created on: Oct 31, 2018
 *      Author: Andrey Moiseenko
 */

#ifndef TO_STRING_H_
#define TO_STRING_H_
#include <string>
#include "Field.h"
#include "Resolve.h"

namespace std {
inline
std::string to_string(const Field<char>& field) {
	std::string res;
	for (int y = 0; y != 9; ++y) {
		for (int x = 0; x != 9; ++x) {
			res += char(field.get(x, y) + '0');
		}
		res += "\n";
	}
	return res;
}

inline
 std::string to_string(Resolver::DecisionAreaItem item) {
	 std::string res("{ ");
	 for(auto s: item) {
		 res += std::to_string(s) + ", ";
	 }
	 res += "}";
	 return res;
 }


template<class T>
std::string to_string(const Field<T>& field) {
	std::string res;
	for (int y = 0; y != 9; ++y) {
		for (int x = 0; x != 9; ++x) {
			res += std::to_string(field.get(x, y));
		}
		res += "\n";
	}
	return res;
}

inline
std::string to_string(Resolver::Status s) {
	switch (s) {
	case Resolver::NOTREDY: 			return "NOTREDY";
	case Resolver::RESOLVED:			return "RESOLVED";
	case Resolver::NO_ONE_SOLVE:		return "NO_ONE_SOLVE";
	case Resolver::TOO_MORE_SOLVE:		return "TOO_MORE_SOLVE";
	default:
		return "UNKNOWN STATTUS";
	};
	}
}

template<class T>
std::string prettyPrint(const Field<T>& field) {
	std::string res;
	for (int y = 0; y != 9; ++y) {
		for (int x = 0; x != 9; ++x) {
			res += std::to_string(field.get(x, y));
			if(x%3 == 2) res += " | ";
		}
		if(y%3 == 2) res += "\n---------------";
		res += "\n";
	}
	return res;
}

#endif /* TO_STRING_H_ */
