//
// Created by WADwi on 2021/3/3.
//

#include "BPuzzle.h"
int BPuzzle::SIZE = 6;
BPuzzle::BPuzzle() : xy(SIZE * SIZE), sxy(this->xy + 2 * C(SIZE, 2)), sxyu(this->sxy + 2 * SIZE * C(SIZE, 2)),
										 sxyub(this->sxyu + 4 * SIZE * C(SIZE, 2)), flattenTable((int **) calloc(sizeof(int[SIZE]), SIZE)) {
	table = (Value **)calloc(sizeof(Value*), SIZE);
	for (int x = 0; x < SIZE; x++) {
		table[x] = (Value *)calloc(sizeof(Value), SIZE);
		for (int y = 0; y < SIZE; y++)
			table[x][y] = undefined;
	}
	int count = 0;
	for (int x = 0; x < SIZE; x++) {
		for (int y = x; y < SIZE; y++) {
			flattenTable[x][y] = count;
			flattenTable[y][x] = count++;
		}
	}
};

int BPuzzle::flatten(int x, int y) {
	return (x - 1) * SIZE + y - 1;
}

int BPuzzle::flatten(int s, int x, int y) const {
	return xy + s * C(SIZE, 2) + flattenTable[x - 1][y - 1];
}

int BPuzzle::flatten(int s, int x, int y, int u) const {
	return sxy + (s * SIZE + u - 1) * C(SIZE, 2) + flattenTable[x - 1][y - 1];
}

int BPuzzle::flatten(int s, int x, int y, int u, int b) const {
	return sxyu + (s * SIZE * 2 + (u - 1) * 2 + b) * C(SIZE, 2) + flattenTable[x - 1][y - 1];
}
BPuzzle::BPuzzle(Value **TABLE, int size) {
	SIZE = size;
	xy = SIZE * SIZE;
	sxy = this->xy + 2 * C(SIZE, 2);
	sxyu = this->sxy + 2 * SIZE * C(SIZE, 2);
	sxyub = this->sxyu + 4 * SIZE * C(SIZE, 2);
	table = (Value **)calloc(sizeof(Value*), SIZE);
	init_literals(sat.origin);
	for (int x = 0; x < SIZE; x++) {
		table[x] = (Value *)calloc(sizeof(Value), SIZE);
		for (int y = 0; y < SIZE; y++){
			table[x][y] = TABLE[x][y];
			if(TABLE[x][y] != undefined){
				auto t = new Clause(1);
				t->literals[0] = &CNF::map[flatten(x + 1, y + 1)][TABLE[x][y] == positive ? 1 : 0];
				sat.origin.add_clauses(t);
			}
		}
	}
	int count = 0;
	flattenTable = (int **) calloc(sizeof(int *), SIZE);
	for (int t = 0; t < SIZE; t++) {
		flattenTable[t] = (int *) calloc(sizeof(int), SIZE);
	}
	for (int x = 0; x < SIZE; x++) {
		for (int y = x + 1; y < SIZE; y++) {
			flattenTable[x][y] = count;
			flattenTable[y][x] = count++;
		}
	}
	P2C(sat.origin);
}

BPuzzle::BPuzzle(const char *filename, int size) : sat(){
	SIZE = size;
	xy = SIZE * SIZE;
	sxy = this->xy + 2 * C(SIZE, 2);
	sxyu = this->sxy + 2 * SIZE * C(SIZE, 2);
	sxyub = this->sxyu + 4 * SIZE * C(SIZE, 2);
	table = (Value **)calloc(sizeof(Value*), SIZE);
	for (int x = 0; x < SIZE; x++) {
		table[x] = (Value *)calloc(sizeof(Value), SIZE);
		for (int y = 0; y < SIZE; y++)
			table[x][y] = undefined;
	}
	int count = 0;
	flattenTable = (int **) calloc(sizeof(int *), SIZE);
	for (int t = 0; t < SIZE; t++) {
		flattenTable[t] = (int *) calloc(sizeof(int), SIZE);
	}
	for (int x = 0; x < SIZE; x++) {
		for (int y = x + 1; y < SIZE; y++) {
			flattenTable[x][y] = count;
			flattenTable[y][x] = count++;
		}
	}
	init_literals(sat.origin);
	int x, y, b;
	FILE *fp;
	if ((fp = fopen(filename, "r")) == nullptr) {
		printf("error occurred while opening file %s", filename);
		exit(-1);
	}
	while (!feof(fp)) {
		fscanf(fp, "%d %d %d", &x, &y, &b);
		table[x - 1][y - 1] = b == 1 ? positive : negative;
		auto t = new Clause(1);
		t->literals[0] = &CNF::map[flatten(x, y)][b];
		sat.origin.add_clauses(t);
	}
	P2C(sat.origin);
}

