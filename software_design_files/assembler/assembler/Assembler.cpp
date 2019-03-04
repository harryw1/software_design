/*
	Harrison Weiss
	Dr. Miller
	Software Design
	Assembler.cpp

	Assembler.cpp: provides implementation for function delcared in Assembler.h
*/

#include "stdafx.h"
#include "Assembler.h"
#include "Errors.h"
#include "SymTab.h"

Assembler::Assembler(int argc, char* argv[]) : m_file_access(argc, argv) { }

void Assembler::PassOne() {
	// Tracks location of the instructions to be generated.
	int location = 0;

	// Process each line of source code.
	while (true) {
		string buffer;

		// If there are no more lines, an end statement is missing. This error
		// will be reported in pass 2.
		if (!m_file_access.GetNextLine(buffer)) {
			return;
		}

		// Parse the instruction and get the instruction type.
		Instruction::InstructionType type = m_instruction.ParseInstruction(buffer);

		// If this is an end statement, there is nothing left to do in pass one.
		// Pass two will determine if the end is the last statement.
		if (type == Instruction::END) {
			return;
		}

		// Labels can only be on machine language and assembly language type
		// instruction. Skip all other types.
		if (type != Instruction::MACHINE &&
			type != Instruction::ASSEMBLY) {
			continue;
		}

		// If the instruction has a label, record the label and its location in the
		// symbol table.
		if (m_instruction.isLabel()) {
			m_symbol_table.AddSymbol(m_instruction.GetLabel(), location);
		}

		// Compute the location of the next instruction.
		location = m_instruction.LocationNextInstruction(location);
	}
}

void Assembler::PassTwo() {
	
	// Start recording errors
	Errors::InitErrorReporting();

	// Back to the beginning of the file
	m_file_access.Rewind();

	int location = 0;
	int operand_location = 0;
	cout << "~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "Translation:" << endl;
	cout << "Location\tContents\tOriginal Instruction" << endl;

	// Process the source code
	while (true) {
		string buffer;

		// If we get to the end of the file, there was no end statement
		if (!m_file_access.GetNextLine(buffer)) 
		{
			Errors::RecordError("Fatal error: missing end statement");
			Errors::DisplayErrors();
			return;
		}

		// Get the type of instruction
		Instruction::InstructionType type = m_instruction.ParseInstruction(buffer);

		if (type == Instruction::END) 
		{
			// There shouldnt be anything after the end statement
			if (m_file_access.GetNextLine(buffer)) {
				Errors::RecordError("Statement after END statement");
				Errors::DisplayErrors();
			}
			return;
		}

		// If the type isnt machine or assembly, its a comment, so print
		if (type != Instruction::MACHINE &&
			type != Instruction::ASSEMBLY) {
			PrintTranslation(location, operand_location, m_instruction.OpCodeNum(),
				buffer, type);
			continue;
		}
		operand_location = 0;

		// Checking for an alphanumeric operand in the symbol table
		if (m_instruction.IsOperand() && !m_instruction.IsNumericOperand() &&
			!m_symbol_table.LookupSymbol(m_instruction.Operand(),
				operand_location)) {
			if (operand_location == SymbolTable::m_MULTIPLY_DEFINED_SYMBOL) {
				Errors::RecordError("Symbol " + m_instruction.Operand() +
					" is multiply defined.");
				Errors::DisplayErrors();
			}
			else {
				Errors::RecordError("Symbol " + m_instruction.Operand() +
					" is undefined.");
				m_symbol_table.AddSymbol(m_instruction.Operand(),
					m_symbol_table.m_UNDEFINED_SYMBOL);
				Errors::DisplayErrors();
			}
		}

		// If the location is out of memory, report
		if (location > Emulator::m_MEMORY_SIZE) {
			Errors::RecordError("Location " + to_string(location) +
				" is out of memory.");
			Errors::DisplayErrors();
			return;
		}

		// Print the finished translation
		PrintTranslation(location, operand_location, m_instruction.OpCodeNum(), buffer, type);

		// Get the location of the next instruction
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

	// If the thing is a comment or an end, print
	if (a_type == Instruction::COMMENT ||
		a_type == Instruction::END) 
	{
		cout << "\t\t\t" << a_instruction << endl;
		return;
	}

	// Different things for assembly than assembly/machine
	if (a_type == Instruction::ASSEMBLY) {
		if (m_instruction.OpCodeNum() == Instruction::DC) 
		{
			cout << "  " << a_instruction_location << "\t\t\t" << a_instruction << endl;
			if (m_instruction.OpCodeNum() == Instruction::ORG) {
				ss.str(string());
				ss << setfill('0') << setw(6) << m_instruction.OperandValue();
				contents = ss.str();
				instruction_information.m_instruction_contents = stoi(contents);
				instruction_information.m_instruction_location = a_instruction_location;
				m_instruction_information.push_back(instruction_information);
			}
			return;
		}
		location = m_instruction.OperandValue();
	}

	// Either assembly or machine
	ss.str(string());
	ss << setfill('0') << setw(2) << a_op_code_number << setw(4) << location;
	contents = ss.str();
	instruction_information.m_instruction_contents = stoi(contents);
	instruction_information.m_instruction_location = a_instruction_location;
	cout << "  " << a_instruction_location << "\t\t" << contents << "\t\t" << a_instruction << endl;
	m_instruction_information.push_back(instruction_information);
}

void Assembler::RunEmulator() {
	for (auto it = m_instruction_information.begin();
		it != m_instruction_information.end(); it++) {
		// Need to make sure there is still enough memory for the program to run
		if (m_emulator.InsertMemory(it->m_instruction_location, it->m_instruction_contents)) {
			continue;
		}
		else {
			Errors::RecordError("Fatal error: out of memory");
			Errors::DisplayErrors();
			system("pause");
			exit(1);
		}
	}

	if (m_emulator.RunProgram()) {
		cout << "Emulation successful." << endl;
	}
	else {
		Errors::RecordError("Fatal error: unable to emulate");
		Errors::DisplayErrors();
		system("pause");
		exit(1);
	}
}