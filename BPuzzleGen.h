//
// Created by WADwi on 2021/3/7.
//

#ifndef PROJECT_DS_BPUZZLEGEN_H
#define PROJECT_DS_BPUZZLEGEN_H
#include "BPuzzle.h"
#define MAX_INDEX 20

typedef struct BPuzzleGen {
	static int SIZE;
	BPuzzle *solver = nullptr;
	Value **table;
	BPuzzleGen() = default;
	BPuzzleGen(const char * filename, int size);
	Status print();
	Status fsave(const char * );
	Status generate();
	Status only_check();
	~BPuzzleGen();
}BPuzzleGen;


#endif //PROJECT_DS_BPUZZLEGEN_H
