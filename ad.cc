#include "structs.h"

ad type_pair::find_ad()
{
	ad adop;
	std::vector<oper_type_e> oper_i = {oper_a, oper_b, oper_c, oper_d};
	type_pair Ji_type = *this;
	arg_pair x, index1_arg, index2_arg;

	Ji_type.argeq = false;

	x.aid1 = 0;
	x.aid2 = 1;
	
	adop.type = Ji_type;
	adop.arg = x;

	index1_arg.aid1 = 2;
	index1_arg.aid2 = 3;
	index2_arg.aid1 = 4;
	index2_arg.aid2 = 5;

	adop.index_arg_1 = index1_arg;
	adop.index_arg_2 = index2_arg;

	for (int i = 0; i < 16; i++)
	for (int j = 0; j < 16; j++)
		adop.elems[i][j].k = 0;

	monom Ji(1, adop.type.first, adop.arg.aid1, adop.type.second, adop.arg.aid2);
	for (int j = 0; j < 16; j++) {
		type_pair Jj_type;
		Jj_type.first = idop(j/4);
		Jj_type.second = idop(j%4);
		Jj_type.argeq = false;

		arg_pair y;
		y = adop.index_arg_2;
		
		monom Jj(1, Jj_type.first, y.aid1, Jj_type.second, y.aid2);
		polynom com = commutate(Ji, Jj);

		//std::cout<<com<<"\n";
		com.convert_to_std();
		for (auto mon : com.monoms) {
			type_pair com_type;
			com_type.first = mon.opers.first.type;
			com_type.second = mon.opers.second.type;
			com_type.argeq = false;

			arg_pair z;
			z.aid1 = mon.opers.first.argid;
			z.aid2 = mon.opers.second.argid;

			coeff f = mon.c;
			f.substitute(z, index1_arg);
			
			/* So, we have term f * J_{com_type}(arg1, arg2) */
			int k = com_type.id(); 
			adop.elems[k][j] = f;
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
/*
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
}*/
