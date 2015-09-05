#include <iostream>
#include <vector>
#include <list>
#include <sstream>
#include <map>

enum comm_type_e {
	comm_k1 = 0,
	comm_k2,
	comm_k3,
	comm_k4,
	comm_k5,
	comm_k6,
};

enum oper_type_e {
	oper_none = 0,
	oper_a,
	oper_b,
	oper_c,
	oper_d,
};

bool print_full = true;
bool print_tex = true;

struct delta {
	comm_type_e type;
	int arg1, arg2;
	int operator % (delta d2);
	int convert();
};

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

std::basic_ostream<char>& operator << (std::basic_ostream<char>& ss, delta& d)
{
	if (print_full) {
		std::string delta = print_tex ? "\\delta" : "δ";
		std::string eps = print_tex ? "\\epsilon" : "ɛ";
		if (d.type == comm_k2 || d.type == comm_k4 || d.type == comm_k6)
			ss << delta << "'(f";
		else
			ss << delta << "(f";
		for (int i = 0; i < d.arg1; i++)
			ss << "\'";
		if (d.type == comm_k3 || d.type == comm_k4)
			ss << "+" << eps;
		ss << "-f";
		for (int i = 0; i < d.arg2; i++)
			ss << "\'";
		if (d.type == comm_k5 || d.type == comm_k6)
			ss << "-" << eps;
		ss <<")";
	} else {
		switch(d.type) {
		case comm_k1:
			ss<<"k1";
			break;
		case comm_k2:
			ss<<"k2";
			break;
		case comm_k3:
			ss<<"k3";
			break;
		case comm_k4:
			ss<<"k4";
			break;
		case comm_k5:
			ss<<"k5";
			break;
		case comm_k6:
			ss<<"k6";
			break;
		}
		if (print_tex) {
			ss << "_{"<<d.arg1<<","<<d.arg2<<"}";
		} else {
			ss << " ";
		}
	}
	return ss;
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

struct coeff {
	double k;
	std::vector<delta> deltas;
	int operator % (coeff c2);
	coeff operator *= (coeff& c2);
	void convert();
};

void coeff::convert()
{
	int c = 1;
	for (auto it = deltas.begin(); it != deltas.end(); it++) {
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


struct oper {
	oper_type_e type;
	int argid;
	oper();
	oper(oper_type_e ntype, int nargid);
	bool operator == (oper op2);
	bool operator != (oper op2);
};

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

std::basic_ostream<char>& operator << (std::basic_ostream<char>& ss, oper& op)
{
	switch (op.type) {
	case oper_a:
		ss << "a";
		break;
	case oper_b:
		ss << "b";
		break;
	case oper_c:
		ss << "c";
		break;
	case oper_d:
		ss << "d";
		break;
	case oper_none:
		throw "Uninitialized operator";
	}
	if (print_full) {
		ss << "(f";
		if (op.argid < 0)
			throw "Uninitialized operator";
		for (int i = 0; i < op.argid; i++)
			ss << "\'";
		ss << ")";
	} else if (print_tex) {
		ss << "_{" << op.argid << "}";
	}
	return ss;
}

coeff commutate(oper op1, oper op2)
{
	coeff res;
	delta d;
	switch (op1.type) {
	case oper_a:
		switch (op2.type) {
		case oper_a:
			res.k = 0;
			break;
		case oper_b:
			res.k = 0;
			break;
		case oper_c:
			res.k = 1;
			d.type = comm_k1;
			d.arg1 = op1.argid;
			d.arg2 = op2.argid;
			break;
		case oper_d:
			res.k = 1;
			d.type = comm_k2;
			d.arg1 = op1.argid;
			d.arg2 = op2.argid;
			break;
		case oper_none:
			throw "Not initialized operator";
		}
		break;
	case oper_b:
		switch (op2.type) {
		case oper_a:
			res.k = 0;
			break;
		case oper_b:
			res.k = 0;
			break;
		case oper_c:
			res.k = 1;
			d.type = comm_k3;
			d.arg1 = op1.argid;
			d.arg2 = op2.argid;
			break;
		case oper_d:
			res.k = 1;
			d.type = comm_k4;
			d.arg1 = op1.argid;
			d.arg2 = op2.argid;
			break;
		case oper_none:
			throw "Not initialized operator";
		}
		break;
		break;
	case oper_c:
		switch (op2.type) {
		case oper_a:
			res.k = -1;
			d.type = comm_k1;
			d.arg1 = op1.argid;
			d.arg2 = op2.argid;
			break;
		case oper_b:
			res.k = -1;
			d.type = comm_k5;
			d.arg1 = op1.argid;
			d.arg2 = op2.argid;
			break;
		case oper_c:
			res.k = 0;
			break;
		case oper_d:
			res.k = 0;
			break;
		case oper_none:
			throw "Not initialized operator";
		}
		break;
	case oper_d:
		switch (op2.type) {
		case oper_a:
			res.k = 1;
			d.type = comm_k2;
			d.arg1 = op1.argid;
			d.arg2 = op2.argid;
			break;
		case oper_b:
			res.k = 1;
			d.type = comm_k6;
			d.arg1 = op1.argid;
			d.arg2 = op2.argid;
			break;
		case oper_c:
			res.k = 0;
			break;
		case oper_d:
			res.k = 0;
			break;
		case oper_none:
			throw "Not initialized operator";
		}
		break;
	case oper_none:
		throw "Not initialized operator";
	}
	res.deltas.push_back(d);
	return res;
}

std::basic_ostream<char>& operator << (std::basic_ostream<char>& ss, coeff& cf)
{
	std::vector<delta>::iterator it;
	if (cf.k == 0) {
		ss << "0";
		return ss;
	}
	if (cf.k != 1 && cf.k != -1) {
		ss << cf.k;
		if (cf.deltas.size() > 0)
			ss << (print_tex ? "\\cdot{}" : "*");
	} else if (cf.k == -1)
		ss << "-";
	it = cf.deltas.begin();
	while (it != cf.deltas.end()) {
		delta d = *it;
		ss << d;
		it++;
	}
	return ss;
}

struct pair {
	oper first, second;
	int operator % (pair p2);
	pair();
	pair(oper_type_e nt1, int na1, oper_type_e nt2, int na2);
};

struct type_pair {
	oper_type_e first, second;
};

std::basic_ostream<char>& operator << (std::basic_ostream<char>& ss, type_pair& tp)
{
	switch (tp.first) {
	case oper_a:
		ss << "a";
		break;
	case oper_b:
		ss << "b";
		break;
	case oper_c:
		ss << "c";
		break;
	case oper_d:
		ss << "d";
		break;
	case oper_none:
		throw "Uninitialized operator";
	}

	if (!print_tex)
		ss << " ";

	switch (tp.second) {
	case oper_a:
		ss << "a";
		break;
	case oper_b:
		ss << "b";
		break;
	case oper_c:
		ss << "c";
		break;
	case oper_d:
		ss << "d";
		break;
	case oper_none:
		throw "Uninitialized operator";
	}
	return ss;
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

std::basic_ostream<char>& operator << (std::basic_ostream<char>& ss, pair& p)
{
	ss << p.first << (print_tex ? " " : "*") << p.second;
	return ss;
}

struct monom {
	pair opers;
	coeff  c;
	monom();
	monom(double nc, oper_type_e nt1, int na1, oper_type_e nt2, int na2);
	int operator %(monom &m);
};

monom::monom()
{
	c.k = 0;
}

monom::monom(double nc, oper_type_e nt1, int na1, oper_type_e nt2, int na2)
:opers(nt1, na1, nt2, na2)
{
	c.k = nc;
}

std::basic_ostream<char>& operator << (std::basic_ostream<char>& ss, monom& mon)
{
	if (mon.c.k != 0) {
		if ((mon.c.k == -1 || mon.c.k == 1) && mon.c.deltas.size() == 0)
			ss << mon.opers;
		else
			ss << mon.c << (print_tex ? "\\cdot{}" : "*") << mon.opers;
	} else {
		ss << "0";
	}
	return ss;
}

int monom::operator %(monom &m)
{
	if (!(opers % m.opers))
		return 0;
	return c % m.c;
}

struct polynom {
	std::list<monom> monoms;
	void rmdouble();
	polynom operator += (polynom p2);
	polynom operator = (monom mon);
};

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

std::basic_ostream<char>& operator << (std::basic_ostream<char>& ss, polynom& pol)
{
	bool first = true;
	if (pol.monoms.size() == 0) {
		ss << "0";
		return ss;
	}
	std::list<monom>::iterator mon = pol.monoms.begin();
	while (mon != pol.monoms.end()) {
		if (!first) {
			ss << " + ";
			if (print_tex) {
				ss << "\\\\ + ";
			}
		}
		ss << (*mon);
		first = false;
		mon++;
	}
	return ss;
}

polynom commutate(monom J1, monom J2)
{
	polynom result;
	monom m1, m2, m3, m4, m5, m6;
	oper a = J1.opers.first, b = J1.opers.second,
			c = J2.opers.first, d = J2.opers.second;
	m1.opers.first = a;
	m1.opers.second = d;
	m1.c = commutate(b, c);
	m1.c *= J1.c;
	m1.c *= J2.c;

	m2.opers.first = a;
	m2.opers.second = c;
	m2.c = commutate(b, d);
	m2.c *= J1.c;
	m2.c *= J2.c;
	m2.c.k /= 2;

	m3.opers.first = d;
	m3.opers.second = b;
	m3.c = commutate(a, c);
	m3.c *= J1.c;
	m3.c *= J2.c;
	m3.c.k /= 2;
	
	m4.opers.first = c;
	m4.opers.second = a;
	m4.c = commutate(b, d);
	m4.c *= J1.c;
	m4.c *= J2.c;
	m4.c.k /= 2;

	m5.opers.first = b;
	m5.opers.second = d;
	m5.c = commutate(a, c);
	m5.c *= J1.c;
	m5.c *= J2.c;
	m5.c.k /= 2;

	m6.opers.first = c;
	m6.opers.second = b;
	m6.c = commutate(a, d);
	m6.c *= J1.c;
	m6.c *= J2.c;

	result.monoms.push_back(m1);
	result.monoms.push_back(m2);
	result.monoms.push_back(m3);
	result.monoms.push_back(m4);
	result.monoms.push_back(m5);
	result.monoms.push_back(m6);

	result.rmdouble();
	return result;
}

polynom commutate(polynom p1, polynom p2)
{
	polynom result;
	std::list<monom>::iterator it1, it2;

	it1 = p1.monoms.begin();
	while (it1 != p1.monoms.end()) {
		it2 = p2.monoms.begin();
		while (it2 != p2.monoms.end()) {
			result += commutate(*it1, *it2);
			it2++;
		}
		it1++;
	}
	result.rmdouble();
	return result;
}

void test1()
{
	std::vector<oper_type_e> oper_i = {oper_a, oper_b,
			oper_c, oper_d};

	for (auto a : oper_i)
	for (auto b : oper_i) {
		oper J1(a, 0);
		oper J2(b, 1);
		if (print_tex)
			std::cout << "\\comm{"<<J1<<"}{"<<J2<<"} = ";
		else
			std::cout << "["<<J1<<", "<<J2<<"] = ";
		coeff com = commutate(J1, J2);
		/*std::cout<<com<<" = ";
		com.convert();*/
		std::cout<<com;
		if (print_tex)
			std::cout << "\\\\\n";
		else
			std::cout << "\n";
	/*	coeff com2 = commutate(J2, J1);
		std::cout<<com2<<"\n";
		if (com.k == 0 && com2.k == 0)
			continue;
		if (!(c = com % com2))
			throw "Wrong comm";
		if (c*com.k != -com2.k)
			throw "Wrong comm";*/
	}
}

void test2()
{
	std::vector<oper_type_e> oper_i = {oper_a, oper_b,
			oper_c, oper_d};

	/*
	oper_type_e a, b, c, d;
	a = oper_a;
	b = oper_b;
	c = oper_c;
	d = oper_c;*/
	for (auto a : oper_i)
	for (auto b : oper_i)
	for (auto c : oper_i)
	for (auto d : oper_i)
	{
		monom J1(1, a, 0, b, 1);
		monom J2(1, c, 2, d, 3);
		if (print_tex) {
			std::cout<<"\\begin{eqnarray}\n";
			std::cout << "\\comm{"<<J1<<"}{"<<J2<<"} = ";
		} else {
			std::cout << "["<<J1<<", "<<J2<<"] = ";
		}
		
		polynom com = commutate(J1, J2);
		std::cout << com;
		std::cout << "\n";
		if (print_tex)
			std::cout<<"\\end{eqnarray}\n";
		polynom com2 = commutate(J2, J1);
		com2 += com;
		if (com2.monoms.size())
			throw "Wrong comm";
	}
}

void find_used()
{
	type_pair elem;
	std::vector<type_pair> used;
	elem.first = oper_a;
	elem.second = oper_b;
	used.push_back(elem);
	elem.first = oper_c;
	elem.second = oper_c;
	used.push_back(elem);
	elem.first = oper_d;
	elem.second = oper_d;
	used.push_back(elem);
	bool addnew;
	do {
		addnew = false;
		int i, j, sz;
		sz = used.size();
		for (i = 1; i < sz && addnew == false; i++)
		for (j = 0; j < i && addnew == false; j++) {
			type_pair tp1 = used[i], tp2 = used[j];
			monom m1, m2;
			m1.c.k = 1;
			m1.opers.first.type = tp1.first;
			m1.opers.first.argid = 0;
			m1.opers.second.type = tp1.second;
			m1.opers.second.argid = 1;
			m2.c.k = 1;
			m2.opers.first.type = tp2.first;
			m2.opers.first.argid = 2;
			m2.opers.second.type = tp2.second;
			m2.opers.second.argid = 3;

			polynom comm = commutate(m1, m2);
			for (auto mon : comm.monoms) {
				type_pair pc;
				pc.first = mon.opers.first.type;
				pc.second = mon.opers.second.type;
				bool find = false;
				for (int k = 0; k < used.size(); k++) {
					if (used[k].first == pc.first && used[k].second == pc.second) {
						find = true;
						break;
					}
				}
				if (!find) {
					used.push_back(pc);
					addnew = true;
				}
			}
		}
	} while (addnew);
	std::cout<<"\\begin{eqnarray}";
	for (unsigned i = 0; i < used.size(); i++) {
		std::cout << used[i];
		if (i != used.size() - 1)
			std::cout << "\\\\";
		std::cout<<"\n";
	}
	std::cout<<"\\end{eqnarray}";
}

int main(void)
{
	print_full = false;
//	print_tex = false;
	if (print_tex) {
		std::cout<<"\\documentclass[12pt]{article}\n";
		std::cout<<"\\usepackage[russian]{babel}\n";
		std::cout<<"\\usepackage[T2A]{fontenc}\n";
		std::cout<<"\\usepackage[utf8]{inputenc}\n";
		std::cout<<"\\usepackage{graphicx}\n";
		std::cout<<"\\usepackage[a4paper]{geometry}\n";
		std::cout<<"\\usepackage{multicol}\n";
		std::cout<<"\\usepackage{amsmath, amssymb}\n";
		std::cout<<"\\begin{document}\n";
		std::cout<<"\\newcommand{\\comm}[2]{\\left[#1, #2\\right]}\n";
		
	}
	try {
		//test1();
		//test2();
		find_used();
	} catch (const char* err) {
		std::cout << "Error: "<<err<<"\n";
	}
	if (print_tex) {
		std::cout<<"\\end{document}\n";
	}
	return 0;
}
