/*
    Author: Shaun Fulham
    Copyright: Internal Media Group
    Licence: IMG Public Licence
*/

#include "LinuxWindow.h"

namespace Internal
{
    LinuxWindow::LinuxWindow(const WindowData& data)
        : Window(data)
    {
        m_Display = XOpenDisplay(NULL);
        if(!m_Display)
        {

        }
        gladLoadGLX(m_Display, 0);

        m_Root = DefaultRootWindow(m_Display);
        m_VI = glXChooseVisual(m_Display, 0, m_Attributes);
        if(!m_VI)
        {

        }

        m_CMap = XCreateColormap(m_Display, m_Root, m_VI->visual, AllocNone);
        m_SWA.colormap = m_CMap;
        m_SWA.event_mask = ExposureMask | KeyPressMask;

        m_Window = XCreateWindow(m_Display, m_Root, 0, 0, m_Data.width, m_Data.height, 0, m_VI->depth, InputOutput, m_VI->visual, CWColormap | CWEventMask, &m_SWA);
        XMapWindow(m_Display, m_Window);
        XStoreName(m_Display, m_Window, m_Data.Title);
        m_VContext.Init();
    }

    void LinuxWindow::OnUpdate()
    {
        XNextEvent(m_Display, &m_Event);
        if(m_Event.type == Expose)
        {
            XGetWindowAttributes(m_Display, m_Window, &m_XWA);
        }
    }

    LinuxWindow::~LinuxWindow()
    {
        m_VContext.Shutdown();
    }
}