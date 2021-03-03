//
// Created by WADwi on 2021/3/2.
//

#include "DPLL.h"
#include <time.h>
int main(){
	DPLL dpll("/mnt/d/Desktop/程序设计综合课程设计任务及指导学生包/SAT测试备选算例/不满足算例/php-010-008.shuffled-as.sat05-1171.cnf");
	int i1 = clock();
	if(dpll.solve() != unholdable){
		for(int i = 0; i < dpll.result.literals_len; i++){
			printf("变元%d的值为%d\n", i + 1, dpll.result.literals[i].val);
		}
	};
	int i2 = clock();
	printf("%f s", (i2 - i1) / 1000000.0);
	fflush(stdout);
//	CNF cnf("D:\\S\\problem1-20.cnf");
//	CNF cnf1 = cnf;
//	cnf1.to_string();
	return 0;
}