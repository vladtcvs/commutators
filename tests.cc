#include "structs.h"

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

	for (auto a : oper_i)
	for (auto b : oper_i)
	for (auto c : oper_i)
	for (auto d : oper_i)
	/*oper_type_e a = oper_a;
	oper_type_e b = oper_b;
	oper_type_e c = oper_c;
	oper_type_e d = oper_c;*/
	{
		monom J1(1, a, 0, b, 0);
		monom J2(1, c, 1, d, 1);
		if (print_tex) {
			std::cout<<"\\begin{eqnarray*}\n";
			std::cout << "\\comm{"<<J1<<"}{"<<J2<<"} = \\\\ =";
		} else {
			std::cout << "["<<J1<<", "<<J2<<"] = ";
		}
		
		polynom com = commutate(J1, J2);
		com.convert_to_std();
		std::cout << com;
		std::cout << "\n";
		if (print_tex)
			std::cout<<"\\end{eqnarray*}\n";
		/*polynom com2 = commutate(J2, J1);
		com2 += com;
		if (com2.monoms.size())
			throw "Wrong comm";*/
	}
}

void find_used()
{
	type_pair elem;
	std::vector<type_pair> used;
	elem.first = oper_a;
	elem.second = oper_b;
	elem.argeq = true;
	used.push_back(elem);
	elem.first = oper_c;
	elem.second = oper_c;
	elem.argeq = true;
	used.push_back(elem);
	elem.first = oper_d;
	elem.second = oper_d;
	elem.argeq = true;
	used.push_back(elem);
	bool addnew;

	std::cout<<"\\begin{eqnarray}\n";
	do {
		addnew = false;
		int i, j, sz;
		sz = used.size();
		for (i = 1; i < sz && addnew == false; i++)
		for (j = 0; j < i && addnew == false; j++) {
			type_pair tp1 = used[i], tp2 = used[j];
			monom m1, m2;
			int argid = 0;
			m1.c.k = 1;
			m1.opers.first.type = tp1.first;
			m1.opers.first.argid = argid;
			if (!tp1.argeq)
				argid++;
			m1.opers.second.type = tp1.second;
			m1.opers.second.argid = argid;

			argid++;

			m2.c.k = 1;
			m2.opers.first.type = tp2.first;
			m2.opers.first.argid = argid;
			if (!tp2.argeq)
				argid++;
			m2.opers.second.type = tp2.second;
			m2.opers.second.argid = argid;

			polynom comm = commutate(m1, m2);
			for (auto mon : comm.monoms) {
				type_pair pc;
				pc.first = mon.opers.first.type;
				pc.second = mon.opers.second.type;
				pc.argeq = (mon.opers.first.argid == mon.opers.second.argid);
				bool find = false;
				int pos;
				for (unsigned k = 0; k < used.size(); k++) {
					if (used[k].first == pc.first && used[k].second == pc.second) {
						find = true;
						pos = k;
						break;
					}
				}
				if (find && used[pos].argeq == true && pc.argeq == false) {
					std::cout << "Set: " << pc << " - " << (pc.argeq ? "eq" : "ne") << "\n";
					used[pos].argeq = false;
				}
				if (!find) {
					std::cout << "Add: " << pc << " - " << (pc.argeq ? "eq" : "ne") << "\n";
					used.push_back(pc);
					addnew = true;
				}
			}
		}
	} while (addnew);
	/*
	for (unsigned i = 0; i < used.size(); i++) {
		std::cout << used[i] << " - " << (used[i].argeq ? "eq" : "not");
		if (i != used.size() - 1)
			std::cout << "\\\\";
		std::cout<<"\n";
	}*/
	std::cout<<"\\end{eqnarray}\n";
}
