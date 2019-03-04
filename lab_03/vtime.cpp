//#include ".\VTime.h" // This works but is freaky!   I will leave it in so we can talk.
#include <iostream>
#include "VTime.h"

using namespace std;

// Adds a specified number of seconds onto the time.
void VTime::addSeconds(int a_seconds)
{
	m_seconds += a_seconds;
	if (m_seconds < 60) return;

	m_minutes += m_seconds / 60;
	m_seconds %= 60;

	if (m_minutes < 60) return;

	m_hours += m_minutes / 60;
	m_minutes %= 60;

	m_hours %= 24;
}
VTime VTime::operator +(int a_seconds)
{
	VTime tmp(m_hours, m_minutes, m_seconds);
	tmp.addSeconds(a_seconds);
	return tmp;
}
// Gets the time as an ASCII string
string VTime::getASCIITime()
{
	char buff[128];
	sprintf(buff, "%2.2d:%2.2d:%2.2d", m_hours, m_minutes, m_seconds);

	return buff;
}