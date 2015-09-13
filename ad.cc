#include "structs.h"

ad type_pair::find_ad()
{
	ad adop;
	std::vector<oper_type_e> oper_i = {oper_a, oper_b, oper_c, oper_d};

	adop.types.first = first;
	adop.types.second = second;
	adop.args1.aid1 = 0;
	adop.args1.aid2 = 1;
	adop.args2.aid1 = 2;
	adop.args2.aid2 = 3;

	for (int i = 0; i < 16; i++)
	for (int j = 0; j < 16; j++)
		adop.elems[i][j].k = 0;

	monom src(1, adop.types.first, adop.args1.aid1, adop.types.second, adop.args1.aid2);
	for (auto a : oper_i)
	for (auto b : oper_i) {
		monom item_pair(1, a, adop.args2.aid1, b, adop.args2.aid2);
		type_pair col;
		col.first = a;
		col.second = b;
		int col_id = col.id();
		//std::cout<<"["<<src<<", "<<item_pair<<"] = ";
		polynom com = commutate(src, item_pair);
		//std::cout<<com<<"\n";
		com.convert_to_std();
		for (auto mon : com.monoms) {
			type_pair row;
			row.first = mon.opers.first.type;
			row.second = mon.opers.second.type;
			int row_id = row.id();
			mon.c.change_argid(mon.opers.first.argid, -2);
			mon.c.change_argid(mon.opers.second.argid, -3);
			adop.elems[row_id][col_id] = mon.c;
		}
	}

	return adop;
}

void monom::convert_to_std()
{
	delta d1, d2;
	if (opers.first.argid >= 0) {
		d1.type = comm_k1;
		d1.arg1 = opers.first.argid;
		d1.arg2 = c.pqr.new_extra();
		c.pqr.dvar[-(d1.arg2 + 2)] = true;
		opers.first.argid = d1.arg2;
		c.deltas.push_back(d1);
	}
	if (opers.second.argid >= 0) {
		d2.type = comm_k1;
		d2.arg1 = opers.second.argid;
		d2.arg2 = c.pqr.new_extra();
		c.pqr.dvar[-(d2.arg2 + 2)] = true;
		opers.second.argid = d2.arg2;
		c.deltas.push_back(d2);
	}
}

void polynom::convert_to_std()
{
	std::list<monom>::iterator it = monoms.begin();
	while (it != monoms.end()) {
		(*it).convert_to_std();
		it++;
	}
}

int new_extra_int(coeff c1, coeff c2)
{
	int id;
	int sz1 = c1.pqr.dvar.size();
	int sz2 = c2.pqr.dvar.size();
	id = sz1 > sz2 ? sz1 : sz2;
	c1.pqr.dvar.resize(id + 1, false);
	c1.pqr.used.resize(id + 1, false);
	c2.pqr.dvar.resize(id + 1, false);
	c2.pqr.used.resize(id + 1, false);
	c1.pqr.dvar[id] = true;
	c1.pqr.used[id] = true;
	c2.pqr.dvar[id] = true;
	c2.pqr.used[id] = true;
	return -id - 2;
}

coeff_list get_trace(type_pair first_type, arg_pair first_arg, type_pair second_type, arg_pair second_arg)
{
	coeff_list res;
	type_pair m, n;
	int p_id, r_id;
	ad ad_first, ad_second;

	ad_first = first_type.find_ad();
	ad_second = second_type.find_ad();
	arg_pair oldp11 = ad_first.args1;
	arg_pair oldp12 = ad_first.args2;
	arg_pair oldp21 = ad_second.args1;
	arg_pair oldp22 = ad_second.args2;

	for (p_id = 0; p_id < 16; p_id++)
	for (r_id = 0; r_id < 16; r_id++) {
		coeff item;
		coeff ad1 = ad_first.elems[p_id][r_id];
		coeff ad2 = ad_second.elems[r_id][p_id];
		
		arg_pair arg_a, arg_g;
		arg_a.aid1 = new_extra_int(ad1, ad2);
		arg_a.aid2 = new_extra_int(ad1, ad2);
		arg_g.aid1 = new_extra_int(ad1, ad2);
		arg_g.aid2 = new_extra_int(ad1, ad2);
		
		ad1.change_argid(oldp11.aid1, arg_a.aid1);
		ad1.change_argid(oldp11.aid2, arg_a.aid2);
		ad1.change_argid(oldp12.aid1, arg_g.aid1);
		ad1.change_argid(oldp12.aid2, arg_g.aid2);

		ad2.change_argid(oldp21.aid1, arg_g.aid1);
		ad2.change_argid(oldp21.aid2, arg_g.aid2);
		ad2.change_argid(oldp22.aid1, arg_a.aid1);
		ad2.change_argid(oldp22.aid2, arg_a.aid2);

		item = ad1;
		item *= ad2;
		res.cf.push_back(item);
	}
	res.rmdouble();
	return res;
}

killings_form find_killings_form()
{
	killings_form kf;
	int f_id, s_id;
	for (f_id = 0; f_id < 16; f_id++)
	for (s_id = 0; s_id < 16; s_id++) {
		int base = (f_id*16 + s_id)*4;
		arg_pair f_arg, s_arg;
		type_pair f_type(f_id), s_type(s_id);
		f_arg.aid1 = base;
		f_arg.aid2 = base + 1;
		s_arg.aid1 = base + 2;
		s_arg.aid2 = base + 3;
		kf.elems[f_id][s_id] = get_trace(f_type, f_arg, s_type, s_arg);
	}
	return kf;
}
