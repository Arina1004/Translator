#pragma once
#include "stdafx.h"
#include "../scaner/Translator.h"
#include <stdexcept>
#include <iomanip>
//#include "../Compiler/Compiler/Scan.h"
//#include "../Compiler/Compiler/Token.h"

void Translator::translate()
{
	StmtList();
}

SymbolTable::TableRecord Translator::SymTable(int i) {
	return _symtable[i];
}
void Translator::SymTablePrint(ostream & stream)
{
	stream << _symtable;
}

void Translator::StrTablePrint(ostream & stream)
{
	stream << _strtable;
}

Token Translator::getNextLexem()
{
	return _scanner.getNextToken();
}

Translator::Translator(istream & stream)
	:_scanner(stream), _currentlexem(LexemType::error)
{
	_currentlexem = _scanner.getNextToken();
}

void Translator::printAtoms(ostream & stream)
{
	for (auto it = _atoms.begin(); it != _atoms.end(); ++it)
	{
		for (int i = 0; i < it->second.size(); i++) {
			stream << setw(3) << it->first << "  " << it->second[i]->toString() << endl;
		}
	}
}



void Translator::generateAtom(shared_ptr<Atom> atom, Scope scope)
{
	_atoms[scope].push_back(atom);

}

shared_ptr<LabelOperand> Translator::newLabel()
{
	_labelid += 1;
	return make_shared<LabelOperand>(_labelid - 1);
}

void Translator::syntaxError(const std::string & message)
{
	cout << "syn:" << message;
	throw MyError(message);
}

void Translator::lexicalError(const std::string & message)
{
	cout << "lex:" << message;
	throw MyError(message);
}
MyError::MyError(const string & mess)
	:_msg(mess)
{
}

shared_ptr<RValue> Translator::E(Scope scope)
{
	shared_ptr<RValue> p = E7(scope);
	if (!p) {
		syntaxError("E");
	}
	return p;
}

shared_ptr<RValue> Translator::E7(Scope scope)
{
	shared_ptr<RValue> q = E6(scope);
	if (!q) {
		syntaxError("E7");
	}
	shared_ptr<RValue> p = E7_(q, scope);
	if (!p) {
		syntaxError("E7");
	}
	return p;
}

shared_ptr<RValue> Translator::E7_(shared_ptr<RValue> p, Scope scope)
{
	if (_currentlexem.type() == LexemType::error) {
		lexicalError("E7_");
	}
	if (_currentlexem.type() == LexemType::opor) {
		_currentlexem = _scanner.getNextToken();
		shared_ptr<RValue> r = E6(scope);
		if (!r) {
			syntaxError("E7_");
		}
		shared_ptr<MemoryOperand> s = _symtable.alloc(scope);
		generateAtom(make_unique<BinaryOpAtom>("OR", p, r, s), scope);
		shared_ptr<RValue> q = E7_(s, scope);
		if (!q) {
			syntaxError("E7_");
		}
		return q;
	}
	return p;
}
shared_ptr<RValue> Translator::E6(Scope scope)
{
	shared_ptr<RValue> q = E5(scope);
	if (!q) {
		syntaxError("E6");
	}
	shared_ptr<RValue> p = E6_(q, scope);
	if (!p) {
		syntaxError("E6");
	}
	return p;
}
shared_ptr<RValue> Translator::E6_(shared_ptr<RValue> p, Scope scope)
{
	if (_currentlexem.type() == LexemType::error) {
		lexicalError("E6_");
	}
	if (_currentlexem.type() == LexemType::opand) {
		_currentlexem = _scanner.getNextToken();
		shared_ptr<RValue> r = E5(scope);
		if (!r) {
			syntaxError("E6_");
		}
		shared_ptr<MemoryOperand> s = _symtable.alloc(scope);
		generateAtom(make_unique<BinaryOpAtom>("AND", p, r, s), scope);
		shared_ptr<RValue> q = E6_(s, scope);
		if (!q) {
			syntaxError("E7_");
		}
		return q;
	}
	return p;
}
shared_ptr<RValue> Translator::E5(Scope scope)
{
	shared_ptr<RValue> q = E4(scope);
	if (!q) {
		syntaxError("E5");
	}
	shared_ptr<RValue> p = E5_(q, scope);
	if (!p) {
		syntaxError("E5");
	}
	return p;
}
shared_ptr<RValue> Translator::E5_(shared_ptr<RValue> p, Scope scope)
{
	shared_ptr<LabelOperand>  l;
	Token tmp(LexemType::error);
	if (_currentlexem.type() == LexemType::error) {
		lexicalError("E5_");
	}
	if ((_currentlexem.type() == LexemType::opeq) || (_currentlexem.type() == LexemType::opne) || (_currentlexem.type() == LexemType::opnot) || (_currentlexem.type() == LexemType::opgt) || (_currentlexem.type() == LexemType::oplt) || (_currentlexem.type() == LexemType::ople)) {
		tmp = _currentlexem;
		shared_ptr<MemoryOperand> s = _symtable.alloc(scope);
		_currentlexem = _scanner.getNextToken();
		shared_ptr<RValue> r = E4(scope);
		l = newLabel();

		if (!r) {
			syntaxError("E5_");
		}
		shared_ptr<NumberOperand> one = make_shared<NumberOperand>(1);
		generateAtom(make_unique<UnaryOpAtom>("MOV", one, s), scope);
		if (tmp.type() == LexemType::opeq) {
			generateAtom(make_unique<ConditionalJumpAtom>("EQ", p, r, l), scope);
		}
		else if (tmp.type() == LexemType::opne) {
			generateAtom(make_unique<ConditionalJumpAtom>("NE", p, r, l), scope);
		}
		else if (tmp.type() == LexemType::opgt) {
			generateAtom(make_unique<ConditionalJumpAtom>("GT", p, r, l), scope);
		}
		else if (tmp.type() == LexemType::oplt) {
			generateAtom(make_unique<ConditionalJumpAtom>("LT", p, r, l), scope);
		}
		else if (tmp.type() == LexemType::ople) {
			generateAtom(make_unique<ConditionalJumpAtom>("LE", p, r, l), scope);
		}
		shared_ptr<NumberOperand> zero = make_shared<NumberOperand>(0);
		generateAtom(make_unique<UnaryOpAtom>("MOV", zero, s), scope);
		generateAtom(make_unique<LabelAtom>(l), scope);
		return s;
	}
	return p;
}

