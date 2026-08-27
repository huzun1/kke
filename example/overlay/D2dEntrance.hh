#pragma once

#include <memory>
#include <mutex>

#include <d2d1_3.h>
#include <d3d11.h>
#include <dxgi.h>
#include <wrl/client.h>

#include "D3d11ContextIsolation.hh"

namespace kke {
class D2dEngine;
}
namespace kke::example::overlay {
class PageController;

class D2dEntrance {
	std::mutex renderMutex;
	Microsoft::WRL::ComPtr<ID3D11Device> d3d11Device;
	Microsoft::WRL::ComPtr<ID2D1Factory3> d2dFactory;
	Microsoft::WRL::ComPtr<ID2D1Device2> d2dDevice;
	Microsoft::WRL::ComPtr<ID2D1DeviceContext> d2dDeviceContext;
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> renderTarget;
	D3d11ContextIsolation contextIsolation;
	std::unique_ptr<kke::D2dEngine> engine;
	std::unique_ptr<PageController> pageController;
	IDXGISwapChain* renderTargetSwapChain = nullptr;

  public:
	D2dEntrance();

	~D2dEntrance();

	void renderFrame(IDXGISwapChain* swapChain);

	void releaseBackBuffer();

	void shutdown();

  private:
	bool initialize(ID3D11Device* device);

	bool createRenderDevices(ID3D11Device* device);

	bool ensureRenderTarget(IDXGISwapChain* swapChain);

	bool createRenderTarget(IDXGISwapChain* swapChain);

	void clearRenderTarget();

	void releaseDevices();
};
} // namespace kke::example::overlay
