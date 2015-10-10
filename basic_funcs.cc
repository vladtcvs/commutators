#include "structs.h"

type_pair::type_pair()
{
	first = oper_none;
	second = oper_none;
}

type_pair::type_pair(int id)
{
	set_id(id);
}

int opid(oper_type_e op)
{
	return op - 1;
}

oper_type_e idop(int id)
{
	switch (id) {
	case 0:
		return oper_a;
	case 1:
		return oper_b;
	case 2:
		return oper_c;
	case 3:
		return oper_d;
	default:
		return oper_none;
	}
}

int delta::zero()
{
	switch (type) {
	case comm_k3:
	case comm_k4:
	case comm_k5:
	case comm_k6:
		if (arg1 == arg2) // delta(eps) = 0
			return 1;
		break;
	case comm_k1:
	case comm_k2:
		if (arg1 == arg2) // delta(0) - it's very, very bad
			return -1;
		break;
	}
	return 0;
}

int delta::convert()
{
	int s = 0;
	if (type == comm_k5 || type == comm_k6) {
		int arg_t = arg1;
		arg1 = arg2;
		arg2 = arg_t;
		if (type == comm_k5)
			s = 1;
		else
			s = -1;
	}
	return s;
}

int delta::operator%(delta d2)
{
	if (type == comm_k1 && d2.type != comm_k1)
		return 0;
	if (type == comm_k2 && d2.type != comm_k2)
		return 0;
	if (type == comm_k3 && d2.type != comm_k5 && d2.type != comm_k3)
		return 0;
	if (type == comm_k5 && d2.type != comm_k3 && d2.type != comm_k5)
		return 0;
	if (type == comm_k4 && d2.type != comm_k6 && d2.type != comm_k4)
		return 0;
	if (type == comm_k6 && d2.type != comm_k4 && d2.type != comm_k6)
		return 0;
	
	if (arg1 == d2.arg1 && arg2 == d2.arg2)
		return 1;
	if (arg1 == d2.arg2 && arg2 == d2.arg1) {
		if (type == comm_k1 || type == comm_k3 || type == comm_k5)
			return 1;
		return -1;
	}
	return 0;
}


void coeff::convert()
{
	int c = 1;
	for (auto it = deltas.begin(); it != deltas.end(); it++) {
		int z = (*it).zero();
		if (z == 1) {
			k = 0;
			deltas.clear();
			break;
		} else if (z == -1) {
			throw "delta(0)";
		}
		int p = (*it).convert();
		if (p)
			c *= p;
	}
	k *= c;
}

int coeff::operator%(coeff c2)
{
	int pp = 1;
	size_t sz = deltas.size(), i, j, k;
	if (sz != c2.deltas.size())
		return 0;
	std::vector<int> delta_map(sz);
	std::vector<int> delta_cf(sz);
	for (i = 0; i < sz; i++) {
		delta_map[i] = -1;
		delta_cf[i] = 0;
		for (j = 0; j < sz; j++) {
			int p = deltas[i] % c2.deltas[j];
			if (p) {
				bool was = false;
				for (k = 0; k < i; k++)
					if (delta_map[k] == (int)j)
						was = true;
				if (was == false) {
					delta_map[i] = j;
					delta_cf[i] = p;
					break;
				}
			}
		}
		if (delta_map[i] == -1)
			return 0;
	}
	for (i = 0; i < sz; i++)
		pp *= delta_cf[i];
	return pp;
}

void coeff::substitute(int old_aid, int new_aid)
{
	std::vector<delta>::iterator it;
	for (it = deltas.begin(); it != deltas.end(); it++) {
		if ((*it).arg1 == old_aid)
			(*it).arg1 = new_aid;
		if ((*it).arg2 == old_aid)
			(*it).arg2 = new_aid;
	}
}

coeff coeff::operator*=(coeff& c2)
{
	k *= c2.k;
	std::vector<delta>::iterator it = c2.deltas.begin();
	while (it != c2.deltas.end()) {
		deltas.push_back(*it);
		it++;
	}
	return *this;
}

