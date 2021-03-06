//
// Created by WADwi on 2021/3/1.
//

#ifndef PROJECT_DS_DPLL_H
#define PROJECT_DS_DPLL_H
#include "CNF.h"
#include "Strategy.h"
enum Status {
	holdable,
	unholdable,
	pending,
	done
};

struct DPLL {
	CNF origin;
	CNF result;
	int isSatisfly = 0;
	DPLL() = default;
	DPLL(const CNF & src);
	inline static Status assign(CNF &cnf, int id);
	inline static Status rsc(CNF &cnf);
	Status solve();
	inline static int choose(CNF &cnf);
	Status perform_dpll(CNF &cnf);
	Status check(const CNF & src);
	void save_result(CNF &cnf, int status);
	void print_res(char * path, float time);
};

#endif //PROJECT_DS_DPLL_H
