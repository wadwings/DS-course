//
// Created by WADwi on 2021/3/2.
//

#include "BPuzzle.h"
#include "BPuzzleGen.h"
#include <unistd.h>
#include <time.h>
//#include <string>

void case1(){
	char t[100];
	printf("CNF file path:");
	scanf("%s", t);
	int t1 = clock();
	DPLL dpll(t);
	dpll.origin.jiexi();
	if (dpll.solve() != unholdable) {
		for (int i = 0; i < dpll.result.literals_len; i++) {
			printf("变元%d的值为%d\n", i + 1, dpll.result.literals[i].val);
		}
	};
	printf("Print to Res file?(Y/N)\n");
	getchar();
	char c;
	scanf("%c", &c);
	int t2 = clock();
	switch (c) {
		case 'Y':
			dpll.print_res(t, (float)(t2 - t1) / CLOCKS_PER_SEC * 1000);
			break;
		default:
			break;
	}
	printf("%f ms", (double)(t2 - t1) / CLOCKS_PER_SEC * 1000);
}

void case2(){
	char t[100];
	int size;
	printf("Puzzle file path & size:");
	scanf("%s %d", t, &size);
	int t1 = clock();
	BPuzzle bpuzzle(t, size);
	//	bpuzzle.sat.origin.to_string();
	if(bpuzzle.solve() != unholdable){
		bpuzzle.print();
	};
	int t2 = clock();
	printf("%f ms", (double)(t2 - t1) / CLOCKS_PER_SEC * 1000);
}

void case3(){
	char t[100];
	int size;
	printf("Puzzle file path & size:");
	scanf("%s %d", t, &size);
	int t1 = clock();
	BPuzzleGen generator(t, size);
	generator.generate();
	generator.print();
	generator.fsave("/home/wings/puzzle");
	BPuzzle bpuzzle("/home/wings/puzzle", size);
	if(bpuzzle.solve() != unholdable){
		bpuzzle.print();
	};
	int t2 = clock();
	printf("%f ms", (double)(t2 - t1) / CLOCKS_PER_SEC * 1000);
}

int main(int argc, char *argv[]) {
	int c;
	printf("1 for solve CNF\n2 for solve Puzzzle\n3 for generate Puzzle\n");
	c = getc(stdin);
	switch (c) {
		case '1':
			case1();
			break;
		case '2':
			case2();
			break;
		case '3':
			case3();
			break;
		default:
			printf("wrong input!");
			break;
	}
	return 0;
}