/*
    Author: Shaun Fulham
    Copyright: Internal Media Group
    Licence: IMG Public Licence
*/

#pragma once

namespace Internal
{
	enum EventType
	{

	};
	class Event
	{
	public:
		virtual EventType GetType() = 0;
	public:
		bool Handled = false;
	private:
	};
}