shared_ptr<RValue> Translator::E4(Scope scope)
{
	shared_ptr<RValue> q = E3(scope);
	if (!q) {
		syntaxError("E4");
	}
	shared_ptr<RValue> p = E4_(q, scope);
	if (!p) {
		syntaxError("E4");
	}
	return p;
}
shared_ptr<RValue> Translator::E4_(shared_ptr<RValue> p, Scope scope)
{

	Token tmp(LexemType::error);
	if (_currentlexem.type() == LexemType::error) {
		lexicalError("E4_");
	}
	if ((_currentlexem.type() == LexemType::opplus) || (_currentlexem.type() == LexemType::opminus)) {
		tmp = _currentlexem;
		shared_ptr<MemoryOperand> s = _symtable.alloc(scope);
		_currentlexem = _scanner.getNextToken();
		shared_ptr<RValue> r = E3(scope);
		
		if (!r) {
			syntaxError("E4_");
		}
		if (tmp.type() == LexemType::opplus) {
			generateAtom(make_unique<BinaryOpAtom>("ADD", p, r, s), scope);
		}
		else if (tmp.type() == LexemType::opminus) {
			generateAtom(make_unique<BinaryOpAtom>("SUB", p, r, s), scope);
		}
		shared_ptr<RValue> q = E4_(s, scope);
		if (!q) {
			syntaxError("E4_");
		}
		return q;
	}
	return p;
}
shared_ptr<RValue> Translator::E3(Scope scope)
{
	shared_ptr<RValue> q = E2(scope);
	if (!q) {
		lexicalError("E3");
	}
	shared_ptr<RValue> p = E3_(q, scope);
	if (!p) {
		syntaxError("E3");
	}
	return p;
}
shared_ptr<RValue> Translator::E3_(shared_ptr<RValue> p, Scope scope)
{

	if (_currentlexem.type() == LexemType::error) {
		lexicalError("E3_");
	}
	if (_currentlexem.type() == LexemType::opmult) {
		_currentlexem = _scanner.getNextToken();
		shared_ptr<RValue> r = E2(scope);
		if (!r) {
			syntaxError("E3_");
		}
		shared_ptr<MemoryOperand> s = _symtable.alloc(scope);
		generateAtom(make_unique<BinaryOpAtom>("MUL", p, r, s), scope);
		shared_ptr<RValue> q = E3_(s, scope);
		if (!q) {
			syntaxError("E3_");
		}
		return q;
	}
	return p;
}
shared_ptr<RValue> Translator::E2(Scope scope)
{
	if (_currentlexem.type() == LexemType::error) {
		lexicalError("E2");
	}
	if (_currentlexem.type() == LexemType::opnot) {
		_currentlexem = _scanner.getNextToken();
		shared_ptr<MemoryOperand> r = _symtable.alloc(scope);
		shared_ptr<RValue> q = E1(scope);
		if (!q) {
			syntaxError("E2");
		}
		generateAtom(make_unique<UnaryOpAtom>("NOT", q, r), scope);
		return r;
	}
	return E1(scope);
}

shared_ptr<RValue> Translator::E1(Scope scope)
{
	if (_currentlexem.type() == LexemType::error) {
		lexicalError("E1");
	}
	if (_currentlexem.type() == LexemType::opinc) {
		_currentlexem = _scanner.getNextToken();
		if (_currentlexem.type() != LexemType::id)
			syntaxError("not id");
		shared_ptr<MemoryOperand> id = _symtable.checkVar(scope, _currentlexem.str());
		_currentlexem = _scanner.getNextToken();
		shared_ptr<NumberOperand> one = make_shared<NumberOperand>(1);
		generateAtom(make_unique<BinaryOpAtom>("ADD", id, one, id), scope);
		return id;
	}
	if (_currentlexem.type() == LexemType::id) {
		string id = _currentlexem.str();
		_currentlexem = _scanner.getNextToken();
		shared_ptr<RValue> s = E1_(id, scope);
		return s;
	}
	if (_currentlexem.type() == LexemType::lpar) {

		_currentlexem = _scanner.getNextToken();
		shared_ptr<RValue> r = E(scope);
		if (!r) {
			syntaxError("E() - error");
		}

		if (_currentlexem.type() != LexemType::rpar) {
			syntaxError("E() - error");
		}
		_currentlexem = _scanner.getNextToken();
		return r;
	}
	if ((_currentlexem.type() == LexemType::num) || (_currentlexem.type() == LexemType::chr)) {
		int val = _currentlexem.value();
		_currentlexem = _scanner.getNextToken();
		return std::make_shared<NumberOperand>(val);
	}
	/*	if  {

	_strtable.add(_currentlexem.str());
	_currentlexem = _scanner.getNextToken();
	if !(_currentlexem) {}
	return ;
	}*/
	syntaxError("E()-error");
	return shared_ptr<RValue>();
}

