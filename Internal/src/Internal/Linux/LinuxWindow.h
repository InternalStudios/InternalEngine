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

namespace Internal
{
    class LinuxWindow : public Window
    {
    public:
        LinuxWindow(WindowData& data);
        virtual void setTitle(const char* title) override {};
        virtual void setWidth(uint32_t width) override {};
        virtual void setHeight(uint32_t height) override {};

        virtual void OnUpdate() override;
    private:
        Display* m_Display;
        ::Window m_Root;
        GLint m_Attributes[5] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
        XVisualInfo* m_VI;
        Colormap m_CMap;
        XSetWindowAttributes m_SWA;
        ::Window m_Window;
        GLXContext m_Context;
        XWindowAttributes m_XWA;
        XEvent m_Event;
    };
}