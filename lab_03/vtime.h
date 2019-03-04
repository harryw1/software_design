#pragma once
#include <string>
using namespace std;

class VTime
{
public:

	// Constructor for this class.  We allow the user to initially set the stime or it will be set to zero.
	VTime(int a_hours = 0, int a_minutes = 0, int a_seconds = 0)
	{
		m_hours = a_hours;
		m_minutes = a_minutes;
		m_seconds = a_seconds;
	}
	// Accessor functions.
	int getHours()
	{
		return m_hours;
	}
	int getMinutes()
	{
		return m_minutes;
	}
	int getSeconds()
	{
		return m_seconds;
	}
	void setHours(int a_hours)
	{
		m_hours = a_hours;
	}
	void setMinutes(int a_minutes)
	{
		m_minutes = a_minutes;
	}
	void setSeconds(int a_seconds)
	{
		m_seconds = a_seconds;
	}
	// A function to add a specified number of seconds to the time.
	void addSeconds(int a_seconds);

	// Plus operator to add seconds onto the time.
	VTime operator +(int a_seconds);

	// Gets the time as an ASCII string.
	string getASCIITime();

private:

	int m_hours, m_minutes, m_seconds;

};