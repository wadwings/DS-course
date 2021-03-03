//
// Created by WADwi on 2021/3/1.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef PROJECT_DS_CNF_H
#define PROJECT_DS_CNF_H
typedef enum Value {
	positive = 1,
	negative = -1,
	undefined = 0
} Value;

typedef struct Literal {
	int id;
	Value val;
	int count;
	int pol;
	Literal() = default;
	Literal(int _id = 0, Value _val = undefined, int _count = 0, int _pol = 0) : id(_id), val(_val), count(_count), pol(_pol) {};
	Literal& operator=(const Literal & src);
}Literal;

typedef struct Clause{
	Literal * literals = NULL;
	int count = 0;
	struct Clause * next = NULL;
	Clause() = default;
	int size()const{return count;};
	~Clause(){free(literals);};
}Clause;

typedef struct CNF {
	Literal * literals;
	Clause * clauses;
	int literals_len;
	int clauses_len;
	CNF() = default;
	CNF(char *filename);
	int real_len();
	CNF(const CNF & src);
	char* to_string();
	CNF& operator=(const CNF & src);
//	~CNF();
} CNF;
#endif //PROJECT_DS_CNF_H
