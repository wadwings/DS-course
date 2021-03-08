//
// Created by WADwi on 2021/3/7.
//

#include "BPuzzleGen.h"
#include <random>
#include <time.h>
int BPuzzleGen::SIZE = 1;
BPuzzleGen::BPuzzleGen(const char *filename, int size) {
	SIZE = size;
	FILE * fp;
	int temp;
	if((fp = fopen(filename, "r")) == nullptr){
		printf("error occurred while opening file %s", filename);
		exit(-1);
	}
	table = (Value **)calloc(sizeof(Value *), SIZE);
	for(int x = 0 ; x < SIZE; x++){
		table[x] = (Value *)calloc(sizeof(Value), SIZE);
		for(int y = 0; y < SIZE; y++){
			fscanf(fp, "%d", &temp);
			table[x][y] = temp == 0 ? negative : positive;
		}
	}
}

Status BPuzzleGen::generate() {
	int map[SIZE * SIZE];
	int dis_map[SIZE * SIZE];
	int count = SIZE * SIZE;
	int min = SIZE * SIZE * 0.3;
	auto *seed = (time_t*)calloc(sizeof(time_t), 1);
	auto _try = (int **)calloc(sizeof(int * ), count + 1);
	for(int i = 0; i < count + 1; i++)
		_try[i] = (int *)calloc(sizeof(int), SIZE * SIZE);
	time(seed);
	srand(*seed);
	for(int i = 0; i < SIZE * SIZE; i++)
		map[i] = i;
	int *disable = (int *)calloc(sizeof(int), count + 1);
	while(count >= min){
		int i = rand() % count;
		int index = map[i];
		if(disable[count] ==  count){
			memset(_try[count], 0, sizeof(int) * SIZE * SIZE);
			disable[count] = 0;
 			map[count] = abs(dis_map[SIZE * SIZE - count - 1]);
 			table[map[count] / SIZE][map[count] % SIZE] = dis_map[SIZE * SIZE - count - 1] > 0 ? positive : negative;
 			dis_map[SIZE * SIZE - count - 1] = 0;
			++count;
			print();
			fflush(stdout);
		}
		if(_try[count][index] == 1)
			continue;
		table[index / SIZE][index % SIZE] = table[index / SIZE][index % SIZE] == positive ? negative : positive;
		if(only_check() != unholdable){
			if(_try[count][index] == 0){
				_try[count][index] = 1;
				++disable[count];
				printf("%d-%d\n", count, disable[count]);
				fflush(stdout);
			}
			table[index / SIZE][index % SIZE] = table[index / SIZE][index % SIZE] == positive ? negative : positive;
		}else{
			if(_try[count][index] == 0){
				_try[count][index] = 1;
				++disable[count];
			}
			dis_map[SIZE * SIZE - count] = map[i] * (table[index / SIZE][index % SIZE] == positive ? -1 : 1);
			for(int x = i + 1; x < count; x++){
				map[x - 1] = map[x];
			}
			--count;
			table[index / SIZE][index % SIZE] = undefined;
			print();
			fflush(stdout);
		}
	}
	free(seed);
	return done;
}

Status BPuzzleGen::only_check() {
	solver = new BPuzzle(table, SIZE);
	Status temp = solver->solve();
	delete solver;
	return temp;
}

Status BPuzzleGen::print() {
	for(int x = 0; x < SIZE; x++){
		for(int y = 0; y < SIZE; y++){
			if(table[x][y] == positive)
				printf("1  ");
			else if(table[x][y] == negative)
				printf("0  ");
			else
				printf("_  ");
		}
		printf("\n");
	}
	return done;
}

Status BPuzzleGen::fsave(const char *filename) {
	FILE * fp;
	if((fp = fopen(filename, "w")) == nullptr){
		printf("error occurred while opening file %s", filename);
		exit(-1);
	}
	for(int x = 0; x < SIZE; x++){
		for(int y = 0; y < SIZE; y++){
			if(table[x][y] == positive)
				fprintf(fp, "%d %d 1\n", x+1, y+1);
			else if(table[x][y] == negative)
				fprintf(fp, "%d %d 0\n", x+1, y+1);
		}
	}
	fclose(fp);
	return done;
}

BPuzzleGen::~BPuzzleGen() {
	for(int i = 0; i < SIZE; i++)
		delete table[i];
	delete table;
}