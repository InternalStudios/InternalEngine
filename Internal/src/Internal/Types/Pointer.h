/*
    Author: Shaun Fulham
    Copyright: Internal Media Group
    Licence: IMG Public Licence
*/

#pragma once

namespace Internal
{
	template<typename T>
	class Pointer
	{
	public:
		Pointer(T* pointer);
		Pointer(const Pointer<T>& pointer);
		T* operator->();
		~Pointer();
	private:
		int* m_RefCount;
		T* m_Pointer;
	};

	template<typename T, typename... Args>
	Pointer<T> CreatePointer(Args... args)
	{

	}
}