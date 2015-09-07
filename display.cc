#include "structs.h"

bool print_full = true;
bool print_tex = true;

std::basic_ostream<char>& operator < (std::basic_ostream<char>& ss, int argid)
{
	if (argid >= 0) {
		ss << "f";
		for (int i = 0; i < argid; i++)
			ss << "\'";
	} else if (argid == -2) {
		ss << "p";
	} else if (argid == -3) {
		ss << "q";
	} else if (argid == -4) {
		ss << "m";
	} else if (argid == -5) {
		ss << "n";
	}
	return ss;
}

std::basic_ostream<char>& operator << (std::basic_ostream<char>& ss, delta& d)
{
	if (print_full) {
		std::string delta = print_tex ? "\\delta" : "δ";
		std::string eps = print_tex ? "\\epsilon" : "ɛ";
		if (d.type == comm_k2 || d.type == comm_k4 || d.type == comm_k6)
			ss << delta << "'(" < d.arg1;
		else
			ss << delta << "(" < d.arg1;
		if (d.type == comm_k3 || d.type == comm_k4)
			ss << "+" << eps;
		ss << "-" < d.arg2;
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
			ss << "_{";
			if (d.arg1 >= 0)
				ss << d.arg1;
			else if (d.arg1 == -2)
				ss << "p";
			else if (d.arg1 == -3)
				ss << "q";
			ss << ",";
			if (d.arg2 >= 0)
				ss << d.arg2;
			else if (d.arg2 == -2)
				ss << "p";
			else if (d.arg2 == -3)
				ss << "q";
			ss <<"}";
		} else {
			ss << " ";
		}
	}
	return ss;
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
		ss << "(" < op.argid;
		ss << ")";
	} else if (print_tex) {
		ss << "_{";
		if (op.argid >= 0)
			ss << op.argid;
		else if (op.argid == -2)
			ss << "p";
		else if (op.argid == -3)
			ss << "q";
		ss << "}";
	}
	return ss;
}

std::basic_ostream<char>& operator << (std::basic_ostream<char>& ss, coeff& cf)
{
	std::vector<delta>::iterator it;
	if (cf.k == 0) {
		ss << "0";
		return ss;
	}
	if (cf.dpdq.int_dp || cf.dpdq.int_dq) {
		if (print_tex) {
			ss << "\\int ";
		} else {
			ss << "int ";
		}
		if (cf.dpdq.int_dp)
			ss << "dp ";
		if (cf.dpdq.int_dq)
			ss << "dq ";
		if (print_tex) {
			ss << "\\cdot ";
		} else {
			ss << " * ";
		}
		
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

std::basic_ostream<char>& operator << (std::basic_ostream<char>& ss, pair& p)
{
	ss << p.first << (print_tex ? " " : "*") << p.second;
	return ss;
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

