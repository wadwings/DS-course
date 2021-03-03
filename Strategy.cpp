//
// Created by WADwi on 2021/3/2.
//

#include "Strategy.h"
namespace Strategies {

	int linear(CNF &cnf) {
		for (int i = 0; i < cnf.literals_len; i++) {
			if (cnf.literals[i].val == undefined) {
				return cnf.literals[i].id;
			}
		}
		return -1;
	}

//	int frequential(CNF &cnf) {
//		std::vector<Literal> filtered;
//		auto src = cnf.literals;
//		std::copy_if(src.begin(), src.end(), std::back_inserter(filtered),
//								 [](const Literal &l) {
//									 return l.val == undefined;
//								 });
//		std::sort(filtered.begin(), filtered.end(),
//							[](const Literal &a, const Literal b) {
//								return a.count > b.count;
//							}
//		);
//		if (filtered.empty()){
//			return -1;
//		}
//		return filtered[0].id;
//	}
//
//	int random(CNF &cnf) {
//		std::vector<Literal> filtered;
//		auto src = cnf.literals;
//		std::copy_if(src.begin(), src.end(), std::back_inserter(filtered),
//								 [](const Literal &l) {
//									 return l.val == undefined && l.count > 0;
//								 });
//		std::random_device rd;
//		std::mt19937 mt(rd());
//		std::uniform_int_distribution<int> dist(0, filtered.size()-1);
//		return filtered[dist(mt)].id;
//	};

}