/*
    Author: Shaun Fulham
    Copyright: Internal Media Group
    Licence: IMG Public Licence
*/

#pragma once

#include "Window.h"
#include "Internal/Vulkan/VulkanContext.h"
#include "Internal/Events/Event.h"
#include "Internal/Events/KeyboardEvent.h"
#include "Logger.h"
#include "Internal/ImGui/ImGuiLayer.h"
#include "Internal/Core/Layer.h"

namespace Internal
{

    struct ApplicationData
    {
        WindowData& m_WindowData;
        const char* m_AppName;
        int m_AppVersionMajor;
        int m_AppVersionMinor;
        int m_AppVersionPatch;
    };

    class Application
    {
    public:
        void Run();
        void OnEvent(Event& e);
        static Application* CreateApplication();
        static Application* Get() {return s_Instance;}
        Window* GetWindow() {return m_Window;}
        Logger& GetLogger() {return m_Logger;}
        static bool OnKeyPressed(KeyPressedEvent& e);
        static bool OnKeyReleased(KeyReleasedEvent& e);
    protected:
        Application(const ApplicationData& data);
    private:
        static Application* s_Instance;
        Window* m_Window;
        VulkanContext m_VContext;
        Logger m_Logger;
        LayerStack m_LayerStack;
        friend class ImGuiLayer;
    };
}