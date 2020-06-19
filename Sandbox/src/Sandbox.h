#pragma once

#include "Internal/Core/Application.h"
#include "Internal/Core/Window.h"

class Sandbox : Internal::Application
{
public:
	Sandbox(const Internal::ApplicationData& data);
private:
};

Internal::Application* Internal::Application::CreateApplication()
{
	Internal::WindowData wdata = {};
	wdata.Title = "Sandbox";
	wdata.width = 1280;
	wdata.height = 720;
	wdata.IsFocused = true;
	Internal::ApplicationData data = {wdata};
	data.m_AppName = "Sandbox";
	data.m_AppVersionMajor = 0;
	data.m_AppVersionMinor = 0;
	data.m_AppVersionPatch = 1;
	return (Internal::Application*)new Sandbox(data);
}