/*
    Author: Shaun Fulham
    Copyright: Internal Media Group
    Licence: IMG Public Licence
*/

#include "LinuxWindow.h"

#include "Internal/Events/KeyboardEvent.h"
#include "Internal/Events/MouseEvent.h"
#include <iostream>
#include <xcb/xinput.h>

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
        uint32_t masks = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
        uint32_t events[2] = {m_Screen->white_pixel, XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_KEY_RELEASE |XCB_EVENT_MASK_RESIZE_REDIRECT | XCB_EVENT_MASK_POINTER_MOTION | XCB_EVENT_MASK_BUTTON_RELEASE | XCB_EVENT_MASK_BUTTON_PRESS};
        xcb_create_window(m_Connection, XCB_COPY_FROM_PARENT, m_Window, m_Screen->root, 0, 0, m_Data.width, m_Data.height, 10, XCB_WINDOW_CLASS_INPUT_OUTPUT, m_Screen->root_visual, masks, events);
        xcb_map_window(m_Connection, m_Window);
        xcb_flush(m_Connection);
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
                case XCB_KEY_PRESS:
                {
                    KeyPressedEvent ke(((xcb_key_press_event_t*)e)->detail, false);
                    EventHandler::PushEvent(ke);
                }
                case XCB_KEY_RELEASE:
                {
                    KeyReleasedEvent ke(((xcb_key_press_event_t*)e)->detail);
                    EventHandler::PushEvent(ke);
                }
                case XCB_BUTTON_PRESS:
                {
                    MouseButtonPressedEvent ke(((xcb_input_raw_button_press_event_t *)e)->detail);
                    EventHandler::PushEvent(ke);
                }
                case XCB_BUTTON_RELEASE:
                {
                    if(e->response_type == XCB_BUTTON_PRESS)
                    {
                        goto end;
                    }
                    MouseButtonReleasedEvent ke(((xcb_input_raw_button_release_event_t*)e)->detail);
                    EventHandler::PushEvent(ke);
                    std::cout << "Mouse released" << std::endl;
                }
                case XCB_MOTION_NOTIFY:
                {
                    MouseMovedEvent me(((xcb_motion_notify_event_t*)e)->event_x, ((xcb_motion_notify_event_t*)e)->event_y);
                    EventHandler::PushEvent(me);
                    std::cout << ((xcb_motion_notify_event_t*)e)->event_x << ',' << ((xcb_motion_notify_event_t*)e)->event_y << std::endl;
                }
            }
        }
        end:
        return;
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