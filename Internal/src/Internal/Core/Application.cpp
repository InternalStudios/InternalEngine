/*
    Author: Shaun Fulham
    Copyright: Internal Media Group
    Licence: IMG Public Licence
*/

#include "Application.h"

#include "Window.h"
#include "Script.h"
#include "Internal/Renderer/GraphicsContext.h"

#include <iostream>
#include <time.h>

namespace Internal
{
    Application* Application::s_Instance = nullptr;

    Application::Application(const ApplicationData& data)
    {
        s_Instance = this;
        m_Window = Window::CreateWindow(data.m_WindowData);
        if (GraphicsContext::s_GraphicsContext == GraphicsContexts::Vulkan)
        {
            m_VContext.Init();
        }
        auto result = discord::Core::Create(705604900792565821, DiscordCreateFlags_NoRequireDiscord, &m_Discord);
        discord::Activity activity {};
        activity.SetDetails("Testing");
        activity.SetState("Testing");
        activity.GetAssets().SetLargeImage("ielogo");
        activity.GetTimestamps().SetStart(time(NULL));
        m_Discord->ActivityManager().UpdateActivity(activity, [](discord::Result result){});
    }

    void Application::Run()
    {
        while (!Window::ShouldClose())
        {
            m_Window->OnUpdate();
            m_Discord->RunCallbacks();
        }

        if (GraphicsContext::s_GraphicsContext == GraphicsContexts::Vulkan)
        {
            m_VContext.Shutdown();
        }
    }

    void Application::OnEvent()
    {

    }
}