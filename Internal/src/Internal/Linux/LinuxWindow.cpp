/*
    Author: Shaun Fulham
    Copyright: Internal Media Group
    Licence: IMG Public Licence
*/

#include "LinuxWindow.h"

#include "Internal/Events/KeyboardEvent.h"
#include "Internal/Events/MouseEvent.h"
#include <iostream>
#include "Internal/Core/Keycodes.h"

namespace Internal
{

    int ConvertXCBMouseCodeToInternal(int keycode)
    {
        switch(keycode)
        {
            case 1: return 0;
            case 2: return 2;
            case 3: return 1;
        }
        return 0;
    }

    int ConvertXCBKeyCodeToInternal(int keycode)
    {
        switch(keycode)
        {
            case 9: return INTERNAL_KEY_ESCAPE;
            case 10: return INTERNAL_KEY_1;
            case 11: return INTERNAL_KEY_2;
            case 12: return INTERNAL_KEY_3;
            case 13: return INTERNAL_KEY_4;
            case 14: return INTERNAL_KEY_5;
            case 15: return INTERNAL_KEY_6;
            case 16: return INTERNAL_KEY_7;
            case 17: return INTERNAL_KEY_8;
            case 18: return INTERNAL_KEY_9;
            case 19: return INTERNAL_KEY_0;
            case 20: return INTERNAL_KEY_MINUS;
            case 21: return INTERNAL_KEY_EQUAL;
            case 22: return INTERNAL_KEY_BACKSPACE;
            case 23: return INTERNAL_KEY_TAB;
            case 24: return INTERNAL_KEY_Q;
            case 25: return INTERNAL_KEY_W;
            case 26: return INTERNAL_KEY_E;
            case 27: return INTERNAL_KEY_R;
            case 28: return INTERNAL_KEY_T;
            case 29: return INTERNAL_KEY_Y;
            case 30: return INTERNAL_KEY_U;
            case 31: return INTERNAL_KEY_I;
            case 32: return INTERNAL_KEY_O;
            case 33: return INTERNAL_KEY_P;
            case 34: return INTERNAL_KEY_LEFT_BRACKET;
            case 35: return INTERNAL_KEY_RIGHT_BRACKET;
            case 36: return INTERNAL_KEY_ENTER;
            case 37: return INTERNAL_KEY_LEFT_CONTROL;
            case 38: return INTERNAL_KEY_A;
            case 39: return INTERNAL_KEY_S;
            case 40: return INTERNAL_KEY_D;
            case 41: return INTERNAL_KEY_F;
            case 42: return INTERNAL_KEY_G;
            case 43: return INTERNAL_KEY_H;
            case 44: return INTERNAL_KEY_J;
            case 45: return INTERNAL_KEY_K;
            case 46: return INTERNAL_KEY_L;
            case 47: return INTERNAL_KEY_SEMICOLON;
            case 48: return INTERNAL_KEY_APOSTROPHE;
            case 49: return INTERNAL_KEY_GRAVE_ACCENT;
            case 50: return INTERNAL_KEY_LEFT_SHIFT;
            case 51: return INTERNAL_KEY_BACKSLASH;
            case 52: return INTERNAL_KEY_Z;
            case 53: return INTERNAL_KEY_X;
            case 54: return INTERNAL_KEY_C;
            case 55: return INTERNAL_KEY_V;
            case 56: return INTERNAL_KEY_B;
            case 57: return INTERNAL_KEY_N;
            case 58: return INTERNAL_KEY_M;
            case 59: return INTERNAL_KEY_COMMA;
            case 60: return INTERNAL_KEY_PERIOD;
            case 61: return INTERNAL_KEY_SLASH;
            case 62: return INTERNAL_KEY_RIGHT_SHIFT;
            case 63: return INTERNAL_KEY_0;
            case 64: return INTERNAL_KEY_0;
            case 65: return INTERNAL_KEY_SPACE;
            case 66: return INTERNAL_KEY_CAPS_LOCK;
            case 67: return INTERNAL_KEY_F1;
            case 68: return INTERNAL_KEY_F2;
            case 69: return INTERNAL_KEY_F3;
            case 70: return INTERNAL_KEY_F4;
            case 71: return INTERNAL_KEY_F5;
            case 72: return INTERNAL_KEY_F6;
            case 73: return INTERNAL_KEY_F7;
            case 74: return INTERNAL_KEY_F8;
            case 75: return INTERNAL_KEY_F9;
            case 76: return INTERNAL_KEY_F10;
        }
        return 0;
    }

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
                    KeyPressedEvent ke(ConvertXCBKeyCodeToInternal((int)((xcb_key_press_event_t*)e)->detail), false);
                    EventHandler::PushEvent(ke);
                    std::cout << ConvertXCBKeyCodeToInternal((int)((xcb_key_press_event_t*)e)->detail) << std::endl;
                }
                case XCB_KEY_RELEASE:
                {
                    if(e->response_type == XCB_KEY_PRESS)
                    {
                        goto end;
                    }
                    KeyReleasedEvent ke(ConvertXCBKeyCodeToInternal((int)((xcb_key_press_event_t*)e)->detail));
                    EventHandler::PushEvent(ke);
                }
                case XCB_BUTTON_PRESS:
                {
                    if(((xcb_button_press_event_t*)e)->detail == 1 || ((xcb_button_press_event_t*)e)->detail == 2 || ((xcb_button_press_event_t*)e)->detail == 3)
                    {
                        MouseButtonPressedEvent ke(ConvertXCBMouseCodeToInternal((int)((xcb_button_press_event_t*)e)->detail));
                        EventHandler::PushEvent(ke);
                        std::cout << ((xcb_button_press_event_t*)e)->detail << std::endl;
                    }
                }
                case XCB_BUTTON_RELEASE:
                {
                    if(e->response_type == XCB_BUTTON_PRESS)
                    {
                        goto end;
                    }
                    if(((xcb_button_press_event_t*)e)->detail == 1 || ((xcb_button_press_event_t*)e)->detail == 2 || ((xcb_button_press_event_t*)e)->detail == 3)
                    {
                        MouseButtonReleasedEvent ke(ConvertXCBMouseCodeToInternal((int)((xcb_button_press_event_t*)e)->detail));
                        EventHandler::PushEvent(ke);
                        std::cout << "Mouse released" << std::endl;
                        std::cout << (uint32_t)((xcb_button_press_event_t*)e)->detail << std::endl;
                    }
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