shared_ptr<RValue> Translator::E1_(string p, Scope scope)
{
	
	if (_currentlexem.type() == LexemType::error) {
		lexicalError("E1_");
	}
	if (_currentlexem.type() == LexemType::opinc) {
		shared_ptr<MemoryOperand> e = _symtable.checkVar(scope, p);
		_currentlexem = _scanner.getNextToken();
		shared_ptr<MemoryOperand> r = _symtable.alloc(scope);
		shared_ptr<NumberOperand> one = make_shared<NumberOperand>(1);
		generateAtom(make_unique<UnaryOpAtom>("MOV", e, r), scope);
		generateAtom(make_unique<BinaryOpAtom>("ADD", e, one, e), scope);
		return r;
	}
	else if (_currentlexem.type() == LexemType::lpar) {
		_currentlexem = _scanner.getNextToken();
		int n = ArgList(scope);
		if (_currentlexem.type() != LexemType::rpar) {
			syntaxError("E1_");
		}
		_currentlexem = _scanner.getNextToken();
		shared_ptr<MemoryOperand> e = _symtable.checkFunc(p, n);
		shared_ptr<MemoryOperand> r = _symtable.alloc(scope);
		generateAtom(make_unique<CallAtom>(e, r), scope);
		return r;
	}
	else {
		shared_ptr<MemoryOperand> e = _symtable.checkVar(scope, p);
		return e;
	}
}
		


int Translator::ArgList(Scope scope)
{
	if (_currentlexem.type() == LexemType::opinc || _currentlexem.type() == LexemType::lpar || _currentlexem.type() == LexemType::opnot ||_currentlexem.type() == LexemType::num || _currentlexem.type() == LexemType::id || _currentlexem.type() == LexemType::chr) {
//		_currentlexem = _scanner.getNextToken();
		shared_ptr<RValue> p = E(scope);
		if (!p) {
			syntaxError("ArgList");
		}
		int m = ArgList_(scope);
		generateAtom(make_unique<ParamAtom>(p), scope);
		return m + 1;
	}
	return 0;
}
int Translator::ArgList_(Scope scope)
{
	if (_currentlexem.type() == LexemType::comma) {
		_currentlexem = _scanner.getNextToken();
		shared_ptr<RValue> p = E(scope);
		if (!p) {
			syntaxError("ArgList");
		}
		int m = ArgList_(scope);
		generateAtom(make_unique<ParamAtom>(p), scope);
		return m + 1;
	}
	return 0;
}

void Translator::DeclareStmt(Scope scope)
{
	SymbolTable::TableRecord::RecordType p = Type(scope);
	if (p == SymbolTable::TableRecord::RecordType::unknown) {
		syntaxError("DeclareStmt");
	}
	if (_currentlexem.type() != LexemType::id) {
		syntaxError("DeclareStmt");
	}
	string q = _currentlexem.str();
	_currentlexem = _scanner.getNextToken();
	DeclareStmt_(p, q, scope);
}

void Translator::DeclareStmt_(SymbolTable::TableRecord::RecordType p, string q, Scope scope)
{
	if (_currentlexem.type() == LexemType::error) {
		syntaxError("DeclareStmt_");
	}
	else if (_currentlexem.type() == LexemType::lpar) {
		if (scope > -1) {
			syntaxError("DeclareStmt_");
		}
		
		_currentlexem = _scanner.getNextToken();
		auto func = _symtable.addFunc(q, p, 0);
		scope = func->index();
		int n = ParamList(scope);
		_symtable.indFunc(q, p, n);
		if (_currentlexem.type() != LexemType::rpar) {
			syntaxError("DeclareStmt_");
		}
		_currentlexem = _scanner.getNextToken();

		if (_currentlexem.type() != LexemType::lbrace) {
			syntaxError("DeclareStmt_");
		}
		_currentlexem = _scanner.getNextToken();
		StmtList(scope);
		if (_currentlexem.type() != LexemType::rbrace) {
			syntaxError("DeclareStmt_");
		}
		_currentlexem = _scanner.getNextToken();
		shared_ptr<NumberOperand> zero = make_shared<NumberOperand>(0);
		generateAtom(make_unique<RetAtom>(scope,&_symtable, zero), scope);
	}
	else if (_currentlexem.type() == LexemType::opassign) {
		_currentlexem = _scanner.getNextToken();
		if (_currentlexem.type() != LexemType::num) {
			syntaxError("DeclareStmt_");
		}
		auto add = _symtable.addVar(q, scope, p, _currentlexem.value());
		if (!add) {
			syntaxError("DecStmt_Add");
		}
		_currentlexem = _scanner.getNextToken();
		DeclVarList_(p, scope);
	}
	else {
		auto add = _symtable.addVar(q, scope, p, _currentlexem.value());
		if (!add) {
			syntaxError("DecStmt_Add");
		}
	}
	DeclVarList_(p, scope);
	if (_currentlexem.type() == LexemType::semicolon) {
		_currentlexem = _scanner.getNextToken();
	}
}