int extra_vars::new_extra()
{
	dvar.push_back(false);
	used.push_back(false);
	return -(dvar.size() - 1) - 2;
}

void coeff_list::substitute(int old_aid, int new_aid)
{
	std::list<coeff>::iterator it;
	for (it = cf.begin(); it != cf.end(); it++) {
		(*it).substitute(old_aid, new_aid);
	}
}

void coeff_list::rmdouble()
{
	bool changed;

	do {
		changed = false;
		std::list<coeff>::iterator it1, it2;
		for (it1 = cf.begin(); it1 != cf.end(); it1++) {
			if ((*it1).k == 0) {
				changed = true;
				cf.erase(it1);
				goto again;
			}
		}
		for (it1 = cf.begin(); it1 != cf.end(); it1++) {
			for (it2 = cf.begin(); it2 != it1; it2++) {
				int p = (*it2)%(*it1);
				if (p) {
					changed = true;
					(*it2).k += (*it1).k * p;
					cf.erase(it1);
					goto again;
				}
			}
		}
		again:
		; // we need smth arter "again:"
	} while (changed);
}

oper::oper()
{
	type = oper_none;
	argid = -1;
}

oper::oper(oper_type_e ntype, int nargid)
{
	type = ntype;
	argid = nargid;
}

bool oper::operator==(oper op2)
{
	if (type == op2.type && argid == op2.argid)
		return true;
	return false;
}

bool oper::operator!=(oper op2)
{
	if (type != op2.type || argid != op2.argid)
		return true;
	return false;
}

int type_pair::id()
{
	return opid(first) * 4 + opid(second);
}

void type_pair::set_id(int nid)
{
	first = idop(nid / 4);
	second = idop(nid % 4);
}

pair::pair()
{}

pair::pair(oper_type_e nt1, int na1, oper_type_e nt2, int na2)
	:first(nt1, na1),
	second(nt2, na2)
{}

int pair::operator%(pair p2)
{
	if (first != p2.first && first != p2.second)
		return 0;
	if (second != p2.first && second != p2.second)
		return 0;
	if (first == p2.first && second == p2.second) {
		return 1;
	} else if (first == p2.second && second == p2.first) {
		coeff cf = commutate(first, second);
		if (cf.k == 0)
			return 1;
		return 0;
	}
	return 0;
}


monom::monom()
{
	c.k = 0;
}

monom::monom(double nc, oper_type_e nt1, int na1, oper_type_e nt2, int na2)
:opers(nt1, na1, nt2, na2)
{
	c.k = nc;
}

int monom::operator %(monom &m)
{
	if (!(opers % m.opers))
		return 0;
	return c % m.c;
}

polynom polynom::operator=(monom mon)
{
	monoms.clear();
	monoms.push_back(mon);
	return *this;
}

polynom polynom::operator+=(polynom p2)
{
	std::list<monom>::iterator it = p2.monoms.begin();
	while (it != p2.monoms.end()) {
		monoms.push_back(*it);
		it++;
	}
	rmdouble();
	return *this;
}

void polynom::rmdouble()
{
	bool nothing;
	do {
		nothing = true;
		std::list<monom>::iterator it = monoms.begin();
		while (it != monoms.end()) {
			if ((*it).c.k == 0) {
				std::list<monom>::iterator it_old = it;
				it++;
				monoms.erase(it_old);
				nothing = false;
			}
			it++;
		}
		it = monoms.begin();
		while (it != monoms.end()) {
			(*it).c.convert();
			it++;
		}
		it = monoms.begin();
		while (it != monoms.end()) {
			std::list<monom>::iterator it2 = it;
			it2++;
			while (it2 != monoms.end()) {
				int cc = (*it)%(*it2);
				if (cc) {
					(*it).c.k += (*it2).c.k * cc;
					std::list<monom>::iterator it_old = it2;
					it2++;
					monoms.erase(it_old);
					nothing = false;
				} else {
					it2++;
				}
			}
			it++;
		}
	} while (nothing == false);
}
