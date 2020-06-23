/*
    Author: Shaun Fulham
    Copyright: Internal Media Group
    Licence: IMG Public Licence
*/

#include "Application.h"

#include "Window.h"

#include <vulkan/vulkan.hpp>

#include <iostream>

namespace Internal
{
    Application* Application::s_Instance = nullptr;

    Application::Application(const ApplicationData& data)
    {
        s_Instance = this;
        m_Window = Window::CreateWindow(data.m_WindowData);
        uint32_t extenstionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extenstionCount, nullptr);
        std::cout << extenstionCount << " extentions supported\n";
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