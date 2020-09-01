/*
    Author: Shaun Fulham
    Copyright: Internal Media Group
    Licence: IMG Public Licence
*/

#pragma once

#include "Event.h"

namespace Internal
{
    class KeyboardEvent : public Event
    {
    public:
        virtual EventType GetType() = 0;
    };

    class KeyPressedEvent : public KeyboardEvent
    {
    public:
        KeyPressedEvent(int keycode, bool repeat) : m_Keycode(keycode), m_Repeat(repeat) {};
        virtual EventType GetType() { return KEY_PRESSED_EVENT; };
        int GetKey() { return m_Keycode; };
    private:
        int m_Keycode;
        bool m_Repeat;
    };

    class KeyReleasedEvent : public KeyboardEvent
    {
    public:
        KeyReleasedEvent(int keycode) : m_Keycode(keycode) {};
        virtual EventType GetType() { return KEY_RELEASED_EVENT; };
    private:
        int m_Keycode;
    };
}