/*
	Harrison Weiss
	Software Design
	Dr. Miller
	Emulator.h

	Emulator.h: Includes everything needed for the VC3600 computer
*/
#pragma once

#include "stdafx.h"

class Emulator
{

public:
	const static int m_MEMORY_SIZE = 10000; // The size of the memory of the VC3600.

	Emulator();

	bool InsertMemory(const int &a_location, const int &a_contents);

	bool RunProgram();

private:
	int m_memory[m_MEMORY_SIZE];
	int m_accumulator;

	enum MachineOpCode
	{
		ADD,
		SUB,
		MULT,
		DIV,
		LOAD,
		STORE,
		READ,
		WRITE,
		B,
		BM,
		BZ,
		BP,
		HALT
	};

	void Add(const int& a_location);
	void Subtract(const int& a_location);
	void Multiply(const int& a_location);
	void Divide(const int& a_location);
	void Load(const int& a_location);
	void Store(const int& a_location);
	void Read(const int& a_location);
	void Write(const int& a_location);
	int Branch(const int& a_location);
	bool Halt();
};