#pragma once
#include <string>

#include <memory>
#include <iostream>
#include <vector>
#include "Atoms.h"
using namespace std;
class StringTable
{
public:
	StringTable();
	const string&operator [](const int index) const;
	shared_ptr<StringOperand> add(const string string);
	friend ostream & operator << (ostream & stream, const StringTable &t);
	bool operator ==(const StringTable &other) const;
	void generateStrings(std::ostream & stream) const;
protected:
	vector<string> _strings;
};
ostream & operator << (ostream & stream, const StringTable &t);
