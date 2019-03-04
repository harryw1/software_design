/*
	Harrison Weiss
	Software Design
	Dr. Miller
	SymTab.h

	SymTab.h: Symbol table storage and manipulation

*/
#pragma once

#include "stdafx.h"

// This class is our symbol table.
class SymbolTable
{

public:

  SymbolTable();
  
  ~SymbolTable();

  const static int m_MULTIPLY_DEFINED_SYMBOL = -999;
  const static int m_UNDEFINED_SYMBOL = -1000;

  // Add a new symbol to the symbol table.
  void AddSymbol(const string &a_symbol, const int &a_location);

  // Display the symbol table.
  void DisplaySymbolTable() const;

  // Lookup a symbol in the symbol table.
  bool LookupSymbol(const string &a_symbol, int &a_location) const;

private:
	map<string, int> m_symbol_table;
	int m_symbol_table_size;
};