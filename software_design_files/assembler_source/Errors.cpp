/*
	Harrison Weiss
	Software Design
	Dr. Miller
	Errors.cpp

	Errors.cpp: Implementation of function in Errors.h; records and displays errors
*/

#include "stdafx.h"
#include "Errors.h"

int Errors::m_total_messages;
vector<string> Errors::m_ErrorMsgs;

void Errors::InitErrorReporting()
{
	m_total_messages = 0;

	if (m_ErrorMsgs.empty())
	{
		return;
	}

	m_ErrorMsgs.clear();
}

void Errors::RecordError(const string& a_error_message)
{
	if (a_error_message.empty())
	{
		return;
	}
	m_ErrorMsgs.push_back(a_error_message);
	m_total_messages++;
}

void Errors::DisplayErrors()
{
	if (!m_total_messages)
	{
		cout << "Errors: none." << endl;
	}

	if (m_total_messages > 0)
	{
		cout << "Errors: " << endl;
		for (auto error : m_ErrorMsgs)
		{
			cout << error << endl;
		}
	}
}