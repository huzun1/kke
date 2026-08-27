#pragma once

#include <d3d11.h>
#include <d3d11_1.h>
#include <dxgi1_2.h>

#define NOMINMAX
#define UNICODE
#include <Windows.h>

#pragma comment(lib, "d3d11.lib")

namespace application {
class D3D11 {
	ID3D11Device1* d3d11Device = nullptr;
	ID3D11DeviceContext1* d3d11DeviceContext = nullptr;

	IDXGIFactory2* dxgiFactory = nullptr;
	IDXGISwapChain1* d3d11SwapChain = nullptr;

	ID3D11RenderTargetView* d3d11FramebufferView = nullptr;

  public:
	void init(HWND hwnd);

	void shutdown();

	ID3D11Device1* getDevice();

	ID3D11DeviceContext1* getDeviceContext();

	IDXGISwapChain1* getSwapChain();

	ID3D11RenderTargetView* getFramebuffer();

  private:
	static bool
	createD3D11Device(ID3D11Device1** deviceOut, ID3D11DeviceContext1** deviceContextOut);

	static void createDxgiFactory(ID3D11Device1* device, IDXGIFactory2** factoryOut);

	static void createSwapChain(
		HWND hwnd, ID3D11Device1* device, IDXGIFactory2* factory, IDXGISwapChain1** swapChainOut
	);

	static void createRenderTarget(
		ID3D11Device1* device,
		IDXGISwapChain1* swapChain,
		ID3D11RenderTargetView** FramebufferViewOut
	);
};
} // namespace application
