#include "structs.h"

ad type_pair::find_ad()
{
	ad adop;
	monom src(1, first, 0, second, 1);
	std::vector<oper_type_e> oper_i;

	adop.used_pair.first.argid = 0;
	adop.used_pair.first.type = first;
	adop.used_pair.second.argid = 1;
	adop.used_pair.second.type = second;

	for (int i = 0; i < 16; i++)
	for (int j = 0; j < 16; j++) {
		adop.elems[i][j].K.k = 0;
		adop.elems[i][j].K.dpdq.int_dp = 0;
		adop.elems[i][j].K.dpdq.int_dq = 0;
	}

	for (auto a : oper_i)
	for (auto b : oper_i) {
		monom item_pair(1, a, 2, b, 3);
		type_pair col;
		col.first = a;
		col.second = b;
		int col_id = col.id();
		polynom com = commutate(src, item_pair);
		com.convert_to_std();
		for (auto mon : com.monoms) {
			type_pair row;
			row.first = mon.opers.first.type;
			row.second = mon.opers.second.type;
			int row_id = row.id();
			adop.elems[row_id][col_id].K = mon.c;
			adop.elems[row_id][col_id].row.aid1 = mon.opers.first.argid;
			adop.elems[row_id][col_id].row.aid2 = mon.opers.second.argid;
			adop.elems[row_id][col_id].col.aid1 = 2;
			adop.elems[row_id][col_id].col.aid2 = 3;
		}
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
