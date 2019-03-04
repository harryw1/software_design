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

	// Assume that things aren't numeric at first
	m_Is_Numeric_Operand = false;

	// Default type of instruction is a comment
	m_Type = ST_Comment;
}

void Instruction::RemoveComment(string & a_Comment)
{
	auto comment_start = a_Comment.find(';');

	// If we find a comment, we can remove it
	if (comment_start != string::npos)
	{
		a_Comment = a_Comment.substr(0, comment_start);
		return;
	}
}

bool Instruction::ValidLabel(const string & a_Test_Label)
{
	// Empty strings are valid labels
	if (a_Test_Label.empty())
	{
		return true;
	}
	
	bool valid = true;	// From here, we decide if we have a valid label

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
	// i will be used to compare through our has map below
	auto i = m_Assem_Code.find(a_Test_Instruction);

	if (i != m_Assem_Code.end())
	{
		m_Op_Code = i->first;
		m_Op_Code_Num = 0;

		// We also need to check to make sure it isn't
		// and end program instruction
		if (i->second == END_PROGRAM)
		{
			m_Type = ST_End;
		}
		else
		{
			m_Type = ST_AssemblerInstr;
		}
		return true;
	}
	return false;
}

bool Instruction::IsMachine(const string & a_Test_Instruction)
{
	auto i = m_VC_Code.find(a_Test_Instruction);

	// Searches the VC Code hash map and if the instruction
	// is found there, set the corresponding values and return
	// true.
	if (i != m_VC_Code.end())
	{
		m_Op_Code = i->first;
		m_Op_Code_Num = i->second;
		m_Type = ST_MachineLanguage;
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

Instruction::InstructionType Instruction::ParseInstruction(const string & a_buff)
{
	// Clears information before each parsing
	ClearInfo();

	// If the line is empty, return as a comment. If the line
	// starts with ';', it is also a comment.
	if (a_buff.empty() || a_buff.find_first_of(';') == 0)
	{
		return m_Type;	// Our default type is a comment
	}

	// Need to preserve original instruction
	m_Instruction = a_buff;
	string temp_buff = a_buff;

	// Leaves original as is
	RemoveComment(temp_buff);

	// Parses the line into a string stream so we can manipulate
	// that further into a vector of three items: label, opcode, operand
	istringstream iss(temp_buff);
	vector<string> line_parts;
	while (iss >> temp_buff)
	{
		if (!temp_buff.empty())
		{
			line_parts.push_back(temp_buff);
		}
	}
	// checks to see if we only have three arguments
	if (line_parts.size() > 3)
	{
		Errors::RecordError("Too many arguments.");
	}
	bool is_assembly = false;
	bool is_machine = false;
	bool valid_label = false;

	// Each instruction part is assigned to member variables
	// we're going to be going column by column
	for (int col = 0; col < line_parts.size(); col++)
	{
		string temp_part = line_parts[col];
		// If there are three columns, the first is a label
		if (col == 0 && line_parts.size() == 3)
		{
			m_Label = line_parts[col];
			valid_label = ValidLabel(m_Label);
			continue;
		}
		// Covert to uppercase
		transform(temp_part.begin(), temp_part.end(), temp_part.begin(), ::toupper);

		// If instruction is machine language
		if (is_machine)
		{
			m_Operand = line_parts[col];

			// In machine language, the label is the operand
			valid_label = ValidLabel(m_Operand);

			// Cannot contain numeric operand
			m_Is_Numeric_Operand = NumericOperand(m_Operand);
			if (m_Is_Numeric_Operand)
			{
				Errors::RecordError("Error: machine language instruction cannot contain numeric operand: " + m_Operand);
			}
			continue;
		}

		// If instruction is assembly
		if (is_assembly) {
			m_Operand = line_parts[col];

			// Cannot contain numeric operand
			m_Is_Numeric_Operand = NumericOperand(m_Operand);
			if (!m_Is_Numeric_Operand) {
				Errors::RecordError("Assembly language instrcution contains numeric oeprand: " + m_Operand);
			}
			else {
				m_Operand_Value = stoi(line_parts[col]);
			}
			continue;
		}

		// Check to see if machine or assembly
		is_machine = IsMachine(temp_part);
		is_assembly = IsAssembly(temp_part);
		if (is_machine) {
			continue;
		}
		if (is_assembly) {
			continue;
		}
	}

	// Syntax check
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
		Errors::RecordError("Invalid syntax. No instructions.");
		m_Op_Code = m_MISS_OPCODE;
		m_Operand = m_UNK_OPERAND;
	}
	return m_Type;
}

int Instruction::LocationNextInstruction(const int & a_loc)
{
	// If were in assembly language, need to check for org or ds
	if (m_Type == ST_AssemblerInstr && IsOperand()) {
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
