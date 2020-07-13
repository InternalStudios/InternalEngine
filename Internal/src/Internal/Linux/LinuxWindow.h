/*
    Author: Shaun Fulham
    Copyright: Internal Media Group
    Licence: IMG Public Licence
*/

#pragma once

#include "Internal/Core/Window.h"

#include <X11/X.h>
#include <X11/Xlib.h>
#include "glad/glad.h"
#include "glad/glad_glx.h"

#include "Internal/Vulkan/VulkanContext.h"
#include "Internal/Events/Event.h"

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

        virtual void OnUpdate() override;
        virtual void OnEvent(Event& e) override {};
        virtual void OnTick() override {};
    private:
        Display* m_Display;
        ::Window m_Root;
        Visual* m_Visual;
        Colormap m_CMap;
        XSetWindowAttributes m_SWA;
        ::Window m_Window;
        XWindowAttributes m_XWA;
        XEvent m_Event;
        VulkanContext m_VContext;
        int m_Depth;
    };
}