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
	TEST_CLASS(TransR)
	{
	public:

		
		TEST_METHOD(NOT)
		{
			istringstream istream("int main () {int p; if ( ! p ) {}  }");
			ostringstream ostream;
			Translator t(istream);
			t.StmtList();
			t.printAtoms(ostream);
			Assert::AreEqual(ostream.str(), (string)("  0  (NOT,1,,2)\n  0  (EQ,2,'0',L0)\n  0  (JMP,,,L1)\n  0  (LBL,,,L0)\n  0  (LBL,,,L1)\n  0  (RET,,,'0')\n"));
		}
		TEST_METHOD(MultOp)
		{
			istringstream istream("int main () {int a, b ; b = a*a; }");
			ostringstream ostream;
			Translator t(istream);
			t.StmtList();
			t.printAtoms(ostream);
			Assert::AreEqual(ostream.str(), (string)("  0  (MUL,1,1,3)\n  0  (MOV,3,,2)\n  0  (RET,,,'0')\n"));
		}
		TEST_METHOD(MultInt)
		{
			istringstream istream("int main () {int b ; b = 1 * 2; }");
			ostringstream ostream;
			Translator t(istream);
			t.StmtList();
			t.printAtoms(ostream);
			Assert::AreEqual(ostream.str(), (string)("  0  (MUL,'1','2',2)\n  0  (MOV,2,,1)\n  0  (RET,,,'0')\n"));
		}
		TEST_METHOD(MultChar)
		{
			istringstream istream("int main () {int b ; b = '1' * '2'; }");
			ostringstream ostream;
			Translator t(istream);
			t.StmtList();
			t.printAtoms(ostream);
			Assert::AreEqual(ostream.str(), (string)("  0  (MUL,'49','50',2)\n  0  (MOV,2,,1)\n  0  (RET,,,'0')\n"));
		}
		TEST_METHOD(MultCharInt)
		{
			istringstream istream("int main () {int a,b ; b = '1' * a; }");
			ostringstream ostream;
			Translator t(istream);
			t.StmtList();
			t.printAtoms(ostream);
			Assert::AreEqual(ostream.str(), (string)("  0  (MUL,'49',1,3)\n  0  (MOV,3,,2)\n  0  (RET,,,'0')\n"));
		}
		TEST_METHOD(SubOp)
		{
			istringstream istream("int main () { int a, b ; b = a - a ;}");
			ostringstream ostream;
			Translator t(istream);
			t.StmtList();
			t.printAtoms(ostream);
			Assert::AreEqual(ostream.str(), (string)("  0  (SUB,1,1,3)\n  0  (MOV,3,,2)\n  0  (RET,,,'0')\n"));
		}
		TEST_METHOD(SubInt)
		{
			istringstream istream("int main () { int  b ; b = 1 - 2 ;}");
			ostringstream ostream;
			Translator t(istream);
			t.StmtList();
			t.printAtoms(ostream);
			Assert::AreEqual(ostream.str(), (string)("  0  (SUB,'1','2',2)\n  0  (MOV,2,,1)\n  0  (RET,,,'0')\n"));
		}
		TEST_METHOD(SubChar)
		{
			istringstream istream("int main () { int  b ; b = '1' - '2' ;}");
			ostringstream ostream;
			Translator t(istream);
			t.StmtList();
			t.printAtoms(ostream);
			Assert::AreEqual(ostream.str(), (string)("  0  (SUB,'49','50',2)\n  0  (MOV,2,,1)\n  0  (RET,,,'0')\n"));
		}
		TEST_METHOD(SubCharInt)
		{
			istringstream istream("int main () { int a, b ; b = a - '2' ;}");
			ostringstream ostream;
			Translator t(istream);
			t.StmtList();
			t.printAtoms(ostream);
			Assert::AreEqual(ostream.str(), (string)("  0  (SUB,1,'50',3)\n  0  (MOV,3,,2)\n  0  (RET,,,'0')\n"));
		}
		TEST_METHOD(AddOp)
		{
			istringstream istream("int main ( ) { int a ,b ; b = a + b ;}");
			ostringstream ostream;
			Translator t(istream);
			t.StmtList();
			t.printAtoms(ostream);
			Assert::AreEqual(ostream.str(), (string)("  0  (ADD,1,2,3)\n  0  (MOV,3,,2)\n  0  (RET,,,'0')\n"));
		}
		TEST_METHOD(AddInt)
		{
			istringstream istream("int main ( ) { int b ; b = 1 + 2 ;}");
			ostringstream ostream;
			Translator t(istream);
			t.StmtList();
			t.printAtoms(ostream);
			Assert::AreEqual(ostream.str(), (string)("  0  (ADD,'1','2',2)\n  0  (MOV,2,,1)\n  0  (RET,,,'0')\n"));
		}
		TEST_METHOD(AddChar)
		{
			istringstream istream("int main ( ) { int b ; b = '2' + '3' ;}");
			ostringstream ostream;
			Translator t(istream);
			t.StmtList();
			t.printAtoms(ostream);
			Assert::AreEqual(ostream.str(), (string)("  0  (ADD,'50','51',2)\n  0  (MOV,2,,1)\n  0  (RET,,,'0')\n"));
		}
		TEST_METHOD(AddCharInt)
		{
			istringstream istream("int main ( ) { int a ,b ; b = a + '8' ;}");
			ostringstream ostream;
			Translator t(istream);
			t.StmtList();
			t.printAtoms(ostream);
			Assert::AreEqual(ostream.str(), (string)("  0  (ADD,1,'56',3)\n  0  (MOV,3,,2)\n  0  (RET,,,'0')\n"));
		}

		/*TEST_METHOD(LeOp)
		{
			istringstream istream("int main ( ) { int a ,b ; if ( a <= b ) { };}");
			ostringstream ostream;
			Translator t(istream);
			t.StmtList();
			t.printAtoms(ostream);
			Assert::AreEqual(ostream.str(), (string)(" -1  (MOV,'1',,1)\n -1  (LE,0,0,L0)\n -1  (MOV,'0',,1)\n -1  (LBL,,,L0)\n"));
		}
		TEST_METHOD(LeInt)
		{
			istringstream istream("int main ( ) {  if ( 1 <= 2) { } ; }");
			ostringstream ostream;
			Translator t(istream);
			t.StmtList();
			t.printAtoms(ostream);
			Assert::AreEqual(ostream.str(), (string)(" -1  (MOV,'1',,0)\n -1  (LE,'1','2',L0)\n -1  (MOV,'0',,0)\n -1  (LBL,,,L0)\n"));
		}
		TEST_METHOD(LeChar)
		{
			istringstream istream("int main ( ) {  if ( '8' <= '9') { };}");
			ostringstream ostream;
			Translator t(istream);
			t.StmtList();
			t.printAtoms(ostream);
			Assert::AreEqual(ostream.str(), (string)(" -1  (MOV,'1',,0)\n -1  (LE,'50','51',L0)\n -1  (MOV,'0',,0)\n -1  (LBL,,,L0)\n"));
		}
		TEST_METHOD(LeCharInt)
		{
			istringstream istream("int main ( ) { int a ; if ( a <= '1') { };}");
			ostringstream ostream;
			Translator t(istream);
			t.StmtList();
			t.printAtoms(ostream);
			Assert::AreEqual(ostream.str(), (string)(" -1  (MOV,'1',,0)\n -1  (LE,'97','3',L0)\n -1  (MOV,'0',,0)\n -1  (LBL,,,L0)\n"));
		}
		/*TEST_METHOD(LtOp)
		{
			istringstream istream("int main ( ) { int a ; if ( a <= 150) { };}");
			ostringstream ostream;
			Translator t(istream);
			t.StmtList();

			t.printAtoms(ostream);
			Assert::AreEqual(ostream.str(), (string)(" -1  (MOV,'1',,1)\n -1  (LT,0,'150',L0)\n -1  (MOV,'0',,1)\n -1  (LBL,,,L0)\n"));
		}
		TEST_METHOD(GtOp)
		{
			istringstream istream("int main ( ) { int a ; if ( a > '1') { };}");
			ostringstream ostream;
			Translator t(istream);
			t.StmtList();
			t.printAtoms(ostream);
			Assert::AreEqual(ostream.str(), (string)(" -1  (MOV,'1',,1)\n -1  (GT,0,'150',L0)\n -1  (MOV,'0',,1)\n -1  (LBL,,,L0)\n"));
		}
	   TEST_METHOD(NeOp)
		{
			istringstream istream("int main ( ) { int a ; if ( a != '1') { };}");
			ostringstream ostream;
			Translator t(istream);
			t.StmtList();
			t.printAtoms(ostream);
			Assert::AreEqual(ostream.str(), (string)(" -1  (MOV,'1',,1)\n -1  (NE,0,'150',L0)\n -1  (MOV,'0',,1)\n -1  (LBL,,,L0)\n"));
		}
		TEST_METHOD(EqOp)
		{
			istringstream istream("int main ( ) { int a ; if ( a == '1') { };}");
			ostringstream ostream;
			Translator t(istream);
			t.StmtList();
			t.printAtoms(ostream);
			Assert::AreEqual(ostream.str(), (string)(" -1  (MOV,'1',,1)\n -1  (EQ,0,'150',L0)\n -1  (MOV,'0',,1)\n -1  (LBL,,,L0)\n"));
		}
		TEST_METHOD(AndOp)
		{
			istringstream istream("int main ( ) { int a , b ; if ( a && b) { };}");
			ostringstream ostream;
			Translator t(istream);
			t.StmtList();
			t.printAtoms(ostream);
			Assert::AreEqual(ostream.str(), (string)(" -1  (AND,0,1,2)\n"));
		}
		TEST_METHOD(AndInt)
		{
			istringstream istream("int main ( ) {  if ( 1 && 2) { };}");
			ostringstream ostream;
			Translator t(istream);
			t.StmtList();
			t.printAtoms(ostream);
			Assert::AreEqual(ostream.str(), (string)(" -1  (AND,'1','2',0)\n"));
		}
		TEST_METHOD(AndChar)
		{
			istringstream istream("int main ( ) {  if ( '9' && '9') { };}");
			ostringstream ostream;
			Translator t(istream);
			t.StmtList();
			t.printAtoms(ostream);
			Assert::AreEqual(ostream.str(), (string)(" -1  (AND,'50','51',0)\n"));
		}
		TEST_METHOD(AndCharInt)
		{
			istringstream istream("int main ( ) { int a ; if ( a && '9') { };}");
			ostringstream ostream;
			Translator t(istream);
			t.StmtList();
			t.printAtoms(ostream);
			Assert::AreEqual(ostream.str(), (string)(" -1  (AND,'97','3',0)\n"));
		}
		TEST_METHOD(OrOp)
		{
			istringstream istream("int main ( ) { int a , b ; if ( a || b) { };}");
			ostringstream ostream;
			Translator t(istream);
			t.StmtList();
			t.printAtoms(ostream);
			Assert::AreEqual(ostream.str(), (string)(" -1  (OR,0,1,2)\n"));
		}
		TEST_METHOD(OrInt)
		{
			istringstream istream("int main ( ) {  if ( 1 || 2) { };}");
			ostringstream ostream;
			Translator t(istream);
			t.StmtList();
			t.printAtoms(ostream);
			Assert::AreEqual(ostream.str(), (string)(" -1  (OR,'1','2',0)\n"));
		}
		TEST_METHOD(OrChar)
		{
			istringstream istream("int main ( ) {  if ( 'a' || 'b') { };}");
			ostringstream ostream;
			Translator t(istream);
			t.StmtList();
			t.printAtoms(ostream);
			Assert::AreEqual(ostream.str(), (string)(" -1  (OR,'50','51',0)\n"));
		}
		TEST_METHOD(OrCharInt)
		{
			istringstream istream("int main ( ) { if ( 'a' || 3) { };}");
			ostringstream ostream;
			Translator t(istream);
			t.StmtList();
			t.printAtoms(ostream);
			Assert::AreEqual(ostream.str(), (string)(" -1  (OR,'97','3',0)\n"));
		}*/

	};
}