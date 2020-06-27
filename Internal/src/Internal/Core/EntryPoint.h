/*
    Author: Shaun Fulham
    Copyright: Internal Media Group
    Licence: IMG Public Licence
*/

#pragma once

#include <iostream>

#include "Internal/Core/Window.h"
#include "Internal/Core/Application.h"
#include "glad/glad.h"

int main(int argc, char* argv[])
{
    Internal::Application* application = Internal::Application::CreateApplication();
    application->Run();
}