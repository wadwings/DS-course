//
// Created by WADwi on 2021/3/1.
//
#ifndef PROJECT_DS_CNF_H
#define PROJECT_DS_CNF_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef enum Value {
	positive = 1,
	negative = 0,
	undefined = -1
} Value;

typedef struct Literal {
	int id{};
	Value val = undefined;
	int count{};
	int pol{};
	Literal() = default;
	Literal(int _id = 0, Value _val = undefined, int _count = 0, int _pol = 0) : id(_id), val(_val), count(_count), pol(_pol) {};
	Literal& operator=(const Literal & src);
}Literal;

typedef struct Clause{
	Literal ** literals = NULL;
	int count = 0;
	struct Clause * next = NULL;
	Clause() = default;
	Clause(int _size);
	int size()const{return count;};
	~Clause(){free(literals);};
}Clause;

typedef struct CNF {
	Literal * literals;
	Clause * clauses;
	Clause * tail;
	static Literal ** map;
	int literals_len;
	int clauses_len;
  Literal ** bind;
	CNF();
	CNF(char *filename);
	CNF(const CNF & src);
	void add_clauses(Clause * src);
	static void remove_clauses(Clause * src);
	char* to_string();
	CNF& operator=(const CNF & src);
	int real_len();
	~CNF();
} CNF;


#endif //PROJECT_DS_CNF_H
