/*
	Harrison Weiss
	Software Design
	Dr. Miller
	Assembler.cpp

	Assembler.cpp: Implementation of the functions in Assembler.h
*/

#include "stdafx.h"
#include "Assembler.h"
#include "Errors.h"
#include "SymTab.h"

Assembler::Assembler(int argc, char *argv[]) : m_file_access(argc, argv){}

// Pass I establishes the location of the labels.  You will write better function comments according to the coding standards.
void Assembler::PassOne()
{
    int location = 0; // Tracks the location of the instructions to be generated.

    // Successively process each line of source code.
    for (;;)
    {
		string buffer;

		// Reads until we hit an end statement, pass two will
		// check to see if an end statement exists
		if (!m_file_access.GetNextLine(buffer))
		{
			return;
		}

		// Get instruction type
		Instruction::InstructionType type = m_instruction.ParseInstruction(buffer);

		// If we read an end statement, the program is done for pass one
		if (type == Instruction::ST_End)
		{
			return;
		}

		// Labels should be in machine and assembly langs
		if (type != Instruction::ST_MachineLanguage &&
			type != Instruction::ST_AssemblerInstr)
		{
			continue;
		}

		// If instruction has label, record the label and its location
		if (m_instruction.isLabel())
		{
			m_symbol_table.AddSymbol(m_instruction.GetLabel(), location);
		}

		// Get the next instruction location
		location = m_instruction.LocationNextInstruction(location);
    }
}

void Assembler::PassTwo()
{
	// Error reporting
	Errors::InitErrorReporting();

	// Go back to the start of the file
	m_file_access.Rewind();

	// Iterators for instruction exectution
	int location = 0;
	int operand_location = 0;
	cout << "~~~~~~~~~~~~" << endl;
	cout << "Translation:" << endl;
	cout << "Location\tContents\tOriginal Instruction" << endl;

	// Process code

	for (;;)
	{
		string buffer;

		// If there are no more lines, there must be and end statement
		if (!m_file_access.GetNextLine(buffer))
		{
			Errors::RecordError("Fatal error: missing end statement");
			Errors::DisplayErrors();
			return;
		}

		Instruction::InstructionType type = m_instruction.ParseInstruction(buffer);

		if (type == Instruction::ST_End)
		{
			if (m_file_access.GetNextLine(buffer))
			{
				Errors::RecordError("Statement after 'END' statement");
				Errors::DisplayErrors();
			}
			return;
		}

		// If the instruction is comment, print
		if (type != Instruction::ST_MachineLanguage &&
			type != Instruction::ST_AssemblerInstr)
		{
			PrintTranslation(location, operand_location, m_instruction.OpCodeNum(),
				buffer, type);
			continue;
		}
		operand_location = 0;

		// Check for operand in symbol table
		if (m_instruction.IsOperand() && !m_instruction.IsNumericOperand() &&
			!m_symbol_table.LookupSymbol(m_instruction.Operand(), operand_location))
		{
			if (operand_location == SymbolTable::m_MULTIPLY_DEFINED_SYMBOL)
			{
				Errors::RecordError("Symbol " + m_instruction.Operand() + " is multiply defined");
				Errors::DisplayErrors();
			}
			else
			{
				Errors::RecordError("Symbol " + m_instruction.Operand() + " is undefined");
				m_symbol_table.AddSymbol(m_instruction.Operand(), m_symbol_table.m_UNDEFINED_SYMBOL);
				Errors::DisplayErrors();
			}
		}

		// If location is out of bounds, report
		if (location > Emulator::m_MEMORY_SIZE)
		{
			Errors::RecordError("Location " + to_string(location) + " is out of memory");
			Errors::DisplayErrors();
			return;
		}

		PrintTranslation(location, operand_location, m_instruction.OpCodeNum(), buffer, type);

		location = m_instruction.LocationNextInstruction(location);
	}
}

void Assembler::PrintTranslation(const int& a_instruction_location,
	const int& a_operand_location,
	const int& a_op_code_number,
	const string& a_instruction,
	const Instruction::InstructionType& a_type)
{
	string contents = "";
	InstructionInfo instruction_information;
	stringstream ss;
	int location = a_operand_location;

	// If comment or end, just print
	if (a_type == Instruction::ST_Comment ||
		a_type == Instruction::ST_End)
	{
		cout << "\t\t\t" << a_instruction << endl;
		return;
	}

	// Assembly lang handling
	if (a_type == Instruction::ST_AssemblerInstr)
	{
		if (m_instruction.OpCodeNum() == Instruction::ORG ||
			m_instruction.OpCodeNum() == Instruction::DC)
		{
			cout << " " << a_instruction_location << "\t\t\t" << a_instruction << endl;
			if (m_instruction.OpCodeNum() == Instruction::ORG)
			{
				ss.str(string());
				ss << setfill('0') << setw(5) << m_instruction.OperandValue();
				contents = ss.str();
				instruction_information.m_instruction_contents = stoi(contents);
				instruction_information.m_instruction_location = a_instruction_location;
				m_instruction_information.push_back(instruction_information);
			}
			return;
		}
		location = m_instruction.OperandValue();
	}

	// Handle else
	ss.str(string());
	ss << setfill('0') << setw(2) << a_op_code_number << setw(4) << location;
	contents = ss.str();
	instruction_information.m_instruction_contents = stoi(contents);
	instruction_information.m_instruction_location = a_instruction_location;
	cout << "  " << a_instruction_location << "\t\t" << contents << "\t\t" << a_instruction << endl;
	m_instruction_information.push_back(instruction_information);
}

void Assembler::RunEmulator()
{
	for (auto iterator = m_instruction_information.begin();
		iterator != m_instruction_information.end(); iterator++)
	{
		if (m_emulator.RunProgram())
		{
			cout << "Emulation complete" << endl;
		}
		else
		{
			Errors::RecordError("Fatal error: unable to emulate");
			Errors::DisplayErrors();
			system("pause");
			exit(1);
		}
	}
}
