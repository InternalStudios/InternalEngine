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

namespace Internal
{
    Application* Application::s_Instance = nullptr;

    Application::Application(const ApplicationData& data)
        : m_Logger("Application")
    {
        s_Instance = this;
        m_Window = Window::CreateWindow(data.m_WindowData);
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
        EventHandler h;
        h.Dispatch<KeyPressedEvent>(e, std::bind(OnKeyPressed, std::placeholders::_1));
        h.Dispatch<KeyReleasedEvent>(e, std::bind(OnKeyReleased, std::placeholders::_1));
        m_VContext.OnEvent(e);
    }

    bool Application::OnKeyPressed(KeyPressedEvent& e)
    {
        std::stringstream ss;
        ss << "Key Pressed: ";
        ss << static_cast<KeyPressedEvent>(e).GetKey();
        Application::Get()->GetLogger().Info(ss.str().c_str());
        return true;
    }

    bool Application::OnKeyReleased(KeyReleasedEvent& e)
    {
        std::stringstream ss;
        ss << "Key Released: ";
        ss << static_cast<KeyReleasedEvent>(e).GetKey();
        Application::Get()->GetLogger().Info(ss.str().c_str());
        return true;
    }
}