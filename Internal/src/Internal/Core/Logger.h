/*
    Author: Shaun Fulham
    Copyright: Internal Media Group
    Licence: IMG Public Licence
*/

#pragma once

namespace Internal
{
	class Logger
	{
	public:
		Logger(const char* name);
		void Info(const char* message);
		void Warn(const char* message);
		void Error(const char* message);
		#ifdef INTERNAL_DEBUG
		void Assert(bool assertion, const char* message);
		#endif
	private:
		const char* m_Name;
	};
}