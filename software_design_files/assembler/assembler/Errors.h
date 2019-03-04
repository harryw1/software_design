/*
	Harrison Weiss
	Software Design
	Dr. Miller
	Errors.h

	Errors.h: Error reporting
*/
#ifndef _ERRORS_H
#define _ERRORS_H

#include "stdafx.h"

class Errors
{

public:
  // Initializes error reports.
  static void InitErrorReporting();

  // Records an error message.
  static void RecordError(const string& a_error_message);

  // Displays the collected error message.
  static void DisplayErrors();

private:
  static std::vector<std::string> m_ErrorMsgs;
  static int m_total_messages;
};
#endif