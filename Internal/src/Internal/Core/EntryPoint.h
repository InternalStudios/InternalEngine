/*
    Author: Shaun Fulham
    Copyright: Internal Media Group
    Licence: IMG Public Licence
*/

#pragma once

#include <iostream>

#include "Internal/Core/Window.h"
#include "Internal/Windows/WindowsWindow.h"
#include "Internal/Core/Application.h"
#include "glad/glad.h"

int main(int argc, char* argv[])
{
    Internal::WindowData data = {};
    data.Title = "InternalWindow";
    data.width = 1280;
    data.height = 720;
    Internal::WindowsWindow window(data);
    while (true)
    {
        window.OnUpdate();
        glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

}