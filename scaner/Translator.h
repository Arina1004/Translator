
#pragma once
#include <string>
#include <memory>
#include <iostream>
#include <vector>
#include "../scaner/Atoms.h"
#include "../scaner/StringTable.h"
#include "../scaner/SymbolTable.h"
#include "Token1.h"
#include "Scan1.h"

using namespace std;
class Translator
{
protected:
	Token _currentlexem;
	map<Scope, vector<shared_ptr<Atom>>> _atoms;
	
	StringTable _strtable;
	Scan _scanner;
	int _labelid = 0;
public:
	SymbolTable _symtable;
	void translate();
	SymbolTable::TableRecord SymTable(int i);
	void SymTablePrint(ostream &stream);
	void StrTablePrint(ostream &stream);
	Token getNextLexem();
	Translator(istream &stream);
	void printAtoms(ostream &stream);
	void generateAtom(shared_ptr<Atom> atom, Scope scope);
	shared_ptr<LabelOperand> newLabel();
	void syntaxError(const std::string &message);
	void lexicalError(const std::string &message);
	shared_ptr<RValue> E(Scope scope = GlobalScope);
	shared_ptr<RValue> E1(Scope scope);
	shared_ptr<RValue> E2(Scope scope);
	shared_ptr<RValue> E3(Scope scope);
	shared_ptr<RValue> E4(Scope scope);
	shared_ptr<RValue> E5(Scope scope);
	shared_ptr<RValue> E6(Scope scope);
	shared_ptr<RValue> E7(Scope scope);
	shared_ptr<RValue> E7_(shared_ptr<RValue> p, Scope scope);
	shared_ptr<RValue> E6_(shared_ptr<RValue> p, Scope scope);
	shared_ptr<RValue> E1_(string p, Scope scope);
	shared_ptr<RValue> E3_(shared_ptr<RValue> p, Scope scope);
	shared_ptr<RValue> E4_(shared_ptr<RValue> p, Scope scope);
	shared_ptr<RValue> E5_(shared_ptr<RValue> p, Scope scope);
	int ArgList(Scope scope);
	int ArgList_(Scope scope);

	void DeclareStmt(Scope scope = GlobalScope);
	void DeclareStmt_(SymbolTable::TableRecord::RecordType p, string q, Scope scope);
	SymbolTable::TableRecord::RecordType Type(Scope scope);
	void InitVar(SymbolTable::TableRecord::RecordType p, string q, Scope scope);
	void DeclVarList_(SymbolTable::TableRecord::RecordType p, Scope scope);
	int ParamList(Scope scope);
	int ParamList_(Scope scope);
	void StmtList(Scope scope = GlobalScope);
	void Stmt(Scope scope);

	void AssignOrCallOp(Scope scope);
	void AssignOrCall(SymbolTable::TableRecord::RecordType type ,Scope scope);
	void AssignOrCall_(Scope scope, string p);
	void WhileOp(Scope scope);
	void DoOp(Scope scope);
	void ForOp(Scope scope);
	void ForInit(Scope scope);
	shared_ptr<RValue> ForExp(Scope scope);
	void ForLoop(Scope scope);
	void IfOp(Scope scope);
	void ElsePart(Scope scope);
	void SwitchOp(Scope scope);
	void Cases(shared_ptr<RValue> p, shared_ptr<LabelOperand> end, Scope scope);
	shared_ptr<LabelOperand> ACase(shared_ptr<RValue> p, shared_ptr<LabelOperand> end, Scope scope);
	void Cases_(shared_ptr<RValue> p, shared_ptr<LabelOperand> end, shared_ptr<LabelOperand> def, Scope scope);
	void IOp(Scope scope);
	void OOp(Scope scope);
	void OOp_(Scope scope);
	void GotoOp(Scope scope);//добавила


	void generateProlog(std::ostream & stream);
	void generateFunction(std::ostream & stream, std::string function);
	void generateCode(std::ostream & stream);
};

class MyError
{
protected:
	string _msg;
public:
	MyError(const string &mess);
};