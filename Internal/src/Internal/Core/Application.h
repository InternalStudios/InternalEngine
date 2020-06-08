/*
    Author: Shaun Fulham
    Copyright: Internal Media Group
    Licence: IMG Public Licence
*/

#pragma once

#include "Window.h"

#include "Internal/Types/Pointer.h"

namespace Internal
{
    class Application
    {
    public:
        Application(WindowData& data);
        void Run();
        void OnEvent();
    private:
        static Application* s_Instance;
        //Pointer<Window> m_Window;
    };
}