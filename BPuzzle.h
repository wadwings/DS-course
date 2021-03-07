//
// Created by WADwi on 2021/3/3.
//
#ifndef PROJECT_DS_BPUZZLE_H
#define PROJECT_DS_BPUZZLE_H
#include "DPLL.h"

struct BPuzzle {
	static int SIZE;
	Value **table;
	int **flattenTable = nullptr;
	DPLL sat = DPLL();
	BPuzzle();
	BPuzzle(const char * filename, int size);
	BPuzzle(Value **TABLE, int size);
	int xy;
	int sxy;
	int sxyu;
	int sxyub;
  Status solve();
  void P2C(CNF & des);
  void init_literals(CNF & des) const;
  void apply_constraint1(CNF & des) const;
  void apply_constraint2(CNF & des);
  void apply_constraint3(CNF & des) const;
	char ** all_combination(int N, int K, char ** bitmask = nullptr);
	static int flatten(int x,int y) ;// vary in [1, n^2]
	int flatten(int s, int x, int y) const;// vary in [n^2 + 1, n^2 + 2 * C(n, 2);
	int flatten(int s, int x, int y, int u) const;//vary in [n^2 + 2 * C(n, 2) + 1, n^2 + 2 * C(n, 2) + 2 * n * C(n, 2)];
	int flatten(int s, int x, int y, int u, int b) const;//vary in [n^2 + 2 * C(n, 2) + 2 * n * C(n, 2) + 1,n^2 + 2 * C(n, 2) + 2 * n * C(n, 2) +  4 * n * C(n, 2)];
	void fsave(const char * filename);
	static int factorial(int k);
	static int C(int N, int K);
	void print();
	~BPuzzle();
};


#endif //PROJECT_DS_BPUZZLE_H
