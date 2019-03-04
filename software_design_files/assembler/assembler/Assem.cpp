/*
	Harrison Weiss
	Software Design
	Dr. Miller
	Assem.cpp

	Assem.cpp: Main program that will run the assembler
*/

#include "stdafx.h"
#include "Assembler.h"
#include "Errors.h"
#include "Emulator.h"

int main(int argc, char *argv[])
{
    Assembler assem(argc, argv);

    // Establish the location of the labels
    assem.PassOne();

    // Display symbol table
	assem.DisplaySymbolTable();
	system("pause");

	// Output symbol table and translation
	assem.PassTwo();
	system("pause");

	// Run emulator
	assem.RunEmulator();
	system("pause");

	// Exited well
	return 0;
}