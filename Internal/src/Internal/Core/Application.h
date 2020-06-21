/*
    Author: Shaun Fulham
    Copyright: Internal Media Group
    Licence: IMG Public Licence
*/

#pragma once

#include "Window.h"

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
    protected:
        Application(const ApplicationData& data);
    private:
        static Application* s_Instance;
        Window* m_Window;
    };
}