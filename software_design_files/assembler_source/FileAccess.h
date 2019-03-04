/*
	Harrison Weiss
	Software Design
	Dr. Miller
	FileAccess.h

	FileAccess.h: Reading in files line by line
*/

#pragma once

#include "stdafx.h"

class FileAccess
{
public:
	
	FileAccess(int argc, char *argv[]);

	~FileAccess();

	bool GetNextLine(string &a_buffer);

	void Rewind();

private:

	ifstream m_source_file;

};