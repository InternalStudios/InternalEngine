/*
    Author: Shaun Fulham
    Copyright: Internal Media Group
    Licence: IMG Public Licence
*/

#include "Application.h"

#include "Window.h"
#include "Script.h"
#include "Internal/Renderer/GraphicsContext.h"
#include "Internal/Events/KeyboardEvent.h"
#include "Internal/Events/EventHandler.h"
#include <sstream>
#include <iostream>
#include <time.h>
#include <functional>
#include "Maths.h"
#include "Internal/Renderer/Renderer.h"

namespace Internal
{
    Application* Application::s_Instance = nullptr;

    Application::Application(const ApplicationData& data)
        : m_Logger("Application")
    {
        s_Instance = this;
        m_Window = Window::CreateWindow(data.m_WindowData);
        Renderer::Init();
        m_LayerStack.PushOverlay(new ImGuiLayer());
        if (GraphicsContext::s_GraphicsContext == GraphicsContexts::Vulkan)
        {
            m_VContext.Init();
        }
        if(false)
        {
        auto result = discord::Core::Create(705604900792565821, DiscordCreateFlags_NoRequireDiscord, &m_Discord);
        discord::Activity activity {};
        activity.SetDetails("Testing");
        activity.SetState("Testing");
        activity.GetAssets().SetLargeImage("ielogo");
        activity.GetTimestamps().SetStart(time(NULL));
        m_Discord->ActivityManager().UpdateActivity(activity, [](discord::Result result){});
        }

    }

    void Application::Run()
    {
        while(!Window::ShouldClose())
        {
            //m_Logger.Info("Update");
            m_Window->OnUpdate();
            for(auto& layer : m_LayerStack)
            {
                layer->OnUpdate();
            }
            m_VContext.SwapBuffers();
            //m_Discord->RunCallbacks();
        }

        if (GraphicsContext::s_GraphicsContext == GraphicsContexts::Vulkan)
        {
            m_VContext.Shutdown();
        }
    }

    void Application::OnEvent(Event& e)
    {
        for(auto& layer : m_LayerStack)
        {
            layer->OnEvent(e);
        }
    }
}