#include <iostream>
#include "structs.h"


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

