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
		std::cout << "\033[38;2;" << 0xF5 << ";" << 0xA9 << ";" << 0xB8 << "m";
	}

	void Logger::Output(const char* message)
	{
		std::cout << message << '\n';
	}

	void Logger::Info(const char* message)
	{
		std::cout << "\033[38;2;" << 0xF5 << ";" << 0xA9 << ";" << 0xB8 << "m" << "Info: [" << m_Name << "] " << message << '\n';
	}

	void Logger::Warn(const char* message)
	{
		std::cout << "\033[38;2;" << 0xFF << ";" << 0xFF << ";" << 0x00 << "m" << "Warn: [" << m_Name << "] " << message << '\n';
	}

	void Logger::Error(const char* message)
	{
		std::cout << "\033[38;2;" << 0xFF << ";" << 0x00 << ";" << 0x00 << "m" << "Error: [" << m_Name << "] " << message << '\n';
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