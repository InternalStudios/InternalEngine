/*
    Author: Shaun Fulham
    Copyright: Internal Media Group
    Licence: IMG Public Licence
*/

#pragma once

#include "Event.h"

namespace Internal
{
	class EventHandler
	{
	public:
		static void PushEvent(Event& e);
		
		template<typename T>
		void Dispatch(Event& e, bool(*f)(T&));
	};
}