/*
    Author: Shaun Fulham
    Copyright: Internal Media Group
    Licence: IMG Public Licence
*/

#include "Window.h"
#include "Core.h"
#ifdef INTERNAL_WINDOWS
#include "Internal/Windows/WindowsWindow.h"
#elif defined(INTERNAL_LINUX)
#include "Internal/Linux/LinuxWindow.h"
#endif

namespace Internal
{
	Window::Window(const WindowData& data)
		: m_Data(data)
	{
		
	}

	Window* Window::CreateWindow(WindowData& data)
	{
		#ifdef INTERNAL_WINDOWS
		return new WindowsWindow(data);
		#elif defined(INTERNAL_LINUX)
		return new LinuxWindow(data)
		#endif
	}
}