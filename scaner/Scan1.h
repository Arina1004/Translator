#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include "Token1.h"

using namespace std;

class Scan
{
	map<char, LexemType> punctuation{ { '[', LexemType::lbracket },{ ']', LexemType::rbracket },{ '*', LexemType::opmult },
	{ '(', LexemType::lpar } ,{ ')', LexemType::rpar },{ '{', LexemType::lbrace },{ '}', LexemType::rbrace },
	{ ';' , LexemType::semicolon },{ ',', LexemType::comma },{ ':', LexemType::colon } };
	map<string, LexemType> keywords{ { "return", LexemType::kwreturn },{ "char", LexemType::kwchar },
	{ "in", LexemType::kwin },{ "out", LexemType::kwout },{ "if", LexemType::kwif },{ "else", LexemType::kwelse },
	{ "for", LexemType::kwfor },{ "int", LexemType::kwint },{ "switch", LexemType::kwswitch },{ "case", LexemType::kwcase },
	{ "while", LexemType::kwwhile },{ "goto", LexemType::kwgoto },{ "do", LexemType::kwdo} };
	string Digit = "0123456789";
	string Letter = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";
	istream &_stream;
public:
	Scan(istream &stream);
	Token getNextToken();
private:
	bool in(string str, char c);
};