void BPuzzle::P2C(CNF &des) {
	apply_constraint1(des);
	apply_constraint2(des);
	apply_constraint3(des);
}

void BPuzzle::init_literals(CNF &des) const {
	auto &map = CNF::map;
	auto &literals_len = des.literals_len = sxyub;
	map = (Literal **) calloc(sizeof(Literal *), literals_len);
	for (int index = 0; index < literals_len; index++) {
		map[index] = (Literal *) calloc(sizeof(Literal), 2);
		map[index][1].id = index + 1;
		map[index][0].id = index + 1;
		map[index][1].val = positive;
		map[index][0].val = negative;
	}
	des.literals = (Literal *) calloc(sizeof(Literal), literals_len);
	for (int i = 0; i < literals_len; i++) {
		des.literals[i] = {i + 1, undefined, 0, 0};
	}
}

void BPuzzle::apply_constraint1(CNF &des) const {
	int polars[2]{0, 1};
	for (auto polar : polars) {
		for (int l = 1; l <= SIZE; l++) {
			for (int s = 1; s <= SIZE - 2; s++) {
				auto r_clause = new Clause(3), c_clause = new Clause(3);
				for (int i = 0; i < 3; i++) {
					r_clause->literals[i] = &CNF::map[flatten(l, s + i)][polar];
					c_clause->literals[i] = &CNF::map[flatten(s + i, l)][polar];
				}
				des.add_clauses(r_clause);
				des.add_clauses(c_clause);
			}
		}
	}
}

void BPuzzle::apply_constraint2(CNF &des) {
	char **bitmask = all_combination(SIZE, SIZE / 2 - 1);
	int t = C(SIZE, SIZE / 2 - 1);
	for (int x = 1; x <= SIZE; x++) {
		for (int i = 0; i < t; i++) {
			auto r_clause_1 = new Clause(SIZE / 2 + 1), r_clause_2 = new Clause(SIZE / 2 + 1),
			c_clause_1 = new Clause(SIZE / 2 + 1), c_clause_2 = new Clause(SIZE / 2 + 1);
			int count = 0;
			for (int j = 0; j < SIZE; j++) {
				if(bitmask[i][j] == '1'){
					r_clause_1->literals[count] = &CNF::map[flatten(x, j + 1)][1];
					r_clause_2->literals[count] = &CNF::map[flatten(x, j + 1)][0];
					c_clause_1->literals[count] = &CNF::map[flatten(j + 1, x)][1];
					c_clause_2->literals[count++] = &CNF::map[flatten(j + 1, x)][0];
				}
			}
			des.add_clauses(r_clause_1);
			des.add_clauses(r_clause_2);
			des.add_clauses(c_clause_1);
			des.add_clauses(c_clause_2);
		}
	}
	for(int i = 0; i < t; i++)
		delete bitmask[i];
	delete bitmask;
}

