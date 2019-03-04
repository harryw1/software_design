/*
	Harrison Weiss
	Software Design
	Dr. Miller
	SymTab.cpp

	SymTab.cpp: Implementation for functions in the SymTab.h file; lookup in table,
	display the table, and add to it
*/

#include "stdafx.h"
#include "SymTab.h"

SymbolTable::SymbolTable() : m_symbol_table_size(0){}

SymbolTable::~SymbolTable()
{
	m_symbol_table_size = 0;
	m_symbol_table.clear();
}

void SymbolTable::AddSymbol(const string &a_symbol, const int &a_location)
{
	auto table_entry = m_symbol_table.find(a_symbol);

	if (table_entry != m_symbol_table.end() && m_symbol_table_size)
	{
		table_entry->second = m_MULTIPLY_DEFINED_SYMBOL;
		return;
	}

	m_symbol_table[a_symbol] = a_location;
	m_symbol_table_size++;
}

void SymbolTable::DisplaySymbolTable() const
{
	int entry_number = 0;
	cout << "Symbol #" << setw(13) << "Symbol" << setw(15) << "Location" << endl;
	for (auto table_entry : m_symbol_table)
	{
		cout << entry_number << setw(17) << table_entry.first << setw(14) << table_entry.second << endl;
		entry_number++;
	}
}

bool SymbolTable::LookupSymbol(const string &a_symbol, int &a_location) const
{
	auto table_entry = m_symbol_table.find(a_symbol);

	if (table_entry != m_symbol_table.end())
	{
		a_location = table_entry->second;
		return true;
	}

	a_location = m_UNDEFINED_SYMBOL;
	return false;
}