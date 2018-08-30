#pragma once
#include "stdafx.h"
#include <sstream>
#include "Atoms.h"
#include "../scaner/StringTable.h"
#include "../scaner/SymbolTable.h"
#include "Translator.h"
#include <iomanip>

Operand::Operand()
{
}

string Operand::toString() const
{
	return string();

}
RValue::RValue() {

}
MemoryOperand::MemoryOperand(int index, const SymbolTable * symbolTable) {
	_index = index;
	_symbolTable = symbolTable;
	
}
string MemoryOperand::toString() const
{
	//auto s=(*_symbolTable)[_index]._name;
	return std::to_string(_index);
	
}
bool MemoryOperand::operator==(const MemoryOperand & other) const
{
	if ((_index == other._index) && (*_symbolTable == *other._symbolTable)) {
		return true;
	}
	return false;
}
int MemoryOperand::index() const
{
	return _index;
}
void MemoryOperand::load(std::ostream & stream) const
{
	if ((*_symbolTable)[_index]._scope == GlobalScope) {
		stream << "LDA var"<< to_string(_index)<< endl;
	}
	else {
		stream << "LXI H," << to_string((*_symbolTable)[_index]._offset)<< endl;
		stream << "DAD SP" << endl;
		stream << "MOV A,M" << endl;
	}
}
void MemoryOperand::save(std::ostream & stream) const {
	if ((*_symbolTable)[_index]._scope == GlobalScope) {
		stream << "STA var" << to_string(_index) << endl;
	}
	else {
		stream << "LXI H," << to_string((*_symbolTable)[_index]._offset) << endl;
		stream << "DAD SP" << endl;
		stream << "MOV M,A" << endl;
	}
}
int MemoryOperand::len() const
{
	return (*_symbolTable)[_index]._len;
}
string MemoryOperand::name() const
{
	return (*_symbolTable)[_index]._name;
}
NumberOperand::NumberOperand(int value) {
	_value = value;
}
string NumberOperand::toString() const
{
	int n = _value; 
	return "'"+to_string(n)+"'";
}
void NumberOperand::load(std::ostream & stream) const
{
	stream << "MVI A," << _value << endl;
}
StringOperand::StringOperand(int index, const StringTable * stringTable) {
	_index = index;
	_stringTable = stringTable;
}
string StringOperand::toString() const
{
//	auto s = (*_stringTable)[_index];
	ostringstream oss;
	oss << "S" << _index;
	return oss.str();
}
bool StringOperand::operator==(const StringOperand & other) const
{

	if ((_index == other._index)&&(*_stringTable == *other._stringTable)) {
		return true;
	}
	return false;
}
LabelOperand::LabelOperand(int labelID) {
	_labelID = labelID;
}
LabelOperand::LabelOperand(string labelID)
{
	_strlabelID = labelID;
}
string LabelOperand::toString() const
{
	if (_strlabelID != " " ){
	return _strlabelID;
	}
	int l = _labelID;
	return "L"+to_string(l);
}

string LabelOperand::name()
{
	return _strlabelID;
}

Atom::Atom()
{
}
string Atom::toString() const
{
	return string();
}

BinaryOpAtom::BinaryOpAtom(const string & name, shared_ptr<RValue> left, shared_ptr<RValue> right, shared_ptr<MemoryOperand> result)
{
	_name = name;
	_left = left;
	_right = right;
	_result = result;
}

string BinaryOpAtom::toString() const
{
	string str = "(" + _name + "," + _left->toString() + "," + _right->toString() + "," + _result->toString() + ")";
	return str;
}
void BinaryOpAtom::generate(std::ostream & stream) const
{
	stream << "\t;" << "(" << _name << "," << _left->toString() << "," << _right->toString() << "," << _result->toString() << ")" << endl;
	_right->load(stream);
	if (_name != "MUL") {
		stream << "MOV B,A" << endl;
		_left->load(stream);
		stream << _name << " B" << endl;
	}
	else {
		stream << "MOV D,A" << endl;
		_left->load(stream);
		stream << "MOV C,A" << endl;
		stream << "CAll@MULT" << endl;
		stream << "MOV A,C " << endl;
	}
	_result->save(stream);
}
UnaryOpAtom::UnaryOpAtom(const string & name, shared_ptr<RValue> operand, shared_ptr<MemoryOperand> result)
{
	_name = name;
	_operand = operand;
	_result = result;
}

string UnaryOpAtom::toString() const
{
	string str = "(" + _name + "," + _operand.get()->toString() + ",," + _result.get()->toString() + ")" ;
	return str;
}
void UnaryOpAtom::generate(std::ostream & stream) const
{
	stream<<"\t;"<<"(" + _name + "," + _operand.get()->toString() + ",," + _result.get()->toString() + ")"<< endl;
	_operand->load(stream);
	_result->save(stream);//Mov это и есть ,надо еще Not  и NEg
}
ConditionalJumpAtom::ConditionalJumpAtom(const string & condition, shared_ptr<RValue> left, shared_ptr<RValue> right, shared_ptr<LabelOperand> label)
{
	_condition = condition;
	_left = left;
	_right = right;
	_label = label;
}

