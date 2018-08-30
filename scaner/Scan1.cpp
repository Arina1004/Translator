#pragma once
#include "stdafx.h"
#include "Scan1.h"
#include <map>


Scan::Scan(istream &stream)
	: _stream(stream)
{

}

bool Scan::in(string str, char c) {
	for (int i = 0; i < str.size(); ++i) {
		if (str[i] == c) {
			return true;
		}
	}
	return false;
}

Token Scan::getNextToken() {
	int state = 0;
	char c = ' ';
	int value = 0;
	string str = " ";
	while (true) {
		_stream >> std::noskipws >> c;
		if (_stream.eof()) {
			break;
		}
		if (state == 0) {
			if (in(Digit, c)) {
				state = 1;
				value = int(c - '0');
				continue;
			}
			if (to_string(c) == "39") {
				state = 2;
				continue;
			}
			if (c == '"') {
				state = 4;
				continue;
			}
			if (in(Letter, c)) {
				state = 5;
				str = c;
				continue;
			}
			if (c == '<') {
				state = 8;
				continue;
			}
			if (c == '-') {
				state = 6;
				continue;
			}
			if (c == '!') {
				state = 7;
				continue;
			}
			if (c == '=') {
				state = 9;
				continue;
			}
			if (c == '+') {
				state = 10;
				continue;
			}
			if (c == '|') {
				state = 11;
				continue;
			}
			if (c == '&') {
				state = 12;
				continue;
			}
			if (c == '>') {
				state = 13;
				continue;
			}
			if (c == '*') {
				return Token::Token(punctuation[c]);
			}

			if (c == '(') {
				return Token::Token(punctuation[c]);
			}
			if (c == ')') {
				return Token::Token(punctuation[c]);
			}
			if (c == '{') {
				return Token::Token(punctuation[c]);
			}
			if (c == '}') {
				return Token::Token(punctuation[c]);
			}
			if (c == '[') {
				return Token::Token(punctuation[c]);
			}
			if (c == ']') {
				return Token::Token(punctuation[c]);
			}
			if (c == ':') {
				return Token::Token(punctuation[c]);
			}
			if (c == ';') {
				return Token::Token(punctuation[c]);
			}
			if (c == ',') {
				return Token::Token(punctuation[c]);
			}
			if ((c == ' ') || (c == '\n') || (c == '\t')) {
				continue;
			}
			return Token(LexemType::error, "Unknown symbol");
		}
		if (state == 1) {
			if (!(in(Digit, c))) {
				state = 0;
				_stream.putback(c);
				return Token(value);
			}
			if (value < 0) {
				value = value * 10 - int(c - '0');
			}
			else {
				value = value * 10 + int(c - '0');
			}
			continue;
		}
		if (state == 2) {
			if (to_string(c) == "39") {
				return Token(LexemType::error, "Empty char constant");
			}
			str = c;
			state = 3;
			continue;
		}
		if (state == 3) {
			if (to_string(c) == "39") {
				state = 0;
				return Token(str[0]);
			}
			return Token(LexemType::error, "Char constant has more then one symbol");
		}

		if (state == 4) {
			if (c == '"') {
				state = 0;
				return Token(LexemType::str, str);
			}
			str += c;
			continue;
		}
		if (state == 5) {
			if ((in(Digit, c)) || (in(Letter, c))) {
				str += c;
				continue;
			}
			_stream.putback(c);
			state = 0;
			if (keywords.count(str) != 0) {
				return Token(keywords[str]);
			}
			return Token(LexemType::id, str);
		}
		if (state == 6) {
			if (in(Digit, c)) {
				value = (-1) * int(c - '0');
				state = 1;
				continue;
			}
			_stream.putback(c);
			state = 0;
			return Token(LexemType::opminus);
		}
		if (state == 7) {
			state = 0;
			if (c == '=') {
				return Token(LexemType::opne);
			}
			_stream.putback(c);
			return Token(LexemType::opnot);
		}
		if (state == 8) {
			state = 0;
			if (c == '=') {
				return Token(LexemType::ople);
			}
			_stream.putback(c);
			return Token(LexemType::oplt);
		}
		if (state == 9) {
			state = 0;
			if (c == '=') {
				return Token(LexemType::opeq);
			}
			_stream.putback(c);
			return Token(LexemType::opassign);
		}
		if (state == 10) {
			state = 0;
			if (c == '+') {
				return Token(LexemType::opinc);
			}
			_stream.putback(c);
			return Token(LexemType::opplus);
		}
		if (state == 11) {
			if (c == '|') {
				state = 0;
				return Token(LexemType::opor);
			}
			return Token(LexemType::error, "Incomplete OR operator");
		}
		if (state == 12) {
			if (c == '&') {
				state = 0;
				return Token(LexemType::opand);
			}
			return Token(LexemType::error, "Incomplete AND operator");
		}
		if (state == 13) {
			state = 0;
			if (c == '=') {
				return Token(LexemType::opme);
			}
			_stream.putback(c);
			return Token(LexemType::opgt);
		}
		perror("[Error]");
	}
	if (state == 11) {
		return Token(LexemType::error, "Incomplete OR operator");
	}
	if (state == 12) {
		return Token(LexemType::error, "Incomplete AND operator");
	}
	if (state == 1) {
		return Token(value);
	}
	if (state == 5) {
		if (keywords.count(str) != 0) {
			return Token(keywords[str]);
		}
		return Token(LexemType::id, str);
	}
	if (state == 2) {
		return Token(LexemType::error, "Empty char constant");
	}
	if (state == 3) {
		return Token(LexemType::error, "Unclosed char constant at the end of file");
	}
	if (state == 4) {
		return Token(LexemType::error, "String constant is not closed");
	}
	return Token(LexemType::eof);
}