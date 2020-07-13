/*
    Author: Shaun Fulham
    Copyright: Internal Media Group
    Licence: IMG Public Licence
*/

#pragma once

#include "Internal/Events/Event.h"

namespace Internal
{
	class Module
	{
	public:
		virtual void OnUpdate() = 0;
		virtual void OnEvent(Event& e) = 0;
		virtual void OnTick() = 0;
	};
}