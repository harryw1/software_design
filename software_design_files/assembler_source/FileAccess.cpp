/*
	Harrison Weiss
	Software Design
	Dr. Miller
	FileAccess.cpp

	FileAccess.cpp: Implmentation of FileAccess.h; read in file, read line
	by line, rewind to the start of the file
*/

#include "stdafx.h"
#include "FileAccess.h"

FileAccess::FileAccess(int argc, char *argv[])
{
	if (argc != 2)
	{
		cerr << "Error: Assem <file_name>" << endl;
		exit(1);
	}

	m_source_file.open(argv[1]);

	if (!m_source_file)
	{
		cerr << "File could not be opened" << endl;
		exit(1);
	}
}

FileAccess::~FileAccess()
{
	m_source_file.close();
}

bool FileAccess::GetNextLine(string &a_buffer)
{
	// Returns false if we're at the end of the file
	if (m_source_file.eof())
	{
		return false;
	}

	getline(m_source_file, a_buffer);
	return true;
}

void FileAccess::Rewind()
{
	// Clear flags like eof
	m_source_file.clear();

	// Reset the file back to the start
	m_source_file.seekg(0, ios::beg);
}