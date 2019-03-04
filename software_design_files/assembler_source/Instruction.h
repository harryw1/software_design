/*
	Harrison Weiss
	Software Design
	Dr. Miller
	Instruction.h

	Instruction.h: Parses code passed by user and translates
	accordingly
*/

#pragma once
class Instruction
{

public:
	Instruction();	// Constructor
	~Instruction();	// Destructor

	// Codes to indicate the type of instruction we are processing
	enum InstructionType
	{
		ST_MachineLanguage, // A machine language instruction.
		ST_AssemblerInstr,  // Assembler Language instruction.
		ST_Comment,         // Comment or blank line
		ST_End              // end instruction.
	};

	// Code to indicate VC3600 machine language instrcutions
	enum VCOpCode {
		ADD,	// ACC <-- c(ACC) + c(ADDR)
		SUB,	// ACC <-- c(ACC) - c(ADDR) 
		MUL,	// ACC <-- c(ACC) * c(ADDR) 
		DIV,	// ACC <-- c(ACC) / c(ADDR)
		LOA,	// ACC <-- c(ADDR)
		STO,	// ADDR <-- c(ACC)
		REA,	// A line is read and its first six digits are placed in the specified address
		WRI,	// c(ACC) is displayed
		B,		// go to ADDR for next instruction
		BM,		// go to ADDR if c(ACC) < 0
		BZ,		// go to ADDR if c(ACC) = 0
		BP,		// go to ADDR if c(ACC) > 0
		HAL		// terminate execution
	};

	// Code to indicate assembly language instructions
	enum AssemCode {
		ORG,		// Define starting point of instructions
		DC,			// Define a constant
		DS,			// Define storage
		END_PROGRAM	// No additional statements
	};

	// PARSE INSTRUCTION
	InstructionType ParseInstruction(const string &a_buff);

	// Compute the location of the next instruction.
	int LocationNextInstruction(const int &a_loc);

	// To access the label
	inline string &GetLabel() { return m_Label; }

	// To determine if a label is blank.
	inline bool isLabel() { return !m_Label.empty(); }

	// PARSE OPCODE
	// Get opcode
	inline string &GetOpCode() { return m_Op_Code; }

	// Get opcode num
	inline int OpCodeNum() const { return m_Op_Code_Num; }

	// PARSE OPERAND
	// Get operand
	inline string &Operand() { return m_Operand; }

	// Get operand value
	inline int OperandValue() const { return m_Operand_Value; }

	// Return true if operand value is numeric
	inline bool IsNumericOperand() const { return m_Is_Numeric_Operand; }

	// Return true if an operand exists
	inline bool IsOperand() const { return !m_Operand.empty(); }

	// Constants so that we can properly translate: we want to be sure the opcode is properly formatted
	const int m_MAX_OPERAND = 999999;	// Opcode can only hold six digits, so max is 999999
	const int m_MISS_OPCODE = 00;		// Missing opcode will be denoted by a 00 since no instruction holds that value
	const string m_UNK_OPERAND = "XXXX";	// If we get an unknown operand, it will be denoted by XXXX
	const string m_UNK_OPERATOR = "XX";		// Missing operator

private:
	// The elemements of a instruction
	string m_Label;   // The label.
	string m_Op_Code;  // The symbolic op code.
	string m_Operand; // The operand.
	string m_Instruction; // The original instruction.

	// Derived values.
	int m_Op_Code_Num;        // The numerical value of the op code.
	int m_Operand_Value;	// The value of the operand if it is numeric.
	bool m_Is_Numeric_Operand; // == true if the operand is numeric.
	InstructionType m_Type; // The type of instruction.

	// Checks to see what language were reading and whether its formatted properly
	bool IsAssembly(const string &a_Test_Instruction);
	bool IsMachine(const string &a_Test_Instruction);
	void VerifyAssembly();
	void VerifyMachine();

	// Additional checks for labels and operand
	bool ValidLabel(const string &a_Test_Label);
	bool NumericOperand(const string &a_Test_Operand);

	// Ignore comments
	void RemoveComment(string &a_Comment);

	// Clears out pervious information
	void ClearInfo();

	// Hash map which is used to translate parsed string into machine language
	const unordered_map<string, VCOpCode> m_VC_Code{
		{ "ADD", ADD },
		{ "SUB", SUB },
		{ "MUL", MUL },
		{ "DIV", DIV },
		{ "LOA", LOA },
		{ "STO", STO },
		{ "REA", REA },
		{ "WRI", WRI },
		{ "B", B },
		{ "BM", BM },
		{ "BZ", BZ },
		{ "BP", BP },
		{ "HAL", HAL },
	};

	// Hash map which is used to translate parsed string into assembly
	const unordered_map<string, AssemCode> m_Assem_Code{
		{ "ORG", ORG },
		{ "DC", DC },
		{ "DS", DS },
		{ "END", END_PROGRAM }
	};
};
