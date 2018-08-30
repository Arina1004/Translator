#include "stdafx.h"
#include "CppUnitTest.h"
#include "../scaner/StringTable.h"
#include "../scaner/SymbolTable.h"
#include"../scaner/Atoms.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(Atoms)
	{
	public:

		TEST_METHOD(BinaryOpatom)
		{
			SymbolTable t;
			t.addVar("2"); 
			shared_ptr<RValue> right= make_shared<NumberOperand>(1);
			shared_ptr<RValue> left = make_shared<NumberOperand>(1);
			shared_ptr<MemoryOperand> result = make_shared<MemoryOperand>(0, &t);
			BinaryOpAtom ADD("ADD", left, right, result);
			Assert::AreEqual(ADD.toString(), (string)"(ADD,'1','1',0)");
		}
		TEST_METHOD(UnaryOpatom)
		{
			SymbolTable t;
			t.addVar("2");
			shared_ptr<RValue> left = make_shared<NumberOperand>(1);
			shared_ptr<MemoryOperand> result = make_shared<MemoryOperand>(0, &t);
			UnaryOpAtom MOV("MOV",left, result);
			Assert::AreEqual(MOV.toString(), (string)"(MOV,'1',,0)");
		}
		TEST_METHOD(ConditionalJumpatom)
		{
			shared_ptr<RValue> right = make_shared<NumberOperand>(1);
			shared_ptr<RValue> left = make_shared<NumberOperand>(1);
			shared_ptr<LabelOperand> label = make_shared<LabelOperand>(10);
			ConditionalJumpAtom EQ("EQ", left, right, label);
			Assert::AreEqual(EQ.toString(), (string)"(EQ,'1','1',L10)");
		}
		TEST_METHOD(Outatom)
		{
			StringTable t;
			t.add("b");
			shared_ptr<Operand> value = make_shared<StringOperand>(0,&t);
			OutAtom out(value);
			Assert::AreEqual(out.toString(), (string)"(OUT,,,S0)");
		}
		TEST_METHOD(Inatom)
		{
			SymbolTable t;
			t.addVar("2");
			shared_ptr<MemoryOperand> result = make_shared<MemoryOperand>(0, &t);
			InAtom in(result);
			Assert::AreEqual(in.toString(), (string)"(IN,,,0)");
		}
		TEST_METHOD(Labelatom)
		{
			shared_ptr<LabelOperand> label = make_shared<LabelOperand>(10);
			LabelAtom lbl(label);
			Assert::AreEqual(lbl.toString(), (string)"(LBL,,,L10)");
		}
		TEST_METHOD(Jumpatom)
		{
			shared_ptr<LabelOperand> label = make_shared<LabelOperand>(10);
			JumpAtom jmp(label);
			Assert::AreEqual(jmp.toString(), (string)"(JMP,,,L10)");
		}
	};
}