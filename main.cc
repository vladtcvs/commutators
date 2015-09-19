#include "structs.h"

int main(void)
{
//	print_full = false;
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
		//find_used();
		/*type_pair tp;
		tp.first = oper_a;
		tp.second = oper_b;
		ad ad_ab = tp.find_ad();
		for (int j = 0; j < 16; j++)
		for (int i = 0; i < 16; i++) {
			type_pair pi(i), pj(j);
			std::cout << "ad(ab)_{" << pi <<"," << pj << "} = " << ad_ab.elems[i][j] << "\n"; 
		}*/
		killings_form kf = find_killings_form();
		for (int j = 0; j < 16; j++) {
			std::cout << "\\begin{eqnarray}\n";
			for (int i = 0; i < 16; i++) {
				coeff_list cfl = kf.elems[i][j];
				type_pair first(i), sec(j);
				std::cout << "K(" << first << ", " << sec << ") = " <<  cfl << "\\\\\n";
			}
			std::cout << "\\end{eqnarray}\n";
		}
	} catch (const char* err) {
		std::cout << "Error: "<<err<<"\n";
	}
	if (print_tex) {
		std::cout<<"\\end{document}\n";
	}
	return 0;
}
