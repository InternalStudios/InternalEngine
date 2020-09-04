/*
    Author: Shaun Fulham
    Copyright: Internal Media Group
    Licence: IMG Public Licence
*/

#pragma once

#include "Internal/Core/Window.h"
#include "Internal/Events/KeyboardEvent.h"
#include "Internal/Events/WindowEvent.h"
#include "Internal/Events/EventHandler.h"
#include "Internal/Core/Application.h"

#include <thread>

#include "Internal/Vulkan/VulkanContext.h"
#include "Internal/Events/Event.h"
#include "LinuxVSurface.h"

namespace Internal
{
    class LinuxWindow : public Window
    {
    public:
        LinuxWindow(const WindowData& data);
        ~LinuxWindow();
        virtual void setTitle(const char* title) override {};
        virtual void setWidth(uint32_t width) override {};
        virtual void setHeight(uint32_t height) override {};

        static void CheckEvents();
        virtual void OnUpdate() override;
        virtual void OnEvent(Event& e) override {};
        virtual void OnTick() override {};
        virtual void CreateSurface(VkInstance& instance, VkSurfaceKHR* surface) override;
    private:
        xcb_connection_t* m_Connection;
        xcb_screen_t* m_Screen;
        xcb_window_t m_Window;
        LinuxVSurface m_Surface;
        static LinuxWindow* s_Instance;
    };
}