/*
    Author: Shaun Fulham
    Copyright: Internal Media Group
    Licence: IMG Public Licence
*/

#pragma once

#include <cstdint>
#include <memory>

namespace Internal
{
    template<typename T>
    class UniquePointer
    {
    public:
    private:
    };

    template<typename T>
    class SharedPointer
    {
    public:
        SharedPointer(SharedPointer& sharedPointer)
        {
            m_Pointer = sharedPointer.m_Pointer;
            m_RefCount = sharedPointer.m_RefCount;
            (*m_RefCount)++;
        }
        SharedPointer(T* pointer)
        {
            m_Pointer = pointer;
        }
        template<typename... Args>
        SharedPointer(Args&&... args)
        {
            uint32_t refcount = 1;
            m_RefCount = &refcount;
            m_Pointer = new T(args...);
        };

        ~SharedPointer()
        {
            (*m_RefCount)--;
            if(m_RefCount <= 0)
            {
                delete m_Pointer;
                delete m_RefCount;
            }
        }
    private:
        int32_t* m_RefCount;
        T* m_Pointer;
    };
}