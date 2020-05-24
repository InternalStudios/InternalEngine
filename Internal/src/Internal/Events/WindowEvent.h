#include "Event.h"

namespace Internal
{
	class WindowEvent : public Event
	{

	};	

	class WindowResizeEvent : public WindowEvent
	{
		public:
		private:
		int m_Width, m_Height;
	};
}
