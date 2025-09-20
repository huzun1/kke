#include <Windows.h>
#include <winuser.h>

#include <cstdio>

#include "Renderer.hpp"
#include "Window.hpp"
#include "backend/D2D1.hpp"
#include "backend/D3D11.hpp"

application::D3D11 d3d11;
application::D2D1 d2d1;

void createConsole() {
	AllocConsole();
	FILE* newStdout;
	FILE* newStdin;
	freopen_s(&newStdout, "CONOUT$", "w", stdout);
	freopen_s(&newStdin, "CONIN$", "r", stdin);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
	createConsole();
	HWND hwnd = application::Window::createWindow(hInstance);
	if (!hwnd) {
		return -1;
	}

	d3d11.init(hwnd);
	d2d1.init(d3d11);

	application::Renderer renderer(d2d1);

	bool isRunning = true;
	while (isRunning) {
		MSG msg;
		while (PeekMessageW(&msg, 0, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT)
				isRunning = false;
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}

		ID3D11DeviceContext1* d3d11DeviceContext = d3d11.getDeviceContext();
		ID3D11RenderTargetView* targetView = d3d11.getFramebuffer();
		IDXGISwapChain1* d3d11SwapChain = d3d11.getSwapChain();

		FLOAT backgroundColor[4] = {0.1f, 0.2f, 0.6f, 1.0f};
		d3d11DeviceContext->ClearRenderTargetView(targetView, backgroundColor);
		renderer.render();
		d3d11DeviceContext->Flush();

		d3d11SwapChain->Present(1, 0);
		Sleep(1000 / 180);
	}

	printf("Done\n");

	return 0;
}
