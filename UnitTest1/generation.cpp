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
	TEST_CLASS(kod)
	{
	public:

		TEST_METHOD(loadvar)
		{
			ostringstream ostream;
			SymbolTable t;
			t.addVar("b");
			MemoryOperand lab(0, &t);
			lab.load(ostream);
			Assert::AreEqual(ostream.str(), (string)"LDA var0\n");

		}
		TEST_METHOD(loadvarelse)
		{
			ostringstream ostream;
			SymbolTable t;
			t.addVar("a", 0, SymbolTable::TableRecord::RecordType::integer, 0);
			MemoryOperand lab(0, &t);
			lab.load(ostream);
			Assert::AreEqual(ostream.str(), (string)"LXI H,-1\nDAD SP\nMOV A,M\n");

		}
		TEST_METHOD(loadnum)
		{
			ostringstream ostream;
			NumberOperand num(0);
			num.load(ostream);
			Assert::AreEqual(ostream.str(), (string)"MVI A,0\n");

		}
		TEST_METHOD(save)
		{
			ostringstream ostream;
			SymbolTable t;
			t.addVar("a", 0, SymbolTable::TableRecord::RecordType::integer, 0);
			MemoryOperand lab(0, &t);
			lab.save(ostream);
			Assert::AreEqual(ostream.str(), (string)"LXI H,-1\nDAD SP\nMOV M,A\n");

		}
		TEST_METHOD(generateStrings)
		{
			ostringstream ostream;
			StringTable t;
			t.add("b");
			t.generateStrings(ostream);
			Assert::AreEqual(ostream.str(), (string)"str0: DB 'b', 0\n");

		}
		TEST_METHOD(generateGlobals)
		{
			ostringstream ostream;
			SymbolTable t;
			t.addVar("a", -1, SymbolTable::TableRecord::RecordType::integer, 5);
			t.generateGlobals(ostream);
			Assert::AreEqual(ostream.str(), (string)"var0: DB 5\n");

		}
			TEST_METHOD(generateBinaryADD)
			{
				ostringstream ostream;
				SymbolTable t;
				t.addVar("2");
				shared_ptr<RValue> right = make_shared<NumberOperand>(1);
				shared_ptr<RValue> left = make_shared<NumberOperand>(1);
				shared_ptr<MemoryOperand> result = make_shared<MemoryOperand>(0, &t);
				BinaryOpAtom ADD("ADD", left, right, result);
				ADD.generate(ostream);
				Assert::AreEqual(ostream.str(), (string)"\t;(ADD,'1','1',0)\nMVI A,1\nMOV B,A\nMVI A,1\nADD B\nSTA var0\n");
			}
		

	};
}