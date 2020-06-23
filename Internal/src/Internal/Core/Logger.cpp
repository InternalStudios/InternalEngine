/*
    Author: Shaun Fulham
    Copyright: Internal Media Group
    Licence: IMG Public Licence
*/

#include "Logger.h"

#include <iostream>

namespace Internal
{
	Logger::Logger(const char* name)
		: m_Name(name)
	{

	}

	void Logger::Info(const char* message)
	{
		std::cout << "Info: [" << m_Name << "] " << message << '\n';
	}

	void Logger::Warn(const char* message)
	{
		std::cout << "Warn: [" << m_Name << "] " << message << '\n';
	}

	void Logger::Error(const char* message)
	{
		std::cout << "Error: [" << m_Name << "] " << message << '\n';
	}

	#ifdef INTERNAL_DEBUG
	void Logger::Assert(bool assertion, const char* message)
	{
		if(assertion)
		{
			std::cout << "Assert: [" << m_Name << "] " << message << '\n';
		}
	}
	#endif
}