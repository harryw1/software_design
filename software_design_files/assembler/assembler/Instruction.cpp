/*
	Harrison Weiss
	Software Design
	Dr. Miller
	Instruction.cpp

	Instruction.cpp: Implementation of functions from the Instruction.h
	header file
*/

#include "stdafx.h"
#include "Instruction.h"
#include "Errors.h"

Instruction::Instruction() { ClearInfo(); }

Instruction::~Instruction() { ClearInfo(); }

void Instruction::ClearInfo()
{
	// Default values are set to empty strings
	m_Label = "";
	m_Op_Code = "";
	m_Operand = "";
	m_Instruction = "";

	// OP Code Num initially set to 0
	m_Op_Code_Num = 0;

	// Operand Value also set to 0
	m_Operand_Value = 0;

	// Assume not numeric
	m_Is_Numeric_Operand = false;

	// Comment is the default
	m_Type = COMMENT;
}

void Instruction::RemoveComment(string & a_Comment)
{
	auto comment_start = a_Comment.find(';');

	if (comment_start != string::npos)
	{
		a_Comment = a_Comment.substr(0, comment_start);
		return;
	}
}

bool Instruction::ValidLabel(const string & a_Test_Label)
{
	// Empty strings can be labels
	if (a_Test_Label.empty())
	{
		return true;
	}
	
	bool valid = true;

	// First letter must be a character
	if (!isalpha(a_Test_Label[0]))
	{
		Errors::RecordError(a_Test_Label + ": Invalid syntax. Labels must start with a letter.");
		
		valid = false;
	}

	// Labels cannot have a length greater than 10
	if (a_Test_Label.length() > 10)
	{
		Errors::RecordError(a_Test_Label + ": Invalid length. Label longer than 10 characters.");

		valid = false;
	}

	// Labels must be alphanumeric
	for (auto i = a_Test_Label.begin(); i != a_Test_Label.end(); i++)
	{
		if (!isalnum(*i))
		{
			Errors::RecordError(a_Test_Label + ": Invalid syntax. Label contains invalid character: " + *i + ".");
			
			valid = false;
		}
	}

	return valid;
}

bool Instruction::IsAssembly(const string & a_Test_Instruction)
{
	auto it = m_Assem_Code.find(a_Test_Instruction);

	// If the test instcution is in the hash map, set the type
	if (it != m_Assem_Code.end()) 
	{
		m_Op_Code = it->first;
		m_Op_Code_Num = 0;

		// If the instruction is to end the program, end
		if (it->second == END_PROGRAM) 
		{
			m_Type = END;
		}
		else 
		{
			m_Type = ASSEMBLY;
		}
		return true;
	}
	return false;
}

bool Instruction::IsMachine(const string & a_Test_Instruction)
{
	auto it = m_VC_Code.find(a_Test_Instruction);

	// If if the code is from the machine part of the hash map, set everthing to machine and return, otherwise false
	if (it != m_VC_Code.end()) 
	{
		m_Op_Code = it->first;
		m_Op_Code_Num = it->second;
		m_Type = MACHINE;
		return true;
	}
	return false;
}

void Instruction::VerifyAssembly()
{
	if (!m_Op_Code.empty()) {
		string op_Code = m_Op_Code;

		// Convert to upper
		transform(op_Code.begin(), op_Code.end(), op_Code.begin(), ::toupper);

		// Compare to values in hash map and report any errors
		auto i = m_Assem_Code.find(op_Code);
		if (i != m_Assem_Code.end()) {
			switch (i->second) {
			case ORG:
				if (isLabel()) {
					// ORG instruction cannot have a label
					Errors::RecordError("ORG cannot have a label");
				}
				break;
			case DS:
			case DC:
				if (!isLabel()) {
					// Same for DS and DC: need a label
					Errors::RecordError("DS and DC must have a label");
				}
				break;
			default:
				if (isLabel() || IsOperand()) {
					// END instruction cannot have a label or operand.
					Errors::RecordError("END cannot have a label or operand");
				}
				return;
			}

			// Check if there is an operand
			if (!IsOperand()) {
				Errors::RecordError("Missing operand on " + m_Op_Code + " instruction.");
			}

			// If there is an operand, its numeric and exceeds the max value of an operand
			if ((IsOperand()) && (m_Is_Numeric_Operand) &&
				(m_Operand_Value > m_MAX_OPERAND)) {
				Errors::RecordError("Operand exceeds max size of " + to_string(m_MAX_OPERAND));
			}
		}
		else {
			// If we get here, there is no op code
			Errors::RecordError("No OP Code. Cannot proceed.");
			Errors::DisplayErrors();
			system("pause");
			exit(1);
		}
	}
}

