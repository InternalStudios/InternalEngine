#include "Event.h"

namespace Internal
{
	class WindowEvent : public Event
	{
	public:
		virtual EventType GetType() = 0;
	};	

	class WindowResizeEvent : public WindowEvent
	{
	public:
		WindowResizeEvent(int width, int height) : m_Width(width), m_Height(height) {};
		virtual EventType GetType() {return WINDOW_RESIZE_EVENT;}
		static EventType GetTypeS() {return WINDOW_RESIZE_EVENT;}
	private:
		int m_Width, m_Height;
	};
}
