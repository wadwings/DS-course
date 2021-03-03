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
		pre = cur = cnf.clauses;
		while(cur){
			if(!cur->size())
				return unholdable;
			else if(cur->size() == 1){
				has_unit = true;
				Literal unit = cur->literals[0];
				cnf.literals[unit.id - 1].val = unit.val;
				Status res = assign(cnf, unit.id);
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
	Clause *pre,*cur;
	pre = cur = clauses;
	bool next = true;
	while(cur) {
		for (int i = 0; i < cur->size(); i++) {
			if (id != cur->literals[i].id) {
				continue;
			}
			if (val == cur->literals[i].val) {
				if (pre == cur){
					cnf.clauses = cur->next;
					pre = cur = clauses;
					next = false;
				}else{
					pre->next = cur->next;
					cur = pre;
				}
				--cnf.clauses_len;
//				printf("%d %d\n", cnf.clauses_len, cnf.real_len());
//				fflush(stdout);
				if (cnf.clauses == nullptr) {
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
		if(next){
			pre = cur;
			cur = cur->next;
		}else{
			next = true;
		}
	}
	return Status::pending;
}
Status DPLL::solve(){
	CNF cnf = origin; // keep original cnf clean
	auto status = perform_dpll(cnf);
	if (status == pending) {
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
	for(int j = 0; j < 2; j++){
		CNF cur_cnf = cnf;
		Literal &var_ref = cur_cnf.literals[var - 1];
		var_ref.val = boolean[(var_ref.pol >= 0) ? j : ((j + 1) % 2)];
		status = assign(cur_cnf, var);
		if (status == holdable){
			save_result(cnf, status);
			return done;
		}else if(status == unholdable){
			continue;
		}
		status = perform_dpll(cur_cnf);
		if(status == done)
				return status;
	}
	return pending;
}
void DPLL::save_result(CNF &cnf, int status) {
	if (status == holdable) {
		result = cnf;
	}
}
int DPLL::choose(CNF &cnf){
	return Strategies::linear(cnf);
}