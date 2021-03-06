//
// Created by WADwi on 2021/3/2.
//

#include "BPuzzle.h"
#include <time.h>
int main(){
	//system("chcp 65001");//终端输出中文
	int i1 = clock();
//	DPLL dpll("/mnt/d/S/Problem9-100.cnf");
//	if(dpll.solve() != unholdable){
//		for(int i = 0; i < dpll.result.literals_len; i++){
//			printf("变元%d的值为%d\n", i + 1, dpll.result.literals[i].val);
//		}
//	};
  BPuzzle bpuzzle("/mnt/d/S/binarypuzzle3");
  bpuzzle.sat.origin.to_string();
	if(bpuzzle.solve() != unholdable){
		bpuzzle.print();
	};
	int i2 = clock();
	printf("%f s", (i2 - i1) / 1000000.0);
	fflush(stdout);
//	CNF cnf("/mnt/d/Desktop/程序设计综合课程设计任务及指导学生包/SAT测试备选算例/满足算例/S/problem1-20.cnf");
//	CNF cnf1 = cnf;
//	cnf1.to_string();
	return 0;
}