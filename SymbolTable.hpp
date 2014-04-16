#ifndef _SYMBOLTABLE_HPP
#define _SYMBOLTABLE_HPP

#include <map>
#include <iostream>
#include <string>

struct Symbol;
class Expr;
class SymbolTable;

struct Symbol {
	const std::string name;
	Expr *value;
	Symbol(const std::string& name, Expr *value) : name(name), value(value) {};
};

class SymbolTable {
  std::map<std::string,Expr *> *table;
  SymbolTable *prev;

public:
  SymbolTable(SymbolTable *p = 0) : prev(p) { table = new std::map<std::string, Expr *>();};
  void put(const std::string& name, Expr *value ) {
	  (*table)[name] = value;
  }
  Expr *get(const std::string& name) {
	SymbolTable *symtab = this;
	while (symtab) {
		auto iter = symtab->table->find(name);
		if (iter != symtab->table->end()){
			return iter->second;
		}
		symtab = symtab->prev;
	}
		return 0;
	}
  SymbolTable *parent() {
	  return prev;
  }
};

#endif
