#pragma once

#include <d2d1.h>
#include <d2d1_3.h>
#include <d3d11_1.h>
#include <dxgi1_2.h>

#include "D3D11.hpp"

#pragma comment(lib, "D2d1.lib")
#pragma comment(lib, "dxguid.lib")

namespace application {
class D2D1 {
	static inline ID2D1Factory3* d2d1Factory = nullptr;
	static inline ID2D1Device* d2d1Device = nullptr;
	static inline ID2D1DeviceContext* d2d1DeviceContext = nullptr;
	static inline ID2D1Bitmap1* renderTarget = nullptr;

public:
	void init(D3D11& context);

	ID2D1DeviceContext* getDeviceContext();

	ID2D1Bitmap1* getRenderTarget();

private:
	static void createFactory(ID2D1Factory3** factoryOut);

	static void createDevice(ID3D11Device1* d11Device, ID2D1Device** deviceOut);

	static void createDeviceContext(ID2D1DeviceContext** deviceContextOut);

	static void createAndSetRenderTarget(IDXGISwapChain1* swapChain, ID2D1DeviceContext* deviceContext, ID2D1Bitmap1** renderTargetOut);
};
}  // namespace application
