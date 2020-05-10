#include "WindowsWindow.h"


namespace Internal
{
	WindowsWindow::WindowsWindow(WindowData& data)
		: Window(data)
	{
		HINSTANCE hinstance = GetModuleHandle(0);
		WNDCLASSA wc = {};
		wc.lpfnWndProc = WindowProc;
		wc.hInstance = hinstance;
		wc.lpszClassName = data.Title;

		RegisterClassA(&wc);

		HWND hwnd = CreateWindowExA(0, (LPCSTR)*data.Title, (LPCSTR)*data.Title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hinstance, NULL);
		if (hwnd == NULL)
		{
			return;
		}

		ShowWindow(hwnd, SW_SHOW);
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