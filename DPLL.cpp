//
// Created by WADwi on 2021/3/1.
//

#include "DPLL.h"
DPLL::DPLL(const CNF & src) : origin(src){};

Status DPLL::rsc(CNF &cnf) {
	Clause *pre,* cur;
	bool has_unit = true;
	while(has_unit){
		has_unit = false;
		pre = cnf.clauses;
		cur = cnf.clauses->next;
		while(cur){
			if(!cur->size())
				return unholdable;
			else if(cur->size() == 1){
				has_unit = true;
				auto &unit = cur->literals[0];
				cnf.literals[unit->id - 1].val = unit->val;
				Status res = assign(cnf, unit->id);
				if(res == holdable || res == unholdable)
					return res;
				break;
			}
			pre = cur;
			cur = cur->next;
		}
	}
	if(pre == cur)
		return holdable;
	return pending;
}

Status DPLL::assign(CNF &cnf, int id) {
	Value val = cnf.literals[id - 1].val;
	auto &clauses = cnf.clauses;
	Clause *pre = clauses, *cur = clauses->next;
	while(cur) {
		for (int i = 0; i < cur->size(); i++) {
			if (id != cur->literals[i]->id) {
				continue;
			}
			if (val == cur->literals[i]->val){
				cnf.remove_clauses(pre);
				cur = pre;
				cnf.clauses_len--;
//			printf("%d %d\n", cnf.clauses_len, cnf.real_len());
//			fflush(stdout);
				if (cnf.clauses->next == nullptr) {
					return Status::holdable;
				}
				break;
			} else {
				for (int x = i + 1; x < cur->size(); x++)
					cur->literals[x - 1] = cur->literals[x];
				cur->count--;
				if (!cur->count) {
					return Status::unholdable;
				}
				break;
			}
		}
		pre = cur;
		cur = cur->next;
	}
	return Status::pending;
}
Status DPLL::solve(){
	CNF cnf = origin; // keep original cnf clean
	auto status = perform_dpll(cnf);
	if (status == pending || check(result)) {
		return Status::unholdable;
	} else {
		return status;
	}
}

Status DPLL::perform_dpll(CNF &cnf) {
	Status status = rsc(cnf);
	if(status == holdable){
		this->result = cnf;
		return done;
	}else if(status == unholdable)
		return pending;
	int var = choose(cnf);
	Value boolean[] = {positive, negative};
//	Clause * clauses = (Clause *)calloc(sizeof(Clause), 1);
//	int clauses_len = cnf.clauses_len;
//	Clause * cur = clauses->next = (Clause *)calloc(sizeof(Clause), 1), * pre = clauses, * cur_cnf = cnf.clauses->next;
//	while(cur_cnf){
//		cur->count = cur_cnf->count;
//		cur->literals = (Literal *)calloc(sizeof(Clause), cur_cnf->count);
//		for(int i = 0; i < cur_cnf->count; i++)
//			cur->literals[i] = cur_cnf->literals[i];
//		cur->next = (Clause *)calloc(sizeof(Clause), 1);
//		pre = cur;
//		cur = cur->next;
//		cur_cnf = cur_cnf->next;
//	}
//	free(pre->next);
//	pre->next = nullptr;
	for(int j = 0; j < 2; j++){
		CNF cur_cnf = cnf;
		Literal &var_ref = cur_cnf.literals[var - 1];
		var_ref.val = boolean[(var_ref.pol >= 0) ? j : ((j + 1) % 2)];
		status = assign(cur_cnf, var);
		if (status == holdable){
			save_result(cur_cnf, status);
			return done;
		}else if(status == unholdable){
			continue;
		}
		status = perform_dpll(cur_cnf);
		if(status == done)
				return status;
	}
//	cnf.clauses_len = clauses_len;
//	cnf.clauses = clauses;
//	cnf.literals[var - 1].val = undefined;
	return pending;
}
void DPLL::save_result(CNF &cnf, int status) {
	if (status == holdable) {
		result = cnf;
	}
}
int DPLL::choose(CNF &cnf){
	return CNF_Strategies::frequential(cnf);
}

Status DPLL::check(const CNF & src){
	auto cur = origin.clauses->next;
	while(cur){
		bool flag = false;
		for(int i = 0; i < cur->count; i++)
			if(cur->literals[i]->val == src.literals[cur->literals[i]->id - 1].val){
				flag = true;
				break;
			}
		if(!flag)
			return unholdable;
		cur = cur->next;
	}
	//printf("Solution Passed!\n");
	return holdable;
}