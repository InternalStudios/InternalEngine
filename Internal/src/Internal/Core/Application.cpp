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

    Application::Application(WindowData& data)
    {
        s_Instance = this;
    }

    void Application::Run()
    {
        /*while (!m_Window->ShouldClose())
        {
            m_Window->OnUpdate();
        }*/
    }

    void Application::OnEvent()
    {

    }
}