SymbolTable::TableRecord::RecordType Translator::Type(Scope scope)
{
	if (_currentlexem.type() == LexemType::error) {
		syntaxError("Type");
	}
	if (_currentlexem.type() == LexemType::kwint) {
		_currentlexem = _scanner.getNextToken();
		return SymbolTable::TableRecord::RecordType::integer;
	}
	if (_currentlexem.type() == LexemType::kwchar) {
		_currentlexem = _scanner.getNextToken();
		return SymbolTable::TableRecord::RecordType::chr;
	}
	syntaxError("Type");
	return SymbolTable::TableRecord::RecordType::unknown;
}

void Translator::DeclVarList_(SymbolTable::TableRecord::RecordType p, Scope scope)
{
	if (_currentlexem.type() == LexemType::error) {
		lexicalError("DeclVarList_");
	}
	if (_currentlexem.type() == LexemType::comma) {
		_currentlexem = _scanner.getNextToken();
		if (_currentlexem.type() != LexemType::id) {
			syntaxError("DecVarList_");
		}
		string s = _currentlexem.str();
		_currentlexem = _scanner.getNextToken();
		InitVar(p, s, scope);
		DeclVarList_(p, scope);
	}
}

void Translator::InitVar(SymbolTable::TableRecord::RecordType p, string q, Scope scope)
{
	if (_currentlexem.type() == LexemType::error) {
		syntaxError("InitVar");
	}
	if (_currentlexem.type() == LexemType::opassign) {
		_currentlexem = _scanner.getNextToken();
		if (_currentlexem.type() != LexemType::num || _currentlexem.type() != LexemType::chr) {
			syntaxError("InitVar");
		}
		auto add = _symtable.addVar(q, scope, p, _currentlexem.value());
		if (!add) {
			syntaxError("InitVar_Add");
		}
		_currentlexem = _scanner.getNextToken();
	}
	else {
		auto add = _symtable.addVar(q, scope, p, _currentlexem.value());
		if (!add) {
			syntaxError("InitVar_Add_v2");
		}
	}
}

int Translator::ParamList(Scope scope)
{
	int s = -1;
	if (_currentlexem.type() == LexemType::rpar) {
		return s + 1;
	}
	SymbolTable::TableRecord::RecordType q = Type(scope);
	if (q == SymbolTable::TableRecord::RecordType::unknown) {
		syntaxError("ParamList"); 
	}
	if (_currentlexem.type() == LexemType::id) {
		auto add = _symtable.addVar(_currentlexem.str(), scope, q);
		if (!add) {
			syntaxError("ParList_Add");
		}
		_currentlexem = _scanner.getNextToken();
		s = ParamList_(scope);
	}
	return s + 1;
}

int Translator::ParamList_(Scope scope)
{
	int s = -1;
	if (_currentlexem.type() == LexemType::error) {
		syntaxError("ParamList_");
	}
	if (_currentlexem.type() == LexemType::comma) {
		_currentlexem = _scanner.getNextToken();
		SymbolTable::TableRecord::RecordType q = Type(scope);
		if (q == SymbolTable::TableRecord::RecordType::unknown) {
			syntaxError("ParamList");
		}
		if (_currentlexem.type() != LexemType::id) {
			syntaxError("ParamList");
		}
		auto add = _symtable.addVar(_currentlexem.str(), scope, q);
		if (!add) {
			syntaxError("ParList__Add");
		}
		_currentlexem = _scanner.getNextToken();
		s = ParamList_(scope);
	}
	return s + 1;
}

void Translator::StmtList(Scope scope)
{
	if (_currentlexem.type() == LexemType::error) {
		lexicalError("StmtList");
	}
	if (_currentlexem.type() == LexemType::kwgoto || _currentlexem.type() == LexemType::kwdo ||_currentlexem.type() == LexemType::kwchar || _currentlexem.type() == LexemType::kwint || _currentlexem.type() == LexemType::id || _currentlexem.type() == LexemType::kwwhile || _currentlexem.type() == LexemType::kwfor || _currentlexem.type() == LexemType::kwif || _currentlexem.type() == LexemType::kwswitch || _currentlexem.type() == LexemType::kwin || _currentlexem.type() == LexemType::kwout || _currentlexem.type() == LexemType::semicolon || _currentlexem.type() == LexemType::lbrace || _currentlexem.type() == LexemType::kwreturn) {//добавила
		Stmt(scope);
		StmtList(scope);
	}
}

