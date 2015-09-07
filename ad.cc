#include "structs.h"

ad type_pair::find_ad()
{
	ad adop;
	monom src(1, first, 0, second, 1);
	std::vector<oper_type_e> oper_i;
	for (auto a : oper_i)
	for (auto b : oper_i) {
		monom item_pair(1, a, 2, b, 3);
		polynom com = commutate(src, item_pair);
	}
	
	return adop;
}

void monom::convert_to_std()
{
	if (c.dpdq.int_dp || c.dpdq.int_dq) {
		std::cout << "dp or dq already present\n";
		return;
	}
	delta d1, d2;
	d1.type = comm_k1;
	d2.type = comm_k1;
	d1.arg1 = opers.first.argid;
	d1.arg2 = -2;
	d2.arg1 = opers.second.argid;
	d2.arg2 = -3;
	opers.first.argid = -2;
	opers.second.argid = -3;
	c.deltas.push_back(d1);
	c.deltas.push_back(d2);
	c.dpdq.int_dp = true;
	c.dpdq.int_dq = true;
}

void polynom::convert_to_std()
{
	std::list<monom>::iterator it = monoms.begin();
	while (it != monoms.end()) {
		(*it).convert_to_std();
		it++;
	}
}
