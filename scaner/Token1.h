#pragma once

#include <iostream>
#include <string>

using namespace std;

enum class LexemType {
	num, chr, str, id, lpar, rpar, lbrace, rbrace, lbracket, rbracket,
	semicolon, comma, colon, opassign, opplus, opminus, opmult, opinc, opeq, opne, oplt,
	opgt, ople, opme, opnot, opor, opand, kwint, kwchar, kwif, kwelse, kwswitch, kwcase, kwwhile,
	kwfor, kwreturn, kwin, kwout, eof, error, kwgoto, kwdo
};

class Token
{
	LexemType _type = LexemType::error;
	int _value = 0;
	string _str;
public:
	void print(ostream &stream);
	Token(LexemType type);
	Token(int value);
	Token(LexemType type, const string & str);
	Token(char c);
	LexemType type();
	int value();
	string str();
private:
	string transform(LexemType type);
};