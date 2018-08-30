#include "stdafx.h"
#include "CppUnitTest.h"
#include "../scaner/StringTable.h"
#include "../scaner/SymbolTable.h"
#include"../scaner/Atoms.h"
#include "../scaner/Translator.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(StrTABLE)
	{
	public:

		TEST_METHOD(str1)
		{
			ostringstream ostream;
			StringTable t;
			t.add("brr");
			ostream << t;
			Assert::AreEqual(ostream.str(), (string)"STRING TABLE\n----------------------------------------------\n0 brr\n----------------------------------------------\n");
		}
		TEST_METHOD(strEmpty)
		{
			ostringstream ostream;
			StringTable t;
			ostream << t;
			Assert::AreEqual(ostream.str(), (string)"STRING TABLE\n----------------------------------------------\n----------------------------------------------\n");
		}
		TEST_METHOD(Test3)
		{
			ostringstream ostream;
			StringTable t;
			t.add("b");
			t.add("a");
			Assert::AreEqual(t[0], (string)"b");
		}
		TEST_METHOD(Test4)
		{
			ostringstream ostream;
			StringTable t;
			t.add("b");
			t.add("a");
			Assert::AreEqual(t[1], (string)"a");
		}
		TEST_METHOD(index)
		{
			ostringstream ostream;
			StringTable t;
			t.add("b");
			shared_ptr<StringOperand> r = t.add("a");
			shared_ptr<StringOperand> r1 = make_shared<StringOperand>(1, &t);
			Assert::IsTrue(*r == *r1);
		}
		TEST_METHOD(indexDouble)
		{
			ostringstream ostream;
			StringTable t;
			t.add("a");
			shared_ptr<StringOperand> r = t.add("a");
			shared_ptr<StringOperand> r1 = make_shared<StringOperand>(0, &t);
			Assert::AreEqual(r->toString(), r1->toString());
		}
	};
	TEST_CLASS(SymTABLE)
	{
	public:

		TEST_METHOD(str1)
		{
			ostringstream ostream;
			SymbolTable t;
			t.addVar("b");
			ostream << t;
			Assert::AreEqual(ostream.str(), (string)"SYMBOL TABLE\n--------------------------------------------------------------------------\ncode      name      kind      type       len      init     scope    offset\n0         b         var       unknown      -1     0         -1        -1        \n--------------------------------------------------------------------------\n");
		}
		TEST_METHOD(strEmpty)
		{
			ostringstream ostream;
			SymbolTable t;
			ostream << t;
			Assert::AreEqual(ostream.str(), (string)"SYMBOL TABLE\n--------------------------------------------------------------------------\ncode      name      kind      type       len      init     scope    offset\n--------------------------------------------------------------------------\n");
		}
		TEST_METHOD(Test3)
		{
			ostringstream ostream;
			SymbolTable t;
			t.addVar("b");
			t.addVar("a");
			Assert::AreEqual(t[0]._name, (string)"b");
		}
		TEST_METHOD(Test4)
		{
			ostringstream ostream;
			SymbolTable t;
			t.addVar("b");
			t.addVar("a");
			Assert::AreEqual(t[1]._name, (string)"a");
		}
		TEST_METHOD(index)
		{
			ostringstream ostream;
			SymbolTable t;
			t.addVar("b");
			shared_ptr<MemoryOperand> r = t.addVar("a");
			shared_ptr<MemoryOperand> r1 = make_shared<MemoryOperand>(1, &t);
			Assert::IsTrue(*r == *r1);
		}
		TEST_METHOD(SymTableSCOPE)
		{
			istringstream istream("int b");
			ostringstream ostream;
			Translator t(istream);
			t.StmtList(5);
			t.SymTablePrint(ostream);
			Assert::AreEqual(ostream.str(), (string)"SYMBOL TABLE\n--------------------------------------------------------------------------\ncode      name      kind      type       len      init     scope    offset\n0         b         var       integer      -1     0         5         -1        \n--------------------------------------------------------------------------\n");
		}
	};
	TEST_CLASS(operand)
	{
	public:

		TEST_METHOD(Numoperand)
		{
			NumberOperand num(123);
			Assert::AreEqual(num.toString(), (string)"'123'");
		}

		TEST_METHOD(LabOperand)
		{
			LabelOperand lab(123);
			Assert::AreEqual(lab.toString(), (string)"L123");
		}
		TEST_METHOD(MemOperand)
		{
			SymbolTable t;
			t.addVar("b");
			t.addVar("a");
			MemoryOperand lab(1, &t);
			Assert::AreEqual(lab.toString(), (string)"1");
		}

		TEST_METHOD(StrOperan)
		{
			StringTable t;
			t.add("b");
			t.add("a");
			StringOperand lab(1, &t);
			Assert::AreEqual(lab.toString(), (string)"S1");
		}

	};
}