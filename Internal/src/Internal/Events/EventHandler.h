/*
    Author: Shaun Fulham
    Copyright: Internal Media Group
    Licence: IMG Public Licence
*/

#pragma once

#include "Event.h"
#include "Internal/Core/Application.h"

namespace Internal
{
	class EventHandler
	{
	public:
        template<typename T>
		static void PushEvent(T& e)
        {
		    Application::Get()->OnEvent(e);
        }
		
		template<typename T, typename F>
		void Dispatch(Event& e, const F& f)
		{
            if(e.GetType() == T::GetTypeS() && (e.Handled == false))
            {
                bool handled = f(static_cast<T&>(e));
                e.Handled = !handled;
            }
        }
	};
}