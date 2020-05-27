/*
    Author: Shaun Fulham
    Copyright: Internal Media Group
    Licence: IMG Public Licence
*/

#include "Application.h"

#include "Window.h"

namespace Internal
{
    Application* Application::s_Instance = nullptr;

    Application::Application()
    {
        s_Instance = this;
    }

    void Application::Run()
    {
        while (true)
        {
            m_Window
        }
    }

    void Application::OnEvent()
    {

    }
}