#include "Window.hh"

#include <stdio.h>

#include "Renderer.hh"

HWND application::Window::createWindow(HINSTANCE instance) {
	printf("creating window\n");

	WNDCLASSEXW winClass = {};
	winClass.cbSize = sizeof(WNDCLASSEXW);
	winClass.style = CS_HREDRAW | CS_VREDRAW;
	winClass.lpfnWndProc = &wndProc;
	winClass.hInstance = instance;
	winClass.hIcon = LoadIconA(0, IDI_APPLICATION);
	winClass.hCursor = LoadCursorA(0, IDC_ARROW);
	winClass.lpszClassName = L"kke_window_class";
	winClass.hIconSm = LoadIconA(0, IDI_APPLICATION);
	if (!RegisterClassExW(&winClass)) {
		MessageBoxA(0, "RegisterClassEx failed", "Fatal Error", MB_OK);
		return 0;
	}

	RECT initialRect = {0, 0, 1280, 720};
	AdjustWindowRectEx(&initialRect, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_OVERLAPPEDWINDOW);
	LONG initialWidth = initialRect.right - initialRect.left;
	LONG initialHeight = initialRect.bottom - initialRect.top;
	HWND hwnd = CreateWindowExW(WS_EX_OVERLAPPEDWINDOW,
								winClass.lpszClassName,
								L"kke",
								WS_OVERLAPPEDWINDOW | WS_VISIBLE,
								CW_USEDEFAULT, CW_USEDEFAULT,
								initialWidth,
								initialHeight,
								0, 0, instance, 0);

	return hwnd;
}

LRESULT CALLBACK application::Window::wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	LRESULT result = 0;
	switch (msg) {
		case WM_KEYDOWN: {
			Renderer* renderer = reinterpret_cast<Renderer*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
			if (renderer) {
				renderer->handleKeyDown(static_cast<uint32_t>(wparam));
			}

			if (wparam == VK_ESCAPE)
				DestroyWindow(hwnd);
			break;
		}
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
		default:
			result = DefWindowProcW(hwnd, msg, wparam, lparam);
	}
	return result;
}
