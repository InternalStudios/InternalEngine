/*
    Author: Shaun Fulham
    Copyright: Internal Media Group
    Licence: IMG Public Licence
*/

#include "Pointer.h"

namespace Internal
{
	template<typename T>
	Pointer::Pointer(T* pointer)
	{
		if(!m_RefCount)
		{
			m_RefCount = new int;
			*m_RefCount = 1;
		} else
		{
			(*m_RefCount)++;
		}
		
		m_Pointer = pointer;
	}

	template<typename T>
	Pointer::Pointer(const Pointer<T>& pointer)
	{
		m_RefCount = pointer.RefCount;
		//Pointer(pointer.m_Pointer);
	}

	template<typename T>
	Pointer::~Pointer()
	{
		if(*m_RefCount == 0)
		{
			delete m_Pointer;
		} else
		{
			(*m_RefCount)--;
		}
		
	}
}