void Translator::Stmt(Scope scope)
{
	if (_currentlexem.type() == LexemType::error) {
		lexicalError("Stmt");
	}
	if (_currentlexem.type() == LexemType::kwchar || _currentlexem.type() == LexemType::kwint) {
		DeclareStmt(scope);
		return;
	}
	if (_currentlexem.type() == LexemType::id ||  _currentlexem.type() == LexemType::kwdo ||_currentlexem.type() == LexemType::kwwhile || _currentlexem.type() == LexemType::kwfor || _currentlexem.type() == LexemType::kwif || _currentlexem.type() == LexemType::kwswitch || _currentlexem.type() == LexemType::kwin || _currentlexem.type() == LexemType::kwout || _currentlexem.type() == LexemType::semicolon || _currentlexem.type() == LexemType::lbrace || _currentlexem.type() == LexemType::kwreturn) {
		if (scope == GlobalScope) {
			syntaxError("Error: operator is outside function definition");
		}
	}
	if (_currentlexem.type() == LexemType::id) {
		string l = _currentlexem.str();
		_currentlexem = _scanner.getNextToken();
		if (_currentlexem.type() == LexemType::colon) {
			_currentlexem = _scanner.getNextToken();
			shared_ptr<LabelOperand> lab = make_shared<LabelOperand>(l);
			generateAtom(make_unique<LabelAtom>(lab), scope);
		}
		else {

		AssignOrCall_(scope, l);
		}
		if (_currentlexem.type() == LexemType::semicolon) {
			_currentlexem = _scanner.getNextToken();
		}
		
		return;
	}
	if (_currentlexem.type() == LexemType::kwwhile) {
		WhileOp(scope);
		return;
	}
	
	if (_currentlexem.type() == LexemType::kwdo) {
		DoOp(scope);
		return;
	}

	if (_currentlexem.type() == LexemType::kwfor) {
		ForOp(scope);
		return;
	}
	if (_currentlexem.type() == LexemType::kwif) {
		IfOp(scope);
		return;
	}
	if (_currentlexem.type() == LexemType::kwswitch) {
		SwitchOp(scope);
	}
	if (_currentlexem.type() == LexemType::kwin) {
		IOp(scope);
		return;
	}
	if (_currentlexem.type() == LexemType::kwout) {
		OOp(scope);
		return;
	}
	if (_currentlexem.type() == LexemType::kwgoto) {
		GotoOp(scope);
		return;
	}
	if (_currentlexem.type() == LexemType::lbrace) {
		_currentlexem = _scanner.getNextToken();
		StmtList(scope);
		if (_currentlexem.type() != LexemType::rbrace) {
			syntaxError("Stmt");
		}
		_currentlexem = _scanner.getNextToken();
		return;
	}
	if (_currentlexem.type() == LexemType::kwreturn) {
		_currentlexem = _scanner.getNextToken();
		shared_ptr<RValue> p = E(scope);
		generateAtom(make_unique<RetAtom>(scope, &_symtable,p), scope);
		if (_currentlexem.type() != LexemType::semicolon) {
			syntaxError("Stmt");
		}
		_currentlexem = _scanner.getNextToken();
		return;
	}
	if (_currentlexem.type() == LexemType::semicolon) {
		_currentlexem = _scanner.getNextToken();
	}
}

void Translator::AssignOrCallOp(Scope scope)
{
	if (_currentlexem.type() == LexemType::error) {
		lexicalError("AssignOrCallOp");
	}
	AssignOrCall(SymbolTable::TableRecord::RecordType::unknown,scope);
	if (_currentlexem.type() != LexemType::semicolon) {
		syntaxError("AssignOrCallOp");
	}
	_currentlexem = _scanner.getNextToken();
}

void Translator::AssignOrCall(SymbolTable::TableRecord::RecordType type, Scope scope) {
	if (_currentlexem.type() == LexemType::error) {
		lexicalError("AssignOrCall");
	}

	if (_currentlexem.type() != LexemType::id) {
		syntaxError("AssignOrCall");
	}
	string q = _currentlexem.str();
	_currentlexem = _scanner.getNextToken();
	AssignOrCall_(scope, q);

}
void Translator::AssignOrCall_(Scope scope, string p) {
	
	if (_currentlexem.type() == LexemType::error) {
		lexicalError("AssignOrCall_");
	}
	if (_currentlexem.type() == LexemType::opassign) {
		_currentlexem = _scanner.getNextToken();
		auto r = _symtable.checkVar(scope,p);
		shared_ptr<RValue> q = E(scope);
		if (!q) {
			syntaxError("AssignOrCall_");
		}
		
		generateAtom(make_unique<UnaryOpAtom>("MOV", q, r), scope);
	}
	if (_currentlexem.type() == LexemType::lpar) {
		_currentlexem = _scanner.getNextToken();
		int n = ArgList(scope);
		shared_ptr<MemoryOperand> q = _symtable.checkFunc(p, n);
		if (_currentlexem.type() != LexemType::rpar) {
			syntaxError("AssignOrCall_");
		}
		_currentlexem = _scanner.getNextToken();
		shared_ptr<MemoryOperand> r = _symtable.alloc(scope);
		generateAtom(make_unique<CallAtom>(q, r), scope);
	}

}

