#pragma once

#include <Windows.h>

namespace application {
class Window {
  public:
	static HWND createWindow(HINSTANCE instance);

  private:
	static LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
};
}; // namespace application
