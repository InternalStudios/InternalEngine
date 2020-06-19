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
        WindowData& WindowData;
        const char* AppName;
        int AppVersionMajor;
        int AppVersionMinor;
        int AppVersionPatch;
    };

    class Application
    {
    public:
        Application(ApplicationData& data);
        void Run();
        void OnEvent();
    private:
        static Application* s_Instance;
        Window* m_Window;
    };
}