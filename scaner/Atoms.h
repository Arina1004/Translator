#pragma once
#include <string>
#include <memory>
#include <iostream>
#include <vector>

using namespace std;

class StringTable;
class SymbolTable;

class Operand
{
public:
	Operand();
	virtual string toString() const = 0 ;
};
class RValue : public Operand
{
public:
	RValue();
	virtual void load(std::ostream & stream) const = 0;
};
class MemoryOperand :public RValue
{
public:
	MemoryOperand(int index, const SymbolTable * symbolTable);
	string toString() const;
	bool operator ==(const MemoryOperand &other) const;
	int index() const;
	void load(std::ostream & stream) const;
	void save(std::ostream & stream) const;
	int len() const;
	string name() const;
protected: 
	int _index;
	const SymbolTable * _symbolTable;
};
class NumberOperand :public RValue
{
public:
	NumberOperand(int value);
	string toString() const;
	void load(std::ostream & stream) const;
protected:
	int _value;
	
};
class StringOperand :public Operand
{
public:
	StringOperand(int index, const StringTable * stringTable);
	virtual string toString() const;
	bool operator ==(const StringOperand &other) const;
protected:
	int _index;
	const StringTable * _stringTable;
};
class LabelOperand :public Operand
{
public:
	LabelOperand(int labelID);
	LabelOperand(string labelID);//добавила
	virtual string toString() const;
	string name();
protected:
	string _strlabelID=" ";
	int _labelID;

};
class Atom
{
public:
	Atom();
	virtual string toString() const = 0;
	virtual void generate(std::ostream & stream) const = 0;
};
class BinaryOpAtom :public Atom
{
public:
	BinaryOpAtom(const string&name, shared_ptr<RValue>left, shared_ptr<RValue> right, shared_ptr<MemoryOperand> result);
	string toString() const;
	void generate(std::ostream & stream) const;
protected:
	string _name;
	shared_ptr<RValue> _left;
	shared_ptr<RValue> _right;
	shared_ptr<MemoryOperand> _result;

};
class UnaryOpAtom : public Atom {
	
public:
	UnaryOpAtom(const string &name, shared_ptr<RValue> operand,
		shared_ptr<MemoryOperand> result);
	string toString() const;
	void generate(std::ostream & stream) const;
protected:	
	string _name;
	shared_ptr<RValue> _operand;
	shared_ptr<MemoryOperand> _result;
};
class ConditionalJumpAtom : public Atom {
	
public:
	ConditionalJumpAtom(const string & condition, shared_ptr<RValue> left, shared_ptr<RValue> right, shared_ptr<LabelOperand> label);
	string toString() const;
	void generate(std::ostream & stream) const;
protected:
	string _condition;
	shared_ptr<RValue> _left;
	shared_ptr<RValue> _right;
	shared_ptr<LabelOperand> _label;
};
class OutAtom : public Atom {
	
public:
	OutAtom(shared_ptr<Operand> value);
	string toString() const;
	void generate(std::ostream & stream) const;
protected:
	shared_ptr<Operand> _value;
};
class InAtom : public Atom {
	
public:
	InAtom(shared_ptr<MemoryOperand> result);
	string toString() const;
	void generate(std::ostream & stream) const;
protected:
	shared_ptr<MemoryOperand> _result;
};
class LabelAtom : public Atom {
protected:
	shared_ptr<LabelOperand> _label;
public:
	LabelAtom(shared_ptr<LabelOperand> label);
	string toString() const;
	void generate(std::ostream & stream) const;
};
class JumpAtom : public Atom {
protected:
	shared_ptr<LabelOperand> _label;
public:
	JumpAtom(shared_ptr<LabelOperand> label);
	string toString() const;
	void generate(std::ostream & stream) const;
};

class CallAtom : public Atom {
protected:
	shared_ptr<MemoryOperand> _func;
	shared_ptr<MemoryOperand> _result;
public:
	CallAtom(shared_ptr<MemoryOperand> func, shared_ptr<MemoryOperand> result);
	string toString() const;
	void saveRegs(std::ostream & stream) const;
	void loadRegs(std::ostream & stream) const;
	void generate(std::ostream & stream) const;
};


class RetAtom : public Atom {
protected:
	int _scope;
	shared_ptr<RValue> _retval;
	const SymbolTable * _symbolTable;
public:
	RetAtom(int scope, const SymbolTable * symbolTable,shared_ptr<RValue> retval);
	string toString() const;
	void generate(std::ostream & stream) const;
};

class ParamAtom : public Atom {
protected:
	shared_ptr<RValue> _funcvalue;
public:
	ParamAtom(shared_ptr<RValue> funcvalue);
	string toString() const;
	void generate(std::ostream & stream) const;
};