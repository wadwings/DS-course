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
	bool satisfiable;
	DPLL() = default;
	DPLL(const CNF & src);
	static Status assign(CNF &cnf, int id);
	Status rsc(CNF &cnf);
	Status solve();
	static int choose(CNF &cnf);
	Status perform_dpll(CNF &cnf);
	void save_result(CNF &cnf, int status);
};

#endif //PROJECT_DS_DPLL_H