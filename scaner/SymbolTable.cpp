#pragma once
#include "stdafx.h"
#include "SymbolTable.h"
#include <iomanip>


SymbolTable::SymbolTable()
{
	records = {};
}

bool SymbolTable::TableRecord::operator==(const TableRecord & other) const
{
	if ((_name == other._name) && (_len == other._len) && (_init == other._init) && (_kind == other._kind) && (_offset == other._offset) && (_scope == other._scope) && (_type == other._type)) {
		return true;
	}
	return false;
}

const SymbolTable ::TableRecord & SymbolTable::operator[](const int index) const
{
	if (index <= records.size()) {
		return records[index];
	}
	throw invalid_argument("String Table arg too large");
}


bool SymbolTable::operator==(const SymbolTable & other) const
{
	if (records == other.records) {
		return true;
	}
	return false;
}

shared_ptr<MemoryOperand> SymbolTable::addVar(const std::string & name, const Scope scope, const TableRecord::RecordType type, const int init)
{
	for (auto s : records) {
		if (s._name == name && s._scope == scope && s._type != TableRecord::RecordType::unknown) {
			return nullptr;
		}
	}
	TableRecord n;
	n._name = name;
	n._init = init;
	n._scope = scope;
	n._type = type;
	n._kind = TableRecord::RecordKind::var;
	records.push_back(n);
	return make_shared<MemoryOperand>(static_cast<int>(records.size()) - 1, this);
}

std::shared_ptr<MemoryOperand> SymbolTable::addFunc(const std::string & name, const TableRecord::RecordType type, const int len)
{
	for (auto s : records) {
		if (s._name == name) {
			return nullptr;
		}
	}
	TableRecord n;
	n._name = name;
	n._type = type;
	n._len = len;
	n._kind = TableRecord::RecordKind::func;
	records.push_back(n);
	return make_shared<MemoryOperand>(static_cast<int>(records.size()) - 1, this);
}

std::shared_ptr<MemoryOperand> SymbolTable::checkVar(const Scope scope, const std::string & name)
{
	int index = 0;
	for (auto s : records) {
		if (s._scope == scope && s._name == name) {
			if (s._kind != TableRecord::RecordKind::var) {
				return nullptr;
			}
			return make_shared<MemoryOperand>(index,this);
		}
		++index;
	}
	if (scope != GlobalScope) {
			return checkVar(GlobalScope, name);
	}
	return nullptr;
}


shared_ptr<MemoryOperand> SymbolTable::checkFunc(const std::string & name, int len)
{
	int index = 0;
	for (auto s : records) {
		if (s._scope == GlobalScope && s._name == name && s._kind == TableRecord::RecordKind::func) {
			if (s._len == len) {
				return make_shared<MemoryOperand>(index, this);
			}
		}
		++index;
	}
	return nullptr;
}

int SymbolTable::getM(int scope) const
{
	int sum = 0;
	for (auto s : records) {
		if (s._scope == scope) {
			sum += 1;
		}
	}
	return sum - records[scope]._len;
}

void SymbolTable::calculateOffset()
{
	for (int i = 0; i < static_cast<int>(records.size()); ++i) {
		if (records[i]._kind == TableRecord::RecordKind::func) {
			int lenM = getM(i);
			for (int j = i+1; j <= i + records[i]._len + lenM; ++j) {
				if (j <= i + records[i]._len) {
					records[j]._offset = lenM * 2 + 2 * (i + records[i]._len - j) + 2;
				}
				else {
					records[j]._offset = 2 * (i + records[i]._len + lenM - j);
				}
			}
		}
	}
}

vector<string> SymbolTable::functionNames() const
{
	vector<string> names;
	for (auto s : records) {
		if (s._kind == TableRecord::RecordKind::func) {
			names.push_back(s._name);
		}
	}
	return names;
}

void SymbolTable::generateGlobals(std::ostream & stream) const
{
	int index = 0;
	for (auto s : records) {

		if (s._scope == GlobalScope && s._kind != SymbolTable::TableRecord::RecordKind::func) {
			stream <<"var"<<index<<": "<<"DB "<< s._init <<endl;
		}
		++index;
	}
}



ostream & operator<<(ostream & stream, const SymbolTable & t)
{
	stream << "SYMBOL TABLE" << endl;
	stream << "--------------------------------------------------------------------------" << endl;
	stream << "code" << setw(10) << "name" << setw(10) << "kind" << setw(10) << "type" << setw(10) << "len" << setw(10) << "init" << setw(10) << "scope" << setw(10) << "offset" << endl;
	for (int i = 0; i < t.records.size(); ++i) {
		string kind = transformKind(t.records[i]._kind);
		string type = transformType(t.records[i]._type);
		string name = t.records[i]._name;
		stream << setw(10) << left << i << setw(10) << left << name << setw(10) << left << kind << setw(13) << left << type << setw(7) << left << t.records[i]._len << setw(10) << left << t.records[i]._init << setw(10) << left << t.records[i]._scope << setw(10) << left << t.records[i]._offset  <<  endl;
	}
	stream << "--------------------------------------------------------------------------" << endl;
	return stream;
}

string transformKind(SymbolTable::TableRecord::RecordKind type)
{
	switch (type) {
	case SymbolTable::TableRecord::RecordKind::unknown:
		return "unknown";
	case SymbolTable::TableRecord::RecordKind::func:
		return "func";
	case SymbolTable::TableRecord::RecordKind::var:
		return "var";
	default:
		break;
	}
	return "error";
}

string transformType(SymbolTable::TableRecord::RecordType type)
{
	switch (type) {
	case SymbolTable::TableRecord::RecordType::unknown:
		return "unknown";
	case SymbolTable::TableRecord::RecordType::chr:
		return "chr";
	case SymbolTable::TableRecord::RecordType::integer:
		return "integer";
	default:
		break;
	}
	return "error";
}

void SymbolTable::indFunc(string name, const TableRecord::RecordType type, int len)
{
	int index = 0;
	for (auto s : records) {
		if (s._type == type && s._name == name) {
			records[index]._len = len;
			break;
		}
		++index;
	}
}

int SymbolTable::indexFunc(string name, const TableRecord::RecordKind type)
{
	int index = 0;
	for (auto s : records) {
		if (s._kind == type && s._name == name) {
			return index;
		}
		++index;
	}
}

void SymbolTable::indVar(string name, Scope scope, int val)
{
	int i = 0;
	int index = 0;
	for (auto s : records) {
		if ( s._name == name && s._scope == scope) {
			i = index;
		}
		++index;
	}
	records[i]._init = val;
}

shared_ptr<MemoryOperand> SymbolTable::alloc(Scope scope)
{
	_tmpid += 1;
	TableRecord n;
	n._name = "TMP" + to_string(_tmpid - 1);
	n._scope = scope;
	n._kind = TableRecord::RecordKind::var;
	n._type = TableRecord::RecordType::integer;
	records.push_back(n);
	return make_shared<MemoryOperand>(static_cast<int>(records.size()) - 1,this);
}
