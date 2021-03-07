//
// Created by WADwi on 2021/3/2.
//

#ifndef PROJECT_DS_STRATEGY_H
#define PROJECT_DS_STRATEGY_H
#include "CNF.h"
namespace CNF_Strategies {
	int linear(CNF &cnf);
//	int random(CNF &cnf);
	int frequential(CNF &cnf);
}

namespace CNF_Strategies{
	int linear(int *);
	int random();
}

#endif //PROJECT_DS_STRATEGY_H
