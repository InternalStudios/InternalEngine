/*
    Author: Shaun Fulham
    Copyright: Internal Media Group
    Licence: IMG Public Licence
*/

namespace Internal
{
	class Module
	{
	public:
		virtual void OnUpdate() = 0;
		virtual void OnEvent() = 0;
	};
}