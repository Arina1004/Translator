#pragma once
#include <string>

#include <memory>
#include <iostream>
#include <vector>
#include"Atoms.h"
using namespace std;
typedef int Scope;
const Scope GlobalScope = -1;
class SymbolTable
{
public:
	SymbolTable();
	struct TableRecord
	{
		enum class RecordKind { unknown, var, func };
		enum class RecordType { unknown, integer, chr };
		RecordKind _kind = RecordKind::unknown;
		RecordType _type = RecordType::unknown;
		int _len = -1;
		int _init = 0;
		Scope _scope = GlobalScope;
		int _offset = -1;
		string _name;
		bool operator ==(const TableRecord &other) const;

	}; 
	int _tmpid = 0;
	void SymbolTable::indVar(string name,  Scope scope , int val);
	void indFunc(string name, const TableRecord::RecordType type, int len);
	shared_ptr<MemoryOperand> alloc(Scope scope);
	int indexFunc(string name, const TableRecord::RecordKind type);
	const TableRecord & operator [](const int index) const;
	friend ostream & operator << (ostream & stream, const SymbolTable &t);
	bool operator ==(const SymbolTable &other) const;
	shared_ptr<MemoryOperand> addVar(const std::string & name,
		const Scope scope = GlobalScope,
		const TableRecord::RecordType type = TableRecord::RecordType::unknown,
		const int init = 0);
	shared_ptr<MemoryOperand> addFunc(const std::string & name,
		const TableRecord::RecordType type = TableRecord::RecordType::unknown,
		const int len = -1);	shared_ptr<MemoryOperand> checkVar(const Scope scope,
		const std::string &name);
	shared_ptr<MemoryOperand> checkFunc(const std::string &name,
		int len);
	int getM(int scope) const;
	void calculateOffset();
	vector<std::string> functionNames() const;
	void generateGlobals(std::ostream & stream) const;
protected:
	vector<TableRecord> records;
};
ostream & operator << (ostream & stream, const SymbolTable &t);
string transformKind(SymbolTable::TableRecord::RecordKind type);
string transformType(SymbolTable::TableRecord::RecordType type);

