/*
    Author: Shaun Fulham
    Copyright: Internal Media Group
    Licence: IMG Public Licence
*/

#include "EventHandler.h"
#include "Internal/Core/Application.h"

namespace Internal
{

	template<typename T>
	void EventHandler::Dispatch(Event& e, bool(*f)(T&))
	{
		if(e.GetType() == T::GetTypeS() && (e.Handled == false))
		{
			e.Handled = !f(e);
		}
	}

	void EventHandler::PushEvent(Event& e)
	{
        Application::Get()->OnEvent(e);
	}
}