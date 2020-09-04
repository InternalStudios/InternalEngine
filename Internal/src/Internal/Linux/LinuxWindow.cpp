/*
    Author: Shaun Fulham
    Copyright: Internal Media Group
    Licence: IMG Public Licence
*/

#include "LinuxWindow.h"



namespace Internal
{

    LinuxWindow* LinuxWindow::s_Instance = nullptr;

    LinuxWindow::LinuxWindow(const WindowData& data)
        : Window(data)
    {
        s_Instance = this;

        m_Connection = xcb_connect(NULL, NULL);
        m_Screen = xcb_setup_roots_iterator(xcb_get_setup(m_Connection)).data;
        m_Window = xcb_generate_id(m_Connection);
        xcb_create_window(m_Connection, XCB_COPY_FROM_PARENT, m_Window, m_Screen->root, 0, 0, m_Data.width, m_Data.height, 10, XCB_WINDOW_CLASS_INPUT_OUTPUT, m_Screen->root_visual, 0, NULL);
        xcb_map_window(m_Connection, m_Window);
        xcb_flush(m_Connection);
        uint32_t masks = XCB_CW_EVENT_MASK;
        uint32_t events = XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_KEY_RELEASE | XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE | XCB_EVENT_MASK_RESIZE_REDIRECT;
    }


    void LinuxWindow::OnUpdate()
    {
        xcb_generic_event_t* e;
        while((e = xcb_poll_for_event(m_Connection)))
        {
            switch (e->response_type & ~0x80) {
                case XCB_RESIZE_REQUEST:
                {
                    WindowResizeEvent re(((xcb_resize_request_event_t*)e)->width, ((xcb_resize_request_event_t*)e)->height);
                    EventHandler::PushEvent(re);
                }
            }
        }
    }

    LinuxWindow::~LinuxWindow()
    {
        xcb_disconnect(m_Connection);
    }

    void LinuxWindow::CreateSurface(VkInstance &instance, VkSurfaceKHR *surface)
    {
        m_Surface.Init(m_Connection, m_Window, instance, surface);
    }
}