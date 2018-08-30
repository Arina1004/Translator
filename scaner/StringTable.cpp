#pragma once
#include "stdafx.h"
#include "StringTable.h"


StringTable::StringTable()
{
	_strings = {};
}


const string & StringTable::operator[](const int index) const
{
	if (index <= _strings.size() ){
		return _strings[index];
		}
	throw invalid_argument("SymbolTable arg too large");
}

shared_ptr<StringOperand> StringTable::add(const string string)
{
	int i = 0;
	for (auto s:_strings) {
		if (s == string) {
			return make_shared<StringOperand>(i, this);
		}
		i++;
		
	}
	_strings.push_back(string);
	return make_shared<StringOperand>(static_cast<int>(_strings.size()) - 1, this);
}

bool StringTable::operator==(const StringTable & other) const
{
	if (_strings == other._strings) {
		return true;
	}
	return false;
}

void StringTable::generateStrings(std::ostream & stream) const
{
	int i = 0;
	for (auto s : _strings) {
		stream << "str" << i << ": " << "DB "<<"'"<<s << "'" <<", 0" << endl;
		i++;

	}
}

ostream & operator<<(ostream & stream, const StringTable & t)
{
	stream << "STRING TABLE" << endl;
	stream << "----------------------------------------------" << endl;
	for (int i = 0; i < t._strings.size(); ++i) {
		stream << i << " " << t[i] << endl;
	}
	stream << "----------------------------------------------" << endl;
	return stream;
}