string ConditionalJumpAtom::toString() const
{
	string str = "(" + _condition + "," + _left.get()->toString() + "," + _right.get()->toString() + "," + _label.get()->toString() +")";
	return str;
}
void ConditionalJumpAtom::generate(std::ostream & stream) const
{
	stream << "\t;" << "(" + _condition + "," + _left.get()->toString() + "," + _right.get()->toString() + "," + _label.get()->toString() + ")"<<endl;
	_right->load(stream);
	stream << "MOV B,A" << endl;
	_left->load(stream);
	stream << "CMP B" << endl;
	if (_condition == "EQ") {
		stream << "JZ LB" << _label->toString() << endl;
	}
	if (_condition == "LE") {
		stream << "JМ LB" << _label->toString() << endl;
		stream << "JZ LB" << _label->toString() << endl;
	}

	if (_condition == "LT") {
		stream << "JМ LB" << _label->toString() << endl;
	}
	if (_condition == "GE") {
		stream << "JP LB" << _label->toString() << endl;
		stream << "JZ LB" << _label->toString() << endl;
	}
	if (_condition == "GT") {
		stream << "JP LB" << _label->toString() << endl;
	}
}
OutAtom::OutAtom(shared_ptr<Operand> value)
{
	_value = value;
}

string OutAtom::toString() const
{
	string str = "(OUT,,," + _value.get()->toString() + ")";
	return str;
}
void OutAtom::generate(std::ostream & stream) const
{
	stream<<"\t;"<< "(OUT,,," + _value.get()->toString() + ")"<<endl;
	
}
InAtom::InAtom(shared_ptr<MemoryOperand> result)
{
	_result = result;
}

string InAtom::toString() const
{
	string str = "(IN,,," + _result.get()->toString() + ")";
	return str;
}

void InAtom::generate(std::ostream & stream) const
{
	stream << "\t;"<< "(IN,,," + _result.get()->toString() + ")"<<endl;
	stream << "IN 0" << endl;
	_result->save(stream);
}

LabelAtom::LabelAtom(shared_ptr<LabelOperand> label)
{
	_label = label;
}

string LabelAtom::toString() const
{
	string str = "(LBL,,," + _label.get()->toString() + ")";
	return str;
}

void LabelAtom::generate(std::ostream & stream) const
{
	stream << "\t;" << "(LBL,,," + _label.get()->toString() + ")" << endl;
	if (_label->name() == " ") {
		stream << "LB" << _label.get()->toString() <<":"<< endl;
	}
	else {
		stream << _label->name() << ":" << endl;
	}
	
}

JumpAtom::JumpAtom(shared_ptr<LabelOperand> label)
{
	_label = label;
}

string JumpAtom::toString() const
{
	string str = "(JMP,,," + _label.get()->toString() + ")";
	return str;
}

void JumpAtom::generate(std::ostream & stream) const
{
	stream << "\t" << ";(JMP,,," + _label.get()->toString() + ")" << endl;
	if (_label->name() == " ") {
		stream << "JMP LB" << _label.get()->toString() << endl;
	}
	else {
		stream << "JMP " << _label->name() << endl;
	}
	
}

CallAtom::CallAtom(shared_ptr<MemoryOperand> func, shared_ptr<MemoryOperand> result)
{
	_func = func;
	_result = result;
}

string CallAtom::toString() const
{
	string str = "(CALL," + _func.get()->toString() + ",," + _result.get()->toString() + ")";
	
	return str;
}

void CallAtom::saveRegs(std::ostream & stream) const
{
	stream << "PUSH B" << endl << "PUSH D" << endl << "PUSH H" << endl << "PUSH PSW" << endl;
}

void CallAtom::loadRegs(std::ostream & stream) const
{
	stream << "POP PSW" << endl << "POP H" << endl << "POP D" << endl << "POP B" << endl;
}

void CallAtom::generate(std::ostream & stream) const
{
	stream << "\t;" << "(CALL," + _func.get()->toString() + ",," + _result.get()->toString() + ")" << endl;
	saveRegs(stream);
	stream << "LXI B,0" << endl;
	stream << "PUSH B" << endl;
	auto r = _func->len();
	int H = 4;
	for (int i = 0; i < r; ++i, H+=2) {
		stream << "LXI B,0" << endl;
		stream << "LXI H," << H << endl;
		stream << "DAD SP" << endl;
		stream << "MOV A,M" << endl;
		stream << "MOV C,A" << endl;
		stream << "PUSH B" << endl;
	}
	stream << "CALL " << _func.get()->name() << endl;
	for (int i = 0; i < r; ++i) {
		stream << "POP B" << endl;
	} 
	stream << "POP B" << endl;
	stream << "MOV A,B" << endl;
	_result->save(stream);
	loadRegs(stream);
}

RetAtom::RetAtom(int scope, const SymbolTable * symbolTable, shared_ptr<RValue> retval)
{
	_scope = scope;
	_symbolTable = symbolTable;
	_retval = retval;
}

string RetAtom::toString() const
{
	string str = "(RET,,," + _retval.get()->toString() + ")";
	return str;
}

void RetAtom::generate(std::ostream & stream) const
{
	stream << "\t;" << "(RET,,," + _retval.get()->toString() + ")" << endl;
	_retval.get()->load(stream);
	int res = 0;
	if ((*_symbolTable)[_scope]._len == 0) {
		res = (*_symbolTable)[_scope + 1]._offset + 4;
	}
	else{
		res = (*_symbolTable)[_scope+1]._offset +2;
	}
	stream << "LXI H," << res << endl;
	stream << "DAD SP" << endl;
	stream << "MOV M,A" << endl;
	for (int i = 0; i < _symbolTable->getM(_scope); ++i) {
		stream << "POP" << endl;
	}
	stream << "RET" << endl;
}

ParamAtom::ParamAtom(shared_ptr<RValue> funcvalue)
{
	_funcvalue = funcvalue;
}

string ParamAtom::toString() const
{
	string str = "(PARAM,,," + _funcvalue.get()->toString() + ")";
	return str;
}

void ParamAtom::generate(std::ostream & stream) const
{
	
}

