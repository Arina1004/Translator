#include "stdafx.h"
#include "CppUnitTest.h"
#include "../scaner/StringTable.h"
#include "../scaner/SymbolTable.h"
#include"../scaner/Atoms.h"
#include "../scaner/Translator.h"
#include <string>
#include <sstream>
#include <fstream>
#include <ostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(Basic)
	{
	public:

		TEST_METHOD(DeclareStmt_int)
		{

			istringstream istream("int func() { }");
			ostringstream ostream;
			Translator t(istream);
			t.StmtList();
			t.printAtoms(ostream);
			Assert::AreEqual(ostream.str(), (string)("  0  (RET,,,'0')\n"));
			SymbolTable sym;
			sym.addFunc("func", SymbolTable::TableRecord::RecordType::integer, 0);
			Assert::IsTrue(t.SymTable(0) == sym[0]);

		}
		TEST_METHOD(DeclareStmt_param_int)
		{

			istringstream istream("int func(int a) { }");
			ostringstream ostream;
			Translator t(istream);
			t.StmtList();
			t.printAtoms(ostream);
			Assert::AreEqual(ostream.str(), (string)("  0  (RET,,,'0')\n"));
			SymbolTable sym;
			sym.addFunc("func", SymbolTable::TableRecord::RecordType::integer, 1);
			sym.addVar("a", 0, SymbolTable::TableRecord::RecordType::integer, 0);
			Assert::IsTrue(t.SymTable(0) == sym[0]);
			Assert::IsTrue(t.SymTable(1) == sym[1]);
		}
		TEST_METHOD(DeclareStmt_char)
		{

			istringstream istream("char func() { }");
			ostringstream ostream;
			Translator t(istream);
			t.StmtList();
			t.printAtoms(ostream);
			Assert::AreEqual(ostream.str(), (string)("  0  (RET,,,'0')\n"));
			SymbolTable sym;
			sym.addFunc("func", SymbolTable::TableRecord::RecordType::chr, 0);
			Assert::IsTrue(t.SymTable(0) == sym[0]);

		}
		TEST_METHOD(DeclareStmt_param_char_int)
		{

			istringstream istream("char func(char a, int b) { }");
			ostringstream ostream;
			Translator t(istream);
			t.StmtList();
			t.printAtoms(ostream);
			Assert::AreEqual(ostream.str(), (string)("  0  (RET,,,'0')\n"));

			//(const std::string & name, const Scope scope, const TableRecord::RecordType type, const int init)
			//(const std::string & name, const TableRecord::RecordType type, const int len)
			SymbolTable sym;
			sym.addFunc("func", SymbolTable::TableRecord::RecordType::chr, 2);
			sym.addVar("a", 0, SymbolTable::TableRecord::RecordType::chr, 0);
			sym.addVar("b", 0, SymbolTable::TableRecord::RecordType::integer, 0);
			Assert::IsTrue(t.SymTable(0) == sym[0]);
			Assert::IsTrue(t.SymTable(1) == sym[1]);
			Assert::IsTrue(t.SymTable(2) == sym[2]);
			//Assert::AreEqual(ostream.str(), (string)"SYMBOL TABLE\n--------------------------------------------------------------------------\ncode      name      kind      type       len      init     scope    offset\n0         func      func      chr          -1     0         -1        -1        \n1         a         var       chr          -1     0         0         -1        \n2         b         var       integer      -1     0         0        -1        \n--------------------------------------------------------------------------\n");
		}

		TEST_METHOD(intaEQ5)
		{

			istringstream istream("int a = 5;");
			ostringstream ostream;
			Translator t(istream);
			t.StmtList();
			t.printAtoms(ostream);
			Assert::AreEqual(ostream.str(), (string)(""));
			SymbolTable sym;
			sym.addVar("a", -1, SymbolTable::TableRecord::RecordType::integer, 5);
			Assert::IsTrue(t.SymTable(0) == sym[0]);

		}
		TEST_METHOD(INTabc)
		{

			istringstream istream("int a, b, c;");
			ostringstream ostream;
			Translator t(istream);
			t.StmtList();
			t.printAtoms(ostream);
			Assert::AreEqual(ostream.str(), (string)(""));
			SymbolTable sym;
			sym.addVar("a", -1, SymbolTable::TableRecord::RecordType::integer, 0);
			sym.addVar("b", -1, SymbolTable::TableRecord::RecordType::integer, 0);
			sym.addVar("c", -1, SymbolTable::TableRecord::RecordType::integer, 0);
			Assert::IsTrue(t.SymTable(0) == sym[0]);
			Assert::IsTrue(t.SymTable(1) == sym[1]);
			Assert::IsTrue(t.SymTable(2) == sym[2]);
		}
		TEST_METHOD(charaEQ5)
		{

			istringstream istream("char a = 5;");
			ostringstream ostream;
			Translator t(istream);
			t.StmtList();
			t.printAtoms(ostream);
			Assert::AreEqual(ostream.str(), (string)(""));
			SymbolTable sym;
			sym.addVar("a", -1, SymbolTable::TableRecord::RecordType::chr, 5);
			Assert::IsTrue(t.SymTable(0) == sym[0]);
		}
		TEST_METHOD(while1)
		{

			istringstream istream("int main() { int i = 0; while ( i > 0 ) { i = i - 1;  } return 0; }");
			ostringstream ostream;
			Translator t(istream);
			t.StmtList();
			t.printAtoms(ostream);
			Assert::AreEqual(ostream.str(), (string)("  0  (LBL,,,L0)\n  0  (MOV,'1',,2)\n  0  (GT,1,'0',L1)\n  0  (MOV,'0',,2)\n  0  (LBL,,,L1)\n  0  (EQ,2,'0',L2)\n  0  (SUB,1,'1',3)\n  0  (MOV,3,,1)\n  0  (JMP,,,L0)\n  0  (LBL,,,L2)\n  0  (RET,,,'0')\n  0  (RET,,,'0')\n"));
			SymbolTable sym;
			sym.addFunc("main", SymbolTable::TableRecord::RecordType::integer, 0);
			sym.addVar("i", 0, SymbolTable::TableRecord::RecordType::integer, 0);
			sym.addVar("TMP0", 0, SymbolTable::TableRecord::RecordType::integer, 0);
			sym.addVar("TMP1", 0, SymbolTable::TableRecord::RecordType::integer, 0);
			Assert::IsTrue(t.SymTable(0) == sym[0]);
			Assert::IsTrue(t.SymTable(1) == sym[1]);
			Assert::IsTrue(t.SymTable(2) == sym[2]);
			Assert::IsTrue(t.SymTable(3) == sym[3]);
		}
		TEST_METHOD(CHARabc)
		{

			istringstream istream("char a, b, c; ");
			ostringstream ostream;
			Translator t(istream);
			t.StmtList();
			t.printAtoms(ostream);
			Assert::AreEqual(ostream.str(), (string)(""));
			SymbolTable sym;
			sym.addVar("a", -1, SymbolTable::TableRecord::RecordType::chr, 0);
			sym.addVar("b", -1, SymbolTable::TableRecord::RecordType::chr, 0);
			sym.addVar("c", -1, SymbolTable::TableRecord::RecordType::chr, 0);
			Assert::IsTrue(t.SymTable(0) == sym[0]);
		}
		TEST_METHOD(CHARabc1)
		{

			istringstream istream("int main() { for (int i = 0; i < 5; ++i) {} }");
			ostringstream ostream;
			Translator t(istream);
			t.StmtList();
			t.printAtoms(ostream);
			Assert::AreEqual(ostream.str(), (string)("  0  (LBL,,,L0)\n  0  (MOV,'1',,2)\n  0  (LT,1,'5',L1)\n  0  (MOV,'0',,2)\n  0  (LBL,,,L1)\n  0  (EQ,2,'0',L4)\n  0  (JMP,,,L3)\n  0  (LBL,,,L2)\n  0  (ADD,1,'1',1)\n  0  (JMP,,,L0)\n  0  (LBL,,,L3)\n  0  (JMP,,,L2)\n  0  (LBL,,,L4)\n  0  (RET,,,'0')\n"));
			SymbolTable sym;
			sym.addFunc("main", SymbolTable::TableRecord::RecordType::integer, 0);
			sym.addVar("i", 0, SymbolTable::TableRecord::RecordType::integer, 0);
			sym.addVar("TMP0", 0, SymbolTable::TableRecord::RecordType::integer, 0);
			Assert::IsTrue(t.SymTable(0) == sym[0]);
			Assert::IsTrue(t.SymTable(1) == sym[1]);
			Assert::IsTrue(t.SymTable(2) == sym[2]);
		}
		TEST_METHOD(example_16_2)
		{

			istringstream istream("int sqRoots(int x, int y, int z){ int result; result = y*y - 4*x*z; if (result < 0){ out \"No real roots\n\"; } else { if (result == 0) out \"One root\n\"; else out \"Two roots\n\"; } return result; } int main() { int a,b,c,d; in a; in b; in c; sqRoots(a, b, c); return 0; } ");
			ostringstream ostream;
			Translator t(istream);
			t.StmtList();
			t.printAtoms(ostream);
			Assert::AreEqual(ostream.str(), (string)("  0  (MUL,2,2,5)\n  0  (MUL,'4',1,7)\n  0  (MUL,7,3,8)\n  0  (SUB,5,8,6)\n  0  (MOV,6,,4)\n  0  (MOV,'1',,9)\n  0  (LT,4,'0',L2)\n  0  (MOV,'0',,9)\n  0  (LBL,,,L2)\n  0  (EQ,9,'0',L0)\n  0  (OUT,,,S0)\n  0  (JMP,,,L1)\n  0  (LBL,,,L0)\n  0  (MOV,'1',,10)\n  0  (EQ,4,'0',L5)\n  0  (MOV,'0',,10)\n  0  (LBL,,,L5)\n  0  (EQ,10,'0',L3)\n  0  (OUT,,,S1)\n  0  (JMP,,,L4)\n  0  (LBL,,,L3)\n  0  (OUT,,,S2)\n  0  (LBL,,,L4)\n  0  (LBL,,,L1)\n  0  (RET,,,4)\n  0  (RET,,,'0')\n 11  (IN,,,12)\n 11  (IN,,,13)\n 11  (IN,,,14)\n 11  (PARAM,,,14)\n 11  (PARAM,,,13)\n 11  (PARAM,,,12)\n 11  (CALL,0,,16)\n 11  (RET,,,'0')\n 11  (RET,,,'0')\n"));
			SymbolTable sym;
			sym.addFunc("sqRoots", SymbolTable::TableRecord::RecordType::integer, 3);
			sym.addVar("x", 0, SymbolTable::TableRecord::RecordType::integer, 0);
			sym.addVar("y", 0, SymbolTable::TableRecord::RecordType::integer, 0);
			sym.addVar("z", 0, SymbolTable::TableRecord::RecordType::integer, 0);
			sym.addVar("result", 0, SymbolTable::TableRecord::RecordType::integer, 0);
			sym.addVar("TMP0", 0, SymbolTable::TableRecord::RecordType::integer, 0);
			sym.addVar("TMP1", 0, SymbolTable::TableRecord::RecordType::integer, 0);
			sym.addVar("TMP2", 0, SymbolTable::TableRecord::RecordType::integer, 0);
			sym.addVar("TMP3", 0, SymbolTable::TableRecord::RecordType::integer, 0);
			sym.addVar("TMP4", 0, SymbolTable::TableRecord::RecordType::integer, 0);
			sym.addVar("TMP5", 0, SymbolTable::TableRecord::RecordType::integer, 0);
			sym.addFunc("main", SymbolTable::TableRecord::RecordType::integer, 0);
			sym.addVar("a", 11, SymbolTable::TableRecord::RecordType::integer, 0);
			sym.addVar("b", 11, SymbolTable::TableRecord::RecordType::integer, 0);
			sym.addVar("c", 11, SymbolTable::TableRecord::RecordType::integer, 0);
			sym.addVar("d", 11, SymbolTable::TableRecord::RecordType::integer, 0);
			sym.addVar("TMP6", 11, SymbolTable::TableRecord::RecordType::integer, 0);

			Assert::IsTrue(t.SymTable(0) == sym[0]);
			Assert::IsTrue(t.SymTable(1) == sym[1]);
			Assert::IsTrue(t.SymTable(2) == sym[2]);
			Assert::IsTrue(t.SymTable(3) == sym[3]);
			Assert::IsTrue(t.SymTable(4) == sym[4]);
			Assert::IsTrue(t.SymTable(5) == sym[5]);
			Assert::IsTrue(t.SymTable(6) == sym[6]);
			Assert::IsTrue(t.SymTable(7) == sym[7]);
			Assert::IsTrue(t.SymTable(8) == sym[8]);
			Assert::IsTrue(t.SymTable(9) == sym[9]);
			Assert::IsTrue(t.SymTable(10) == sym[10]);
			Assert::IsTrue(t.SymTable(11) == sym[11]);
			Assert::IsTrue(t.SymTable(12) == sym[12]);
			Assert::IsTrue(t.SymTable(13) == sym[13]);
			Assert::IsTrue(t.SymTable(14) == sym[14]);
			Assert::IsTrue(t.SymTable(15) == sym[15]);
			Assert::IsTrue(t.SymTable(16) == sym[16]);
		}
	};
}