void Translator::WhileOp(Scope scope)
{
	if (_currentlexem.type() == LexemType::error) {
		lexicalError("WhileOp");
	}
	if (_currentlexem.type() == LexemType::kwwhile) {
		_currentlexem = _scanner.getNextToken();
		shared_ptr<LabelOperand> l1 = newLabel();
		generateAtom(make_unique<LabelAtom>(l1), scope);
		if (_currentlexem.type() != LexemType::lpar) {
			syntaxError("WhileOp");
		}
		_currentlexem = _scanner.getNextToken();
		shared_ptr<RValue> r = E(scope);
		if (_currentlexem.type() != LexemType::rpar) {
			syntaxError("WhileOp");
		}
		_currentlexem = _scanner.getNextToken();
		shared_ptr<LabelOperand> l2 = newLabel();
		shared_ptr<NumberOperand> zero = make_shared<NumberOperand>(0);
		generateAtom(make_unique<ConditionalJumpAtom>("EQ",r, zero, l2), scope);
		Stmt(scope);
		generateAtom(make_unique<JumpAtom>(l1), scope);
		generateAtom(make_unique<LabelAtom>(l2), scope);
	}
}

void Translator::DoOp(Scope scope)
{
	if (_currentlexem.type() == LexemType::error) {
		lexicalError("DoOp");
	}
	if (_currentlexem.type() == LexemType::kwdo) {
		_currentlexem = _scanner.getNextToken();
		shared_ptr<LabelOperand> l1 = newLabel();
		generateAtom(make_unique<LabelAtom>(l1), scope);
		if (_currentlexem.type() != LexemType::lbrace) {
			syntaxError("DoOp-1");
		}
		_currentlexem = _scanner.getNextToken();
		Stmt(scope);
		if (_currentlexem.type() != LexemType::rbrace) {
			syntaxError("DoOp-2");
		}
		_currentlexem = _scanner.getNextToken();
		if (_currentlexem.type() != LexemType::kwwhile) {
			syntaxError("DoOp-3");
		}
		_currentlexem = _scanner.getNextToken();
		if (_currentlexem.type() != LexemType::lpar) {
			syntaxError("DoOp-4");
		}
		_currentlexem = _scanner.getNextToken();
		shared_ptr<RValue> r = E(scope);
		if (_currentlexem.type() != LexemType::rpar) {
			syntaxError("DoOp-3");
		}
		_currentlexem = _scanner.getNextToken();
		shared_ptr<NumberOperand> zero = make_shared<NumberOperand>(0);
		shared_ptr<LabelOperand> l3 = newLabel();
		generateAtom(make_unique<ConditionalJumpAtom>("EQ", r, zero, l3), scope);
		generateAtom(make_unique<JumpAtom>(l1), scope);
		generateAtom(make_unique<LabelAtom>(l3), scope);
	}
}

void Translator::ForOp(Scope scope)
{
	if (_currentlexem.type() == LexemType::error) {
		lexicalError("ForOp");
	}
	if (_currentlexem.type() == LexemType::kwfor) {
		_currentlexem = _scanner.getNextToken();
		if (_currentlexem.type() != LexemType::lpar) {
			syntaxError("ForOp");
		}
		_currentlexem = _scanner.getNextToken();
		ForInit(scope);
		shared_ptr<LabelOperand> l1 = newLabel();
		generateAtom(make_unique<LabelAtom>(l1), scope);
		shared_ptr<RValue> p = ForExp(scope);
		if (_currentlexem.type() != LexemType::semicolon) {
			syntaxError("ForOp");
		}
		_currentlexem = _scanner.getNextToken();
		shared_ptr<LabelOperand> l2 = newLabel();
		shared_ptr<LabelOperand> l3 = newLabel();
		shared_ptr<LabelOperand> l4 = newLabel();
		shared_ptr<NumberOperand> zero = make_shared<NumberOperand>(0);
		generateAtom(make_unique<ConditionalJumpAtom>("EQ",p, zero, l4), scope);
		generateAtom(make_unique<JumpAtom>(l3), scope);
		generateAtom(make_unique<LabelAtom>(l2), scope);
		ForLoop(scope);
		generateAtom(make_unique<JumpAtom>(l1), scope);
		if (_currentlexem.type() != LexemType::rpar) {
			syntaxError("ForOp");
		}
		_currentlexem = _scanner.getNextToken();
		generateAtom(make_unique<LabelAtom>(l3), scope);
		Stmt(scope);
		generateAtom(make_unique<JumpAtom>(l2), scope);
		generateAtom(make_unique<LabelAtom>(l4), scope);
	}
}



void Translator::ForInit(Scope scope)
{
	if (_currentlexem.type() == LexemType::error) {
		lexicalError("ForInit");
	}
	if (_currentlexem.type() == LexemType::kwint || _currentlexem.type() == LexemType::kwchar) {
		DeclareStmt(scope);
	}
}

shared_ptr<RValue> Translator::ForExp(Scope scope)
{
	if (_currentlexem.type() == LexemType::error) {
		lexicalError("ForExp");
	}
	if (_currentlexem.type() == LexemType::opinc || _currentlexem.type() == LexemType::lpar || _currentlexem.type() == LexemType::opnot || _currentlexem.type() == LexemType::num || _currentlexem.type() == LexemType::id || _currentlexem.type() == LexemType::chr) {
		shared_ptr<RValue> q = E(scope);
		return q;
	}
	shared_ptr<NumberOperand> one = make_shared<NumberOperand>(1);
	return one;
}