void Instruction::VerifyMachine()
{
	if (!m_Op_Code.empty()) {
		string op_Code = m_Op_Code;

		// Convert to uppercase
		transform(op_Code.begin(), op_Code.end(), op_Code.begin(), ::toupper);

		// Finds machine code in the hash map and comapres to ensure
		// that there are no errors
		auto i = m_VC_Code.find(op_Code);
		if (i != m_VC_Code.end()) {
			if (i->second == HAL) {
				if (IsOperand()) {
					Errors::RecordError("Invalid syntax. Cannot call HALT.");
				}
				return;
			}
			else {
				if (!IsOperand()) {
					Errors::RecordError("Operand required after " + m_Operand);
				}
			}
		}
	}
	else {
		// If we get here, there is no op code
		Errors::RecordError("No OP Code. Cannot proceed.");
		Errors::DisplayErrors();
		system("pause");
		exit(1);
	}
}

bool Instruction::NumericOperand(const string & a_Test_Operand)
{
	return ((!a_Test_Operand.empty()) && (find_if(a_Test_Operand.begin(), a_Test_Operand.end(), [](char c) { return !isdigit(c); }) == a_Test_Operand.end()));
}

Instruction::InstructionType Instruction::ParseInstruction(const string & a_buffer)
{
	// Clear info before starting the next parse
	ClearInfo();

	// Empty lines and lines starting with ; are comments
	if (a_buffer.empty() || a_buffer.find_first_of(';') == 0) 
	{
		return m_Type; // default type is a comment, so we can just return
	}

	m_Instruction = a_buffer;
	string temp_buffer = a_buffer;

	// Remove the comment if there is one. We use a temporary buffer because we
	// have to preserve the original instruction.
	RemoveComment(temp_buffer);

	// Read each segment into a string stream and put it into a vector of the
	// three separate parts of an instruction: label, opcode, operand.

	istringstream iss(temp_buffer);
	vector<string> instruction_parts;
	while (iss >> temp_buffer) 
	{
		if (!temp_buffer.empty()) 
		{
			instruction_parts.push_back(temp_buffer);
		}
	}

	// see if there are more than 3 arguments
	if (instruction_parts.size() > 3) 
	{
		Errors::RecordError("Too many arguments");
	}
	bool is_assembly = false;
	bool is_machine = false;
	bool valid_label = false;

	// Each instruction part gets assigned column by column
	for (int column = 0; column < instruction_parts.size(); column++) 
	{
		string temp_part = instruction_parts[column];

		// If the part is in the first column and there are 3 parts, the first part is a label
		if (column == 0 && instruction_parts.size() == 3) 
		{
			m_Label = instruction_parts[column];
			valid_label = ValidLabel(m_Label);
			continue;
		}

		// Convert the part to all uppercase
		transform(temp_part.begin(), temp_part.end(), temp_part.begin(), ::toupper);

		// If the part is machine language, handle it accordingly
		if (is_machine) {
			m_Operand = instruction_parts[column];

			// For machine language, the label is the operand.
			valid_label = ValidLabel(m_Operand);

			// Machine language instructions are not allowed to have numeric
			// operands.
			m_Is_Numeric_Operand = NumericOperand(m_Operand);
			if (m_Is_Numeric_Operand) 
			{
				Errors::RecordError("Error: machine language instruction cannot contain numeric operand: " + m_Operand);
			}
			continue;
		}

		// If the part is assembly, handle it accordingly
		if (is_assembly) {
			m_Operand = instruction_parts[column];

			// Assembly language instructions can only have numeric operands
			m_Is_Numeric_Operand = NumericOperand(m_Operand);
			if (!m_Is_Numeric_Operand) 
			{
				Errors::RecordError("Error: machine language instruction cannot contain non-numeric operand : " + m_Operand);
			}
			else 
			{
				m_Operand_Value = stoi(instruction_parts[column]);
			}
			continue;
		}

		// Verifying if an instruction is machine or assembly.
		is_machine = IsMachine(temp_part);
		is_assembly = IsAssembly(temp_part);
		if (is_machine) 
		{
			continue;
		}
		if (is_assembly) 
		{
			continue;
		}
	}

	// Check if machine/assembly syntax is correct.
	if (is_machine) 
	{
		VerifyMachine();
	}
	else if (is_assembly) 
	{
		VerifyAssembly();
	}
	else 
	{
		Errors::RecordError("Invalid syntax. No instruction found.");
		m_Op_Code = m_MISS_OPCODE;
		m_Operand = m_UNK_OPERAND;
	}
	return m_Type;
}

int Instruction::LocationNextInstruction(const int & a_loc)
{
	// If were in assembly language, need to check for org or ds
	if (m_Type == ASSEMBLY && IsOperand()) {
		if (m_Assem_Code.at(m_Op_Code) == ORG) {
			if (m_Is_Numeric_Operand) {
				return m_Operand_Value;
			}
		}
		else if (m_Assem_Code.at(m_Op_Code) == DS) {
			if (m_Is_Numeric_Operand) {
				return a_loc + m_Operand_Value;
			}
		}
	}
	return a_loc + 1;
}
