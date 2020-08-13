/*
    Author: Shaun Fulham
    Copyright: Internal Media Group
    Licence: IMG Public Licence
*/

#pragma once

#include "Window.h"
#include "discord.h"
#include "Internal/Vulkan/VulkanContext.h"

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
        void OnEvent();
        static Application* CreateApplication();
        static Application* Get() {return s_Instance;}
        Window* GetWindow() {return m_Window;}
    protected:
        Application(const ApplicationData& data);
    private:
        static Application* s_Instance;
        Window* m_Window;
        discord::Core* m_Discord {};
        VulkanContext m_VContext;
    };
}