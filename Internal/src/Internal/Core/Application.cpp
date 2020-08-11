/*
    Author: Shaun Fulham
    Copyright: Internal Media Group
    Licence: IMG Public Licence
*/

#include "Application.h"

#include "Window.h"
#include "Script.h"

#include <iostream>

namespace Internal
{
    Application* Application::s_Instance = nullptr;

    Application::Application(const ApplicationData& data)
    {
        s_Instance = this;
        //m_Window = Window::CreateWindow(data.m_WindowData);
        Script discord("Discord", "Discord.exe");

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