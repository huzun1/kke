#include <Windows.h>
#include <synchapi.h>
#include <winuser.h>

#include <cstdio>

#include "Renderer.hh"
#include "Window.hh"
#include "backend/D2D1.hh"
#include "backend/D3D11.hh"

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
	HRESULT comResult = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
	bool isComInitialized = SUCCEEDED(comResult) || comResult == S_FALSE;
	createConsole();
	HWND hwnd = application::Window::createWindow(hInstance);
	if (!hwnd) {
		if (isComInitialized) {
			CoUninitialize();
		}
		return -1;
	}

	d3d11.init(hwnd);
	d2d1.init(d3d11);

	application::Renderer* renderer = new application::Renderer(d2d1);
	SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(renderer));

	bool isRunning = true;
	while (isRunning) {
		MSG msg;
		while (PeekMessageW(&msg, 0, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				isRunning = false;
			}
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}

		if (!isRunning) {
			break;
		}

		ID3D11DeviceContext1* d3d11DeviceContext = d3d11.getDeviceContext();
		ID3D11RenderTargetView* targetView = d3d11.getFramebuffer();
		IDXGISwapChain1* d3d11SwapChain = d3d11.getSwapChain();

		FLOAT backgroundColor[4] = {0.3f, 0.6f, 0.8f, 1.0f};
		d3d11DeviceContext->ClearRenderTargetView(targetView, backgroundColor);
		renderer->render();
		d3d11DeviceContext->Flush();

		d3d11SwapChain->Present(0, 0);

		Sleep(1);
	}

	delete renderer;
	d2d1.shutdown();
	d3d11.shutdown();
	if (isComInitialized) {
		CoUninitialize();
	}

	return 0;
}
