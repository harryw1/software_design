/*
	Harrison Weiss
	Software Design
	Dr. Miller
	Assembler.h

	Assembler.h: Establishes the basic parts of the assembler; the first and second pass, the symbol table display,
	and the emulator call
*/
#pragma once

#include "stdafx.h"
#include "SymTab.h"
#include "Instruction.h"
#include "FileAccess.h"
#include "Emulator.h"
#include "Errors.h"

class Assembler
{

  public:
    Assembler(int argc, char *argv[]);
    // ~Assembler();

    // Pass I - establish the locations of the symbols
    void PassOne();

    // Pass II - generate a translation
    void PassTwo();

    // Display the symbols in the symbol table.
    void DisplaySymbolTable() { m_symbol_table.DisplaySymbolTable(); }

    // Run emulator on the translation.
    void RunEmulator();

  private:
    FileAccess m_file_access;    // File Access object
    SymbolTable m_symbol_table; // Symbol table object
    Instruction m_instruction;   // Instruction object
    Emulator m_emulator;      // Emulator object

	struct InstructionInfo {
		int m_instruction_location;
		int m_instruction_contents;
	};

	vector<InstructionInfo> m_instruction_information;

	void PrintTranslation(
		const int& a_instruction_location,
		const int& a_operand_location,
		const int& a_op_code_number,
		const string& a_instruction,
		const Instruction::InstructionType& a_type);
};
