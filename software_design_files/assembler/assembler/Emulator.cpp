/*
	Harrison Weiss
	Software Design
	Dr. Miller
	Emulator.cpp

	Emulator.cpp: Implementation of Emulator.h functions; defines the functionality
	of the VC3600 computer
*/

#include "stdafx.h"
#include "Emulator.h"
#include "Errors.h"

Emulator::Emulator() : m_accumulator(0)
{
	memset(m_memory, 0, m_MEMORY_SIZE * sizeof(int));
}

bool Emulator::InsertMemory(const int &a_location, const int &a_contents)
{
	if (a_location > m_MEMORY_SIZE)
	{
		return false;
	}

	m_memory[a_location] = a_contents;
	return true;
}

bool Emulator::RunProgram()
{
	int current_location = 0;
	current_location = m_memory[current_location];

	while (current_location < m_MEMORY_SIZE)
	{
		string contents = to_string(m_memory[current_location]);
		string op_code = "";
		string location = "";
		int iterator = 0;

		if (stoi(contents) > 99999)
		{
			while (iterator < 2)
			{
				op_code = op_code + contents[iterator];
				iterator++;
			}
			while (iterator < contents.size())
			{
				location = location + contents[iterator];
				iterator++;
			}
		}
		else
		{
			while (iterator < 1)
			{
				op_code = op_code + contents[iterator];
				iterator++;
			}
			while (iterator < contents.size())
			{
				location = location + contents[iterator];
				iterator++;
			}
		}

		int op_code_value = stoi(op_code);
		int location_int = stoi(location);

		switch (op_code_value) {
		case ADD:
			Add(location_int);
			break;
		case SUB:
			Subtract(location_int);
			break;
		case MULT:
			Multiply(location_int);
			break;
		case DIV:
			Divide(location_int);
			break;
		case LOAD:
			Load(location_int);
			break;
		case STORE:
			Store(location_int);
			break;
		case READ:
			Read(location_int);
			break;
		case WRITE:
			Write(location_int);
			break;
		case B:
			current_location = Branch(location_int);
			continue;
		case BM:
			if (m_accumulator < 0) {
				current_location = Branch(location_int);
				continue;
			}
			break;
		case BZ:
			if (m_accumulator == 0) {
				current_location = Branch(location_int);
				continue;
			}
			break;
		case BP:
			if (m_accumulator > 0) {
				current_location = Branch(location_int);
				continue;
			}
			break;
		case HALT:
			return Halt();
		default:
			Errors::RecordError("Unknown op code: terminating");
			Errors::DisplayErrors();
			return false;
		}

		current_location++;

	}
	
	// If we get to this point, no halt statement was found
	// and the emulation 'broke'
	return false;

}

void Emulator::Add(const int &a_location)
{
	m_accumulator += m_memory[a_location];
}

void Emulator::Subtract(const int& a_location) {
	m_accumulator -= m_memory[a_location];
}

void Emulator::Multiply(const int& a_location) {
	m_accumulator *= m_memory[a_location];
}

void Emulator::Divide(const int& a_location) {
	// Check for dividing by zero
	if (m_memory[a_location] == 0) {
		Errors::RecordError("Fatal error: zero division");
		Errors::DisplayErrors();
		system("pause");
		exit(1);
	}
	m_accumulator /= m_memory[a_location];
}

void Emulator::Load(const int& a_location) {
	m_accumulator = m_memory[a_location];
}

void Emulator::Store(const int& a_location) {
	m_memory[a_location] = m_accumulator;
}

void Emulator::Read(const int& a_location) {
	string input;
	bool is_alpha = true;

	// Checks to see if the value is numeric
	while (is_alpha) 
	{
		cout << "Reading: ";
		cin >> input;

		for (auto iterator = input.begin(); iterator != input.end(); iterator++) 
		{
			if (isdigit(*iterator)) 
			{
				is_alpha = false;
				continue;
			}
			else if (*iterator == '-' && iterator == input.begin()) 
			{
				continue;
			}
		}
		if (is_alpha) 
		{
			cout << "Input must be an integer" << endl;
		}
	}
	m_memory[a_location] = stoi(input);
}

void Emulator::Write(const int& a_location) 
{
	cout << m_memory[a_location] << endl;
}

int Emulator::Branch(const int& a_location) 
{
	return a_location;
}

bool Emulator::Halt() 
{
	// Returns true to indicate that we've reached a halt statement
	return true;
}