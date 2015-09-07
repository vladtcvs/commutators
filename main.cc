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
		test2();
		//find_used();
	} catch (const char* err) {
		std::cout << "Error: "<<err<<"\n";
	}
	if (print_tex) {
		std::cout<<"\\end{document}\n";
	}
	return 0;
}
