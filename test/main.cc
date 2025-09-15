#include <Windows.h>

#include <cstdio>

#include "window.hpp"

void createConsole() {
	AllocConsole();
	FILE* newStdout;
	FILE* newStdin;
	freopen_s(&newStdout, "CONOUT$", "w", stdout);
	freopen_s(&newStdin, "CONIN$", "r", stdin);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
	createConsole();
	HWND hwnd = window::createWindow(hInstance);
	if (!hwnd) {
		return -1;
	}

	Sleep(100000);

	return 0;
}