void Translator::ForLoop(Scope scope)
{
	if (_currentlexem.type() == LexemType::error) {
		lexicalError("ForLoop");
	}
	if (_currentlexem.type() == LexemType::id) {
		_currentlexem = _scanner.getNextToken();
		AssignOrCall(SymbolTable::TableRecord::RecordType::unknown,scope);
	}
	if (_currentlexem.type() == LexemType::opinc) {
		_currentlexem = _scanner.getNextToken();
		if (_currentlexem.type() != LexemType::id) {
			syntaxError("ForLoop");
		}
		string name = _currentlexem.str();
		auto p = _symtable.checkVar(scope, name);
		shared_ptr<NumberOperand> one = make_shared<NumberOperand>(1);
		generateAtom(make_unique<BinaryOpAtom>("ADD", p, one, p), scope);
		_currentlexem = _scanner.getNextToken();
	}
}

void Translator::IfOp(Scope scope)
{
	if (_currentlexem.type() == LexemType::error) {
		lexicalError("IfOp");
	}
	if (_currentlexem.type() == LexemType::kwif) {
		shared_ptr<LabelOperand> l1 = newLabel();
		shared_ptr<LabelOperand> l2 = newLabel();
		_currentlexem = _scanner.getNextToken();
		if (_currentlexem.type() != LexemType::lpar) {
			syntaxError("IfOp");
		}
		_currentlexem = _scanner.getNextToken();
		shared_ptr<RValue> p = E(scope);
		if (!p) {
			syntaxError("IfOp");
		}
		if (_currentlexem.type() != LexemType::rpar) {
			syntaxError("IfOp");
		}
		_currentlexem = _scanner.getNextToken();
		shared_ptr<NumberOperand> zero = make_shared<NumberOperand>(0);
		generateAtom(make_unique<ConditionalJumpAtom>("EQ", p, zero, l1), scope);
		Stmt(scope);
		generateAtom(make_unique<JumpAtom>(l2), scope);
		generateAtom(make_unique<LabelAtom>(l1), scope);
		ElsePart(scope);
		generateAtom(make_unique<LabelAtom>(l2), scope);
	}
}

void  Translator::ElsePart(Scope scope) {
	if (_currentlexem.type() == LexemType::error) {
		lexicalError("ElsePart");
	}
	if (_currentlexem.type() == LexemType::kwelse) {
		_currentlexem = _scanner.getNextToken();
		Stmt(scope);
	}
}

void Translator::SwitchOp(Scope scope)
{
	if (_currentlexem.type() == LexemType::error) {
		lexicalError("SwitchOp");
	}
	if (_currentlexem.type() == LexemType::kwswitch) {
		_currentlexem = _scanner.getNextToken();
		if (_currentlexem.type() != LexemType::lpar) {
			syntaxError("SwitchOp");
		}
		_currentlexem = _scanner.getNextToken();
		shared_ptr<RValue> p = E(scope);
		if (_currentlexem.type() != LexemType::rpar) {
			syntaxError("SwitchOp");
		}
		_currentlexem = _scanner.getNextToken();
		if (_currentlexem.type() != LexemType::lbrace) {
			syntaxError("SwitchOp");
		}
		_currentlexem = _scanner.getNextToken();
		shared_ptr<LabelOperand> end = newLabel();
		Cases(p, end, scope);
		if (_currentlexem.type() != LexemType::rbrace) {
			syntaxError("SwitchOp");
		}
		_currentlexem = _scanner.getNextToken();
		generateAtom(make_unique<LabelAtom>(end), scope);
	}
}

void Translator::Cases(shared_ptr<RValue> p, shared_ptr<LabelOperand> end, Scope scope)
{
	if (_currentlexem.type() == LexemType::error) {
		lexicalError("Cases");
	}
	shared_ptr<LabelOperand> def = ACase(p, end, scope);
	Cases_(p, end, def, scope);
}

shared_ptr<LabelOperand> Translator::ACase(shared_ptr<RValue> p, shared_ptr<LabelOperand> end, Scope scope)
{
	if (_currentlexem.type() == LexemType::error) {
		lexicalError("ACase");
	}
	auto next = newLabel();
	if (_currentlexem.type() == LexemType::kwcase) {
		_currentlexem = _scanner.getNextToken();
		if (_currentlexem.type()!= LexemType::num) {
			syntaxError("ACase_1");
		}
		auto val = _currentlexem.value();
		shared_ptr<NumberOperand> v = make_shared<NumberOperand>(val);
		generateAtom(make_unique<ConditionalJumpAtom>("NE", p, v, next), scope);
		_currentlexem = _scanner.getNextToken();
		if (_currentlexem.type() != LexemType::colon) {
			syntaxError("ACase_2");
		}
		_currentlexem = _scanner.getNextToken();
		Stmt(scope);
		generateAtom(make_unique<JumpAtom>(end), scope);
		generateAtom(make_unique<LabelAtom>(next), scope);
		return nullptr;
	}
	return nullptr;
}

