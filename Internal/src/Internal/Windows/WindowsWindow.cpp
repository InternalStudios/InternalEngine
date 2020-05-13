#include "WindowsWindow.h"

#include <string>

#include "Internal/Core/Application.h"

#include "glad/glad.h"
#include "glad/glad_wgl.h"

#include <iostream>

namespace Internal
{
	WindowsWindow::WindowsWindow(WindowData& data)
		: Window(data)
	{
		int wstr_size = MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, data.Title, -1, nullptr, 0);
		wchar_t* title = new wchar_t[wstr_size];
		MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, data.Title, -1, title, wstr_size);
		HINSTANCE hinstance = GetModuleHandle(0);
		WNDCLASSW wc = {};
		wc.lpfnWndProc = WindowProc;
		wc.hInstance = hinstance;
		wc.lpszClassName = title;

		RegisterClass(&wc);

		HWND hwnd = CreateWindowExW(0, title, title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hinstance, NULL);
		if (hwnd == NULL)
		{
			return;
		}

		ShowWindow(hwnd, SW_SHOW);

		HDC deviceContext = GetDC(hwnd);
		if (!gladLoadWGL(deviceContext))
			std::cout << "Failed to load WGL";

		PIXELFORMATDESCRIPTOR pfd = {
			sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
			PFD_TYPE_RGBA,
			32,
			0, 0, 0, 0, 0, 0,
			0,
			0,
			0,
			0, 0, 0, 0,
			24,
			8,
			0,
			PFD_MAIN_PLANE,
			0,
			0, 0, 0
		};

		unsigned int format = ChoosePixelFormat(deviceContext, &pfd);


		SetPixelFormat(deviceContext, format, &pfd);

		m_Context = wglCreateContext(deviceContext);

		wglMakeCurrent(deviceContext, m_Context);
		gladLoadGL();
		glEnable(GL_DEPTH);
	}

	WindowsWindow::~WindowsWindow()
	{
		wglDeleteContext(m_Context);
	}

	void WindowsWindow::setTitle(const char* title)
	{
	}

	void WindowsWindow::setWidth(uint32_t width)
	{
	}

	void WindowsWindow::setHeight(uint32_t height)
	{
	}

	void WindowsWindow::OnUpdate()
	{
		MSG msg = {};
		GetMessage(&msg, NULL, 0, 0);
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	LRESULT CALLBACK WindowsWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			Window::s_ShouldClose = true;
			return 0;

		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);

			FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

			EndPaint(hwnd, &ps);
		}
		return 0;

		}
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	void WindowsWindow::OnEvent()
	{
	}
}