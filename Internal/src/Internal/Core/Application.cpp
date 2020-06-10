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

    Application::Application(ApplicationData& data)
    {
        s_Instance = this;
        m_Window = Window::CreateWindow(data.WindowData);
    }

    void Application::Run()
    {
        while (!Window::ShouldClose())
        {
            m_Window->OnUpdate();
        }
    }

    void Application::OnEvent()
    {

    }
}