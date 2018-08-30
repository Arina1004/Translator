// scaner.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "StringTable.h"
#include "Atoms.h"
#include "SymbolTable.h"
#include "Translator.h"
#include "Scan1.h"
#include "Token1.h"
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

int main()
{
	//ifstream ifile("progminic.txt");
	//ifstream ifile("Arina.txt");
	ifstream ifile("Sabina.txt");
	ofstream atoms("prog.atoms");
	ofstream table("ForScan.txt");
	ofstream asmcode("prog.asm");
	Translator t(ifile);
	t.translate();
	t.generateCode(asmcode);
	t.SymTablePrint(table);
	t.printAtoms(atoms);
	
}


/*		StringTable t;
		t.add("b");
		shared_ptr<StringOperand> r = t.add("a");
		shared_ptr<StringOperand> r1 = make_shared<StringOperand>(1, &t);
		//cout << r;
		cout << (*r1 == *r);
		return 0;*/


