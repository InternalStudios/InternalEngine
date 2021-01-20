/*
    Author: Shaun Fulham
    Copyright: Internal Media Group
    Licence: IMG Public Licence
*/

#pragma once

#undef KEY_EVENT
#undef MOUSE_EVENT

namespace Internal
{
	enum class EventType
	{
		None = 0,
		WINDOW_EVENT = 2, WINDOW_CLOSE_EVENT =  4, WINDOW_RESIZE_EVENT = 8, WINDOW_MOVE_EVENT = 16,
		KEY_EVENT = 32, KEY_PRESSED_EVENT = 64, KEY_RELEASED_EVENT = 128,
		MOUSE_EVENT = 256, MOUSE_MOVED_EVENT = 512, MOUSE_BUTTON_PRESSED_EVENT = 1024, MOUSE_BUTTON_RELEASED_EVENT = 2048, MOUSE_SCROLLED_EVENT = 4096
	};

	class Event
	{
	public:
		virtual ~Event() = 0;
		virtual EventType GetType() = 0;
	public:
		bool Handled = false;
	};
}