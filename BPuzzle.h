//
// Created by WADwi on 2021/3/3.
//

#ifndef PROJECT_DS_BPUZZLE_H
#define PROJECT_DS_BPUZZLE_H

#include "DPLL.h"
class BPuzzle {
	int** BP;
	DPLL dpll;
	BPuzzle() = default;
	BPuzzle(char * filename);
	BPuzzle(const BPuzzle & src);

};


#endif //PROJECT_DS_BPUZZLE_H
