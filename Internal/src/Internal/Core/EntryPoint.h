/*
    Author: Shaun Fulham
    Copyright: Internal Media Group
    Licence: IMG Public Licence
*/

#pragma once

#include "Internal/Core/Window.h"
//#include "Internal/Windows/WindowsWindow.h"
#include "Internal/Core/Application.h"

#include "glad/glad.h"

int main(int argc, char* argv[])
{
    Internal::WindowData data = {};
    data.Title = "Window";
    data.width = 1280;
    data.height = 720;
    data.IsFocused = true;
/*    Internal::WindowsWindow window(data);
    while (!Internal::Window::ShouldClose())
    {
        window.OnUpdate();
        glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    }*/
}