void Translator::Cases_(shared_ptr<RValue> p, shared_ptr<LabelOperand> end, shared_ptr<LabelOperand> def, Scope scope)
{
	if (_currentlexem.type() == LexemType::error) {
		lexicalError("Cases_");
	}
	if (_currentlexem.type() == LexemType::kwcase) {
		shared_ptr<LabelOperand> _def = ACase(p, end, scope);
		shared_ptr<LabelOperand> defT = nullptr;
		if (def != nullptr && _def!=nullptr) {
			syntaxError("Cases_two default sections");
		}
		else {
			if (def) {
				shared_ptr<LabelOperand> defT = def;
			}
			else if (_def) {
				shared_ptr<LabelOperand> defT = _def;
			}
		}
		Cases_(p, end, defT, scope);
		return;
	}
	if (def) {
		shared_ptr<LabelOperand> q = def;
		generateAtom(make_unique<JumpAtom>(q), scope);
	}
	else {
		shared_ptr<LabelOperand> q = end;
		generateAtom(make_unique<JumpAtom>(q), scope);
	}
}

void Translator::IOp(Scope scope)
{
	if (_currentlexem.type() == LexemType::error) {
		lexicalError("IOp");
	}
	if (_currentlexem.type() == LexemType::kwin) {
		_currentlexem = _scanner.getNextToken();
		if (_currentlexem.type() == LexemType::id) {
			auto p = _symtable.checkVar(scope, _currentlexem.str());
			_currentlexem = _scanner.getNextToken();
				if (_currentlexem.type() != LexemType::semicolon) {
					syntaxError("IOp");
				}
			_currentlexem = _scanner.getNextToken();
			generateAtom(make_unique<InAtom>(p), scope);
		}
	}
	
}

void Translator::OOp(Scope scope)
{
	if (_currentlexem.type() == LexemType::error) {
		lexicalError("OOp");
	}
	if (_currentlexem.type() == LexemType::kwout) {
		_currentlexem = _scanner.getNextToken();
		OOp_(scope);
		if (_currentlexem.type() != LexemType::semicolon) {
			syntaxError("OOp");
		}
		_currentlexem = _scanner.getNextToken();
	}
}

void Translator::OOp_(Scope scope)
{
	if (_currentlexem.type() == LexemType::error) {
		lexicalError("OOp_");
	}
	if (_currentlexem.type() == LexemType::opinc || _currentlexem.type() == LexemType::lpar || _currentlexem.type() == LexemType::opnot || _currentlexem.type() == LexemType::num || _currentlexem.type() == LexemType::id || _currentlexem.type() == LexemType::chr) {
		_currentlexem = _scanner.getNextToken();
		shared_ptr<Operand> p = E(scope);
		generateAtom(make_unique<OutAtom>(p), scope);
	}
	if (_currentlexem.type() == LexemType::str) {
		shared_ptr<StringOperand> str = _strtable.add(_currentlexem.str());
		generateAtom(make_unique<OutAtom>(str), scope);
		_currentlexem = _scanner.getNextToken();
	}
}

void Translator::GotoOp(Scope scope)
{
	if (_currentlexem.type() == LexemType::error) {
		lexicalError("GotoOp");
	}
	if (_currentlexem.type() == LexemType::kwgoto) {
		_currentlexem = _scanner.getNextToken();
		if (_currentlexem.type() != LexemType::id) {
			syntaxError("GotoOp");
		}

		shared_ptr<LabelOperand> lab = make_shared<LabelOperand>(_currentlexem.str());
		generateAtom(make_unique<JumpAtom>(lab), scope);
		_currentlexem = _scanner.getNextToken();
	}
}

void Translator::generateProlog(std::ostream & stream)
{
	stream << "ORG 0" << endl;
	stream << "LXI H, 0" << endl;
	stream << "SPHL" << endl;
	stream << "CALL main" << endl;
	stream << "END " << endl;
	stream << "@MULT:" << endl;
	stream << "; Code for MULT libraru function" << endl;
	stream << "@PRINT:" << endl;
	stream << "; Code for PRINT libraru function" << endl;
}

void Translator::generateFunction(std::ostream & stream, std::string function)
{
	stream << endl;
	stream << function << ":" << endl;
	stream << "LXI B,0" << endl;
	int indF = _symtable.indexFunc(function, SymbolTable::TableRecord::RecordKind::func);
	int m = _symtable.getM(indF);
	for (int i = 0; i < m; ++i) {
		stream << "PUSH B"<<endl;
	}
	vector<shared_ptr<Atom>> funcatoms = _atoms[indF];
	for (int i = 0; i < static_cast<int>(funcatoms.size()); ++i) {
		funcatoms[i].get()->generate(stream);
	}
}

void Translator::generateCode(std::ostream & stream)
{
	vector<string> name;
	_symtable.calculateOffset();
	stream << "ORG 8000H" << endl;
	_symtable.generateGlobals(stream);
	_strtable.generateStrings(stream);
	generateProlog(stream);
	name = _symtable.functionNames();
	for (auto it = name.begin(); it != name.end(); ++it) {
		generateFunction(stream, (*it));
	}
}
