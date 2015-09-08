#ifndef __STRUCTS_H__
#define __STRUCTS_H__

#include <iostream>
#include <sstream>
#include <vector>
#include <list>

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

struct delta {
	comm_type_e type;
	int arg1, arg2;
	int operator % (delta d2);
	int convert();
	int zero();
};

struct extra_vars {
	std::vector<bool> dvar;
	std::vector<bool> used;
	int new_extra();
};

struct coeff {
	extra_vars pqr;
	double k;
	std::vector<delta> deltas;
	int operator % (coeff c2);
	coeff operator *= (coeff& c2);
	void convert();
	void change_argid(int old_aid, int new_aid);
};

struct coeff_list {
	std::list<coeff> cf;
	void rmdouble();
};

struct oper {
	oper_type_e type;
	int argid;
	oper();
	oper(oper_type_e ntype, int nargid);
	bool operator == (oper op2);
	bool operator != (oper op2);
};

struct pair {
	oper first, second;
	int operator % (pair p2);
	pair();
	pair(oper_type_e nt1, int na1, oper_type_e nt2, int na2);
};

struct arg_pair {
	int aid1, aid2;
};

struct ad;

struct type_pair {
	oper_type_e first, second;
	bool argeq;
	int id();
	void set_id(int nid);
	ad find_ad();
	type_pair();
	type_pair(int id);
};

struct ad {
	type_pair types;
	arg_pair args1, args2;
	coeff elems[16][16];
};

struct killings_form {
	coeff elems[16][16];
	type_pair tp;
	arg_pair arg;
};

struct monom {
	pair opers;
	coeff  c;
	monom();
	monom(double nc, oper_type_e nt1, int na1, oper_type_e nt2, int na2);
	int operator %(monom &m);
	void convert_to_std();
};

struct polynom {
	std::list<monom> monoms;
	void rmdouble();
	polynom operator += (polynom p2);
	polynom operator = (monom mon);
	void convert_to_std();
};

extern bool print_full;
extern bool print_tex;

void test1();
void test2();
void find_used();

coeff commutate(oper op1, oper op2);
polynom commutate(monom J1, monom J2);
polynom commutate(polynom p1, polynom p2);

oper_type_e idop(int id);
int opid(oper_type_e op);

std::basic_ostream<char>& operator < (std::basic_ostream<char>& ss, int argid);
std::basic_ostream<char>& operator << (std::basic_ostream<char>& ss, delta& d);
std::basic_ostream<char>& operator << (std::basic_ostream<char>& ss, oper& op);
std::basic_ostream<char>& operator << (std::basic_ostream<char>& ss, coeff& cf);
std::basic_ostream<char>& operator << (std::basic_ostream<char>& ss, type_pair& tp);
std::basic_ostream<char>& operator << (std::basic_ostream<char>& ss, pair& p);
std::basic_ostream<char>& operator << (std::basic_ostream<char>& ss, monom& mon);
std::basic_ostream<char>& operator << (std::basic_ostream<char>& ss, polynom& pol);
std::basic_ostream<char>& operator << (std::basic_ostream<char>& ss, oper_type_e op);

#endif
