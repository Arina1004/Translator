#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\scaner\Scan1.h"
#include "..\scaner\Token1.h"
#include <string>
#include <sstream>
#include <fstream>
#include <ostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestToken
{
	TEST_CLASS(TokenTest)
	{
	public:

		TEST_METHOD(num)
		{
			ostringstream stream;
			Token d(3);
			d.print(stream);
			Assert::AreEqual(stream.str(), static_cast<string>("[num,3]\n"));
		}
		TEST_METHOD(chr)
		{
			ostringstream stream;
			Token d('c');
			d.print(stream);
			Assert::AreEqual(stream.str(), static_cast<string>("[chr]\n"));
		}
		TEST_METHOD(str)
		{
			ostringstream stream;
			Token d(LexemType::str, "Hello");
			d.print(stream);
			Assert::AreEqual(stream.str(), static_cast<string>("[str,Hello]\n"));
		}
		TEST_METHOD(id)
		{
			ostringstream stream;
			Token d(LexemType::id, "test");
			d.print(stream);
			Assert::AreEqual(stream.str(), static_cast<string>("[id,test]\n"));
		}
		TEST_METHOD(lpar)
		{
			ostringstream stream;
			Token d(LexemType::lpar);
			d.print(stream);
			Assert::AreEqual(stream.str(), static_cast<string>("[lpar]\n"));
		}
		TEST_METHOD(lbracket)
		{
			ostringstream stream;
			Token d(LexemType::lbracket);
			d.print(stream);
			Assert::AreEqual(stream.str(), static_cast<string>("[lbracket]\n"));
		}
		TEST_METHOD(lbrace)
		{
			ostringstream stream;
			Token d(LexemType::lbrace);
			d.print(stream);
			Assert::AreEqual(stream.str(), static_cast<string>("[lbrace]\n"));
		}
		TEST_METHOD(rpar)
		{
			ostringstream stream;
			Token d(LexemType::rpar);
			d.print(stream);
			Assert::AreEqual(stream.str(), static_cast<string>("[rpar]\n"));
		}
		TEST_METHOD(rbracket)
		{
			ostringstream stream;
			Token d(LexemType::rbracket);
			d.print(stream);
			Assert::AreEqual(stream.str(), static_cast<string>("[rbracket]\n"));
		}
		TEST_METHOD(rbrace)
		{
			ostringstream stream;
			Token d(LexemType::rbrace);
			d.print(stream);
			Assert::AreEqual(stream.str(), static_cast<string>("[rbrace]\n"));
		}
		TEST_METHOD(semicolon)
		{
			ostringstream stream;
			Token d(LexemType::semicolon);
			d.print(stream);
			Assert::AreEqual(stream.str(), static_cast<string>("[semicolon]\n"));
		}
		TEST_METHOD(comma)
		{
			ostringstream stream;
			Token d(LexemType::comma);
			d.print(stream);
			Assert::AreEqual(stream.str(), static_cast<string>("[comma]\n"));
		}
		TEST_METHOD(colon)
		{
			ostringstream stream;
			Token d(LexemType::colon);
			d.print(stream);
			Assert::AreEqual(stream.str(), static_cast<string>("[colon]\n"));
		}
		TEST_METHOD(opassign)
		{
			ostringstream stream;
			Token d(LexemType::opassign);
			d.print(stream);
			Assert::AreEqual(stream.str(), static_cast<string>("[opassign]\n"));
		}
		TEST_METHOD(opplus)
		{
			ostringstream stream;
			Token d(LexemType::opplus);
			d.print(stream);
			Assert::AreEqual(stream.str(), static_cast<string>("[opplus]\n"));
		}
		TEST_METHOD(opminus)
		{
			ostringstream stream;
			Token d(LexemType::opminus);
			d.print(stream);
			Assert::AreEqual(stream.str(), static_cast<string>("[opminus]\n"));
		}
		TEST_METHOD(opmult)
		{
			ostringstream stream;
			Token d(LexemType::opmult);
			d.print(stream);
			Assert::AreEqual(stream.str(), static_cast<string>("[opmult]\n"));
		}
		TEST_METHOD(opinc)
		{
			ostringstream stream;
			Token d(LexemType::opinc);
			d.print(stream);
			Assert::AreEqual(stream.str(), static_cast<string>("[opinc]\n"));
		}
		TEST_METHOD(opeq)
		{
			ostringstream stream;
			Token d(LexemType::opeq);
			d.print(stream);
			Assert::AreEqual(stream.str(), static_cast<string>("[opeq]\n"));
		}
		TEST_METHOD(opne)
		{
			ostringstream stream;
			Token d(LexemType::opne);
			d.print(stream);
			Assert::AreEqual(stream.str(), static_cast<string>("[opne]\n"));
		}
		TEST_METHOD(oplt)
		{
			ostringstream stream;
			Token d(LexemType::oplt);
			d.print(stream);
			Assert::AreEqual(stream.str(), static_cast<string>("[oplt]\n"));
		}
		TEST_METHOD(opgt)
		{
			ostringstream stream;
			Token d(LexemType::opgt);
			d.print(stream);
			Assert::AreEqual(stream.str(), static_cast<string>("[opgt]\n"));
		}
		TEST_METHOD(ople)
		{
			ostringstream stream;
			Token d(LexemType::ople);
			d.print(stream);
			Assert::AreEqual(stream.str(), static_cast<string>("[ople]\n"));
		}
		TEST_METHOD(opnot)
		{
			ostringstream stream;
			Token d(LexemType::opnot);
			d.print(stream);
			Assert::AreEqual(stream.str(), static_cast<string>("[opnot]\n"));
		}
		TEST_METHOD(opor)
		{
			ostringstream stream;
			Token d(LexemType::opor);
			d.print(stream);
			Assert::AreEqual(stream.str(), static_cast<string>("[opor]\n"));
		}
		TEST_METHOD(opand)
		{
			ostringstream stream;
			Token d(LexemType::opand);
			d.print(stream);
			Assert::AreEqual(stream.str(), static_cast<string>("[opand]\n"));
		}
		TEST_METHOD(kwint)
		{
			ostringstream stream;
			Token d(LexemType::kwint);
			d.print(stream);
			Assert::AreEqual(stream.str(), static_cast<string>("[kwint]\n"));
		}
		TEST_METHOD(kwchar)
		{
			ostringstream stream;
			Token d(LexemType::kwchar);
			d.print(stream);
			Assert::AreEqual(stream.str(), static_cast<string>("[kwchar]\n"));
		}
		TEST_METHOD(kwif)
		{
			ostringstream stream;
			Token d(LexemType::kwif);
			d.print(stream);
			Assert::AreEqual(stream.str(), static_cast<string>("[kwif]\n"));
		}
		TEST_METHOD(kwelse)
		{
			ostringstream stream;
			Token d(LexemType::kwelse);
			d.print(stream);
			Assert::AreEqual(stream.str(), static_cast<string>("[kwelse]\n"));
		}
		TEST_METHOD(kwswitch)
		{
			ostringstream stream;
			Token d(LexemType::kwswitch);
			d.print(stream);
			Assert::AreEqual(stream.str(), static_cast<string>("[kwswitch]\n"));
		}
		TEST_METHOD(kwcase)
		{
			ostringstream stream;
			Token d(LexemType::kwcase);
			d.print(stream);
			Assert::AreEqual(stream.str(), static_cast<string>("[kwcase]\n"));
		}
		TEST_METHOD(kwwhile)
		{
			ostringstream stream;
			Token d(LexemType::kwwhile);
			d.print(stream);
			Assert::AreEqual(stream.str(), static_cast<string>("[kwwhile]\n"));
		}
		TEST_METHOD(kwfor)
		{
			ostringstream stream;
			Token d(LexemType::kwfor);
			d.print(stream);
			Assert::AreEqual(stream.str(), static_cast<string>("[kwfor]\n"));
		}
		TEST_METHOD(kwreturn)
		{
			ostringstream stream;
			Token d(LexemType::kwreturn);
			d.print(stream);
			Assert::AreEqual(stream.str(), static_cast<string>("[kwreturn]\n"));
		}
		TEST_METHOD(kwin)
		{
			ostringstream stream;
			Token d(LexemType::kwin);
			d.print(stream);
			Assert::AreEqual(stream.str(), static_cast<string>("[kwin]\n"));
		}
		TEST_METHOD(kwout)
		{
			ostringstream stream;
			Token d(LexemType::kwout);
			d.print(stream);
			Assert::AreEqual(stream.str(), static_cast<string>("[kwout]\n"));
		}
		TEST_METHOD(eof)
		{
			ostringstream stream;
			Token d(LexemType::eof);
			d.print(stream);
			Assert::AreEqual(stream.str(), static_cast<string>("[eof]\n"));
		}
		TEST_METHOD(error)
		{
			ostringstream stream;
			Token d(LexemType::error);
			d.print(stream);
			Assert::AreEqual(stream.str(), static_cast<string>("[error]\n"));
		}
	};
	TEST_CLASS(ScanTest)
	{
	public:
		TEST_METHOD(example1)
		{

			istringstream istream("555 '5'\"Hello\" int x<= < ++ -52432 - () {} [] , : = + * == != || && char if else switch case while ");
			istream >> noskipws;
			ostringstream ostream;
			Scan scanner = Scan(istream);
			Token cur = scanner.getNextToken();
			cur.print(ostream);
			while (cur.type() != LexemType::eof && cur.type() != LexemType::error)
			{
				cur = scanner.getNextToken();
				cur.print(ostream);
			}
			Assert::AreEqual(ostream.str(), (string)("[num,555]\n[chr]\n[str, Hello]\n[kwint]\n[id,x]\n[ople]\n[oplt]\n[opinc]\n[num,-52432]\n[opminus]\n[lpar]\n[rpar]\n[lbrace]\n[rbrace]\n[lbracket]\n[rbracket]\n[comma]\n[colon]\n[opassign]\n[opplus]\n[opmult]\n[opeq]\n[opne]\n[opor]\n[opand]\n[kwchar]\n[kwif]\n[kwelse]\n[kwswitch]\n[kwcase]\n[kwwhile]\n[eof]\n"));

		}
		TEST_METHOD(example2)
		{

			istringstream istream("int return 555");
			istream >> noskipws;
			ostringstream ostream;
			Scan scanner = Scan(istream);
			Token cur = scanner.getNextToken();
			cur.print(ostream);
			while (cur.type() != LexemType::eof && cur.type() != LexemType::error)
			{
				cur = scanner.getNextToken();
				cur.print(ostream);
			}
			Assert::AreEqual(ostream.str(), (string)("[kwint]\n[kwreturn]\n[num,555]\n[eof]\n"));
		}
		TEST_METHOD(example3)
		{
			istringstream istream("int return");
			istream >> noskipws;
			ostringstream ostream;
			Scan scanner = Scan(istream);
			Token cur = scanner.getNextToken();
			cur.print(ostream);
			while (cur.type() != LexemType::eof && cur.type() != LexemType::error)
			{
				cur = scanner.getNextToken();
				cur.print(ostream);
			}
			Assert::AreEqual(ostream.str(), (string)("[kwint]\n[kwreturn]\n[eof]\n"));
		}
		TEST_METHOD(example4)
		{
			istringstream istream("int return '");
			istream >> noskipws;
			ostringstream ostream;
			Scan scanner = Scan(istream);
			Token cur = scanner.getNextToken();
			cur.print(ostream);
			while (cur.type() != LexemType::eof && cur.type() != LexemType::error)
			{
				cur = scanner.getNextToken();
				cur.print(ostream);
			}
			Assert::AreEqual(ostream.str(), (string)("[kwint]\n[kwreturn]\n[error,Empty char constant]\n"));
		}
		TEST_METHOD(example5)
		{
			istringstream istream("int return 'a");
			istream >> noskipws;
			ostringstream ostream;
			Scan scanner = Scan(istream);
			Token cur = scanner.getNextToken();
			cur.print(ostream);
			while (cur.type() != LexemType::eof && cur.type() != LexemType::error)
			{
				cur = scanner.getNextToken();
				cur.print(ostream);
			}
			Assert::AreEqual(ostream.str(), (string)("[kwint]\n[kwreturn]\n[error,Unclosed char constant at the end of file]\n"));
		}
		TEST_METHOD(example6)
		{
			istringstream istream("int return ''");
			istream >> noskipws;
			ostringstream ostream;
			Scan scanner = Scan(istream);
			Token cur = scanner.getNextToken();
			cur.print(ostream);
			while (cur.type() != LexemType::eof && cur.type() != LexemType::error)
			{
				cur = scanner.getNextToken();
				cur.print(ostream);
			}
			Assert::AreEqual(ostream.str(), (string)("[kwint]\n[kwreturn]\n[error,Empty char constant]\n"));
		}
		TEST_METHOD(example7)
		{
			istringstream istream("int return \"");
			istream >> noskipws;
			ostringstream ostream;
			Scan scanner = Scan(istream);
			Token cur = scanner.getNextToken();
			cur.print(ostream);
			while (cur.type() != LexemType::eof && cur.type() != LexemType::error)
			{
				cur = scanner.getNextToken();
				cur.print(ostream);
			}
			Assert::AreEqual(ostream.str(), (string)("[kwint]\n[kwreturn]\n[error,String constant is not closed]\n"));
		}
		TEST_METHOD(example8)
		{
			istringstream istream("int return &");
			istream >> noskipws;
			ostringstream ostream;
			Scan scanner = Scan(istream);
			Token cur = scanner.getNextToken();
			cur.print(ostream);
			while (cur.type() != LexemType::eof && cur.type() != LexemType::error)
			{
				cur = scanner.getNextToken();
				cur.print(ostream);
			}
			Assert::AreEqual(ostream.str(), (string)("[kwint]\n[kwreturn]\n[error,Incomplete AND operator]\n"));
		}
		TEST_METHOD(example9)
		{
			istringstream istream("int return |");
			istream >> noskipws;
			ostringstream ostream;
			Scan scanner = Scan(istream);
			Token cur = scanner.getNextToken();
			cur.print(ostream);
			while (cur.type() != LexemType::eof && cur.type() != LexemType::error)
			{
				cur = scanner.getNextToken();
				cur.print(ostream);
			}
			Assert::AreEqual(ostream.str(), (string)("[kwint]\n[kwreturn]\n[error,Incomplete OR operator]\n"));
		}
		TEST_METHOD(example10)
		{
			istringstream istream("int return 'aaa'");
			istream >> noskipws;
			ostringstream ostream;
			Scan scanner = Scan(istream);
			Token cur = scanner.getNextToken();
			cur.print(ostream);
			while (cur.type() != LexemType::eof && cur.type() != LexemType::error)
			{
				cur = scanner.getNextToken();
				cur.print(ostream);
			}
			Assert::AreEqual(ostream.str(), (string)("[kwint]\n[kwreturn]\n[error,Char constant has more then one symbol]\n"));
		}
	};
}

