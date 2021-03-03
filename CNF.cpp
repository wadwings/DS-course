//
// Created by WADwi on 2021/3/2.
//
#include "CNF.h"

CNF::CNF(char *filename) {
	char c;
	int literals_len, clauses_len, temp;
	FILE *fp;
	if ((fp = fopen(filename, "r")) == nullptr) {
		printf("error occurred when open file %s", filename);
		exit(-1);
	};
	while (!feof(fp)) {
		c = fgetc(fp);
		if (c == 'p')
			break;
		while (c != '\n') {
			c = fgetc(fp);
		}
	}
	for (int i = 0; i < 5; i++)
		fgetc(fp);
	fscanf(fp, "%d %d", &literals_len, &clauses_len);
	this->clauses_len = clauses_len;
	this->literals_len = literals_len;
	this->literals = (Literal *) calloc(sizeof(Literal), literals_len);
	for (int index = 0; index < literals_len; index++) {
		literals[index] = {index + 1};
	}
	Clause *cur = clauses = (Clause *) calloc(sizeof(Clause), 1), *pre;
	for (int index = 0; index < clauses_len; index++) {
		fscanf(fp, "%d", &temp);
		int count = 0;
		cur->count = 0;
		while (temp != 0) {
			cur->count++;
			if (count == 0)
				cur->literals = (Literal *) calloc(sizeof(Literal), 1);
			else
				cur->literals = (Literal *) realloc(cur->literals, sizeof(Literal) * (1 + count));
			cur->literals[count++] = {abs(temp), temp > 0 ? positive : negative};
			this->literals[abs(temp) - 1].count++;
			this->literals[abs(temp) - 1].pol += temp > 0 ? 1 : -1;
			fscanf(fp, "%d", &temp);
		}
		cur->next = (Clause *) calloc(sizeof(Clause), 1);
		pre = cur;
		cur = cur->next;
	}
	free(pre->next);
	pre->next = nullptr;
};

CNF::CNF(const CNF &src) : literals_len(src.literals_len), clauses_len(src.clauses_len) {
	this->literals = (Literal *) calloc(sizeof(Literal), this->literals_len);
	for (int index = 0; index < literals_len; index++) {
		literals[index] = src.literals[index];
	}
	if(src.clauses){
		auto cur = src.clauses;
		clauses = (Clause *) calloc(sizeof(Clause), 1);
		Clause *cnf_cur = clauses, *cnf_pre;
		while (cur) {
			cnf_cur->count = cur->count;
			cnf_cur->literals = (Literal *) calloc(sizeof(Literal), cnf_cur->count);
			for (int i = 0; i < cur->count; i++)
				cnf_cur->literals[i] = cur->literals[i];
			cnf_cur->next = (Clause *) calloc(sizeof(Clause), 1);
			cnf_pre = cnf_cur;
			cnf_cur = cnf_cur->next;
			cur = cur->next;
		}
		free(cnf_cur);
		cnf_pre->next = nullptr;
	}else{
		clauses = nullptr;
	}

};

CNF &CNF::operator=(const CNF &src) {
	if (this == &src)
		return *this;
	this->literals_len = src.literals_len;
	this->clauses_len = src.clauses_len;
	this->literals = (Literal *) calloc(sizeof(Literal), this->literals_len);
	for (int index = 0; index < literals_len; index++) {
		literals[index] = src.literals[index];
	}
	if(src.clauses){
		auto cur = src.clauses;
		Clause *cnf_cur = clauses = (Clause *) calloc(sizeof(Clause), 1), *cnf_pre;
		while (cur) {
			cnf_cur->count = cur->count;
			cnf_cur->literals = (Literal *) calloc(sizeof(Literal), cnf_cur->count);
			for (int i = 0; i < cur->count; i++)
				cnf_cur->literals[i] = cur->literals[i];
			cnf_cur->next = (Clause *) calloc(sizeof(Clause), 1);
			cnf_pre = cnf_cur;
			cnf_cur = cnf_cur->next;
			cur = cur->next;
		}
		free(cnf_cur);
		cnf_pre->next = nullptr;
	}else{
		this->clauses = nullptr;
	}

	return *this;
};

void safe_strcat(char *_destination, char *src, int &size) {
	while (size <= strlen(_destination) + strlen(src) + 3) {
		_destination = (char *) realloc(_destination, sizeof(char) * size * 2);
		size *= 2;
	}
	strcat(_destination, src);
}

char *CNF::to_string() {
	char *str = (char *) malloc(sizeof(char) * 1000000);
	int size = 7000000;
	strcpy(str, "p cnf ");
	char temp[10];
	sprintf(temp, "%d", this->literals_len);
	safe_strcat(str, temp, size);
	safe_strcat(str, " ", size);
	sprintf(temp, "%d", this->clauses_len);
	safe_strcat(str, temp, size);
	safe_strcat(str, "\n", size);
	auto cur = clauses;
	while (cur) {
		for (int i = 0; i < cur->count; i++) {
			sprintf(temp, "%d", cur->literals[i].id * cur->literals[i].val);
			safe_strcat(str, temp, size);
			safe_strcat(str, " ", size);
		}
		safe_strcat(str, "0\n", size);
		cur = cur->next;
	}
	printf("%s", str);
	return str;
};

int CNF::real_len() {
	auto cur = clauses;
	int count = 0;
	while(cur){
		count++;
		cur = cur->next;
	}return count;
}
Literal &Literal::operator=(const Literal &src) = default;
//CNF::~CNF(){
//	free(this->literals);
//	Clause * pre, * cur;
//	cur = this->clauses;
//	while(cur){
//		pre = cur;
//		cur = cur->next;
//		free(pre);
//	}
//}