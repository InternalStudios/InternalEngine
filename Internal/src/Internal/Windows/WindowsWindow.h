/*
    Author: Shaun Fulham
    Copyright: Internal Media Group
    Licence: IMG Public Licence
*/

#pragma once

#include <Windows.h>

#include "Internal/Core/Window.h"

namespace Internal
{
    class WindowsWindow : public Window
    {
    public:
        WindowsWindow(WindowData& data);
        ~WindowsWindow();
        virtual void setTitle(const char* title) override;
        virtual void setWidth(uint32_t width) override;
        virtual void setHeight(uint32_t height) override;
        virtual void OnUpdate() override;
        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

        virtual void OnEvent() override;
    private:
        HGLRC m_Context;
        HWND m_HWND;
        HDC m_HDC;
    };
}