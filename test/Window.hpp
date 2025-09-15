#pragma once

#include <Windows.h>

class window {
public:
	static HWND createWindow(HINSTANCE instance);

private:
	static LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
};