void BPuzzle::apply_constraint3(CNF &des) const {
	for (int s = 0; s <= 1; s++) {
		for (int x = 1; x <= SIZE; x++) {
			for (int y = x + 1; y <= SIZE; y++) {
				const int sxy = flatten(s,x,y);
				auto sxy_clause_1 = new Clause(SIZE + 1);
				auto sxy_clause = new Clause(1);
				sxy_clause->literals[0] = &CNF::map[sxy][1];
				sxy_clause_1->literals[0] = &CNF::map[sxy][0];
				for (int u = 1; u <= SIZE; u++) {
					const int sxyu = flatten(s,x,y,u);
					auto sxyu_clause_1 = new Clause(3);
					auto sxyu_clause_2 = new Clause(2);
					auto sxyu_clause_3 = new Clause(2);
					auto sxy_clause_2 = new Clause(2);
					sxyu_clause_1->literals[2] = &CNF::map[sxyu][0];
					sxyu_clause_2->literals[1] = &CNF::map[sxyu][1];
					sxyu_clause_3->literals[1] = &CNF::map[sxyu][1];
					for (int b = 0; b <= 1; b++) {
						const int sxyub = flatten(s, x, y, u, b);
						auto sxyub_clause_1 = new Clause(3);
						auto sxyub_clause_2 = new Clause(2);
						auto sxyub_clause_3 = new Clause(2);
						if(s == 0){
							sxyub_clause_1->literals[0] = &CNF::map[flatten(x, u)][0];
							sxyub_clause_1->literals[1] = &CNF::map[flatten(y, u)][0];
							sxyub_clause_1->literals[2] = &CNF::map[sxyub][1];
							sxyub_clause_2->literals[0] = &CNF::map[flatten(x, u)][1];
							sxyub_clause_2->literals[1] = &CNF::map[sxyub][0];
							sxyub_clause_3->literals[0] = &CNF::map[flatten(y, u)][1];
							sxyub_clause_3->literals[1] = &CNF::map[sxyub][0];
						}else{
							sxyub_clause_1->literals[0] = &CNF::map[flatten(u, x)][1];
							sxyub_clause_1->literals[1] = &CNF::map[flatten(u, y)][1];
							sxyub_clause_1->literals[2] = &CNF::map[sxyub][1];
							sxyub_clause_2->literals[0] = &CNF::map[flatten(u, x)][0];
							sxyub_clause_2->literals[1] = &CNF::map[sxyub][0];
							sxyub_clause_3->literals[0] = &CNF::map[flatten(u, y)][0];
							sxyub_clause_3->literals[1] = &CNF::map[sxyub][0];
						}
						if (b == 0) {
							sxyu_clause_1->literals[0] = &CNF::map[sxyub][1];
							sxyu_clause_2->literals[0] = &CNF::map[sxyub][0];
						} else {
							sxyu_clause_1->literals[1] = &CNF::map[sxyub][1];
							sxyu_clause_3->literals[0] = &CNF::map[sxyub][0];
						}
						des.add_clauses(sxyub_clause_1);
						des.add_clauses(sxyub_clause_2);
						des.add_clauses(sxyub_clause_3);
					}
					sxy_clause_1->literals[u] = &CNF::map[sxyu][0];
					sxy_clause_2->literals[0] = &CNF::map[sxy][1];
					sxy_clause_2->literals[1] = &CNF::map[sxyu][1];
					des.add_clauses(sxyu_clause_1);
					des.add_clauses(sxyu_clause_2);
					des.add_clauses(sxyu_clause_3);
					des.add_clauses(sxy_clause_2);
				}
				des.add_clauses(sxy_clause);
				des.add_clauses(sxy_clause_1);
			}
		}
	}
}

int BPuzzle::factorial(int k) {
	int i = 1;
	while (k > 1)i *= k--;
	return i;
}

int BPuzzle::C(int N, int K) {
	return factorial(N) / factorial(N - K) / factorial(K);
}

char **BPuzzle::all_combination(int N, int K, char **bitmask) {
	//N represent 0 + 1 nums, K represent 0 nums
	if (bitmask == nullptr) {
		int t = C(N, K);
		bitmask = (char **) calloc(sizeof(char *), t);
		for (int i = 0; i < t; i++)
			bitmask[i] = (char *) calloc(sizeof(char), SIZE);
	} else if (N == K) {
		for (int i = K - 1; i >= 0; i--) {
			bitmask[0][i] = '0';
		}
		return bitmask;
	} else if (K == 0) {
		for (int i = N - 1; i >= 0; i--) {
			bitmask[0][i] = '1';
		}
		return bitmask;
	}
	int t1 = C(N - 1, K - 1);
	for (int i = 0; i < t1; i++) {
		bitmask[i][N - 1] = '0';
	}
	int t2 = C(N - 1, K);
	for (int i = t1; i < t1 + t2; i++) {
		bitmask[i][N - 1] = '1';
	}
	all_combination(N - 1, K - 1, bitmask);
	all_combination(N - 1, K, bitmask + t1);
	return bitmask;
}

Status BPuzzle::solve() {
	if(sat.solve() == done){
		for(int x = 1; x <= SIZE; x++)
			for(int y = 1; y <= SIZE; y++)
				table[x - 1][y - 1] = sat.result.literals[flatten(x, y)].val;
		return done;
	}else{
		return unholdable;
	}
}

void BPuzzle::print() {
	for(int x = 0; x < SIZE; x++){
		for(int y = 0; y < SIZE; y++){
			if(table[x][y] == positive)
				printf("1 ");
			else if(table[x][y] == negative)
				printf("0 ");
			else
				printf("  ");
		}
		printf("\n");
	}
}

void BPuzzle::fsave(const char *filename) {
	FILE * fp;
	if((fp = fopen(filename, "w")) == nullptr){
		printf("error occurred while opening file %s", filename);
		exit(-1);
	}
	for(int x = 0; x < SIZE; x++){
		for(int y = 0; y < SIZE; y++){
			if(table[x][y] == positive)
				fprintf(fp, "1 ");
			else if(table[x][y] == negative)
				fprintf(fp, "0 ");
			else
				fprintf(fp, "  ");
		}
		fprintf(fp, "\n");
	}
}

BPuzzle::~BPuzzle() {
	for(int i = 0; i < SIZE; i++)
		delete table[i];
	delete table;
	for(int i = 0; i < SIZE; i++)
		delete flattenTable[i];
	delete flattenTable;
	for(int i = 0; i < sxyub; i++)
		delete CNF::map[i];
	delete CNF::map;
}