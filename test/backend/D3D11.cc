#include "D3D11.hh"

#include <assert.h>

void application::D3D11::init(HWND hwnd) {
	bool d3d11Result = createD3D11Device(&d3d11Device, &d3d11DeviceContext);
	if (!d3d11Result) {
		return;
	}
	createDxgiFactory(d3d11Device, &dxgiFactory);
	createSwapChain(hwnd, d3d11Device, dxgiFactory, &d3d11SwapChain);
	createRenderTarget(d3d11Device, d3d11SwapChain, &d3d11FramebufferView);
}

ID3D11Device1* application::D3D11::getDevice() {
	return d3d11Device;
}

ID3D11DeviceContext1* application::D3D11::getDeviceContext() {
	return d3d11DeviceContext;
}

IDXGISwapChain1* application::D3D11::getSwapChain() {
	return d3d11SwapChain;
}

ID3D11RenderTargetView* application::D3D11::getFramebuffer() {
	return d3d11FramebufferView;
}

bool application::D3D11::createD3D11Device(ID3D11Device1** deviceOut, ID3D11DeviceContext1** deviceContextOut) {
	ID3D11Device* baseDevice;
	ID3D11DeviceContext* baseDeviceContext;
	D3D_FEATURE_LEVEL featureLevels[] = {D3D_FEATURE_LEVEL_11_0};
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

	HRESULT hResult = D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE,
										0, creationFlags,
										featureLevels, ARRAYSIZE(featureLevels),
										D3D11_SDK_VERSION, &baseDevice,
										0, &baseDeviceContext);
	if (FAILED(hResult)) {
		return false;
	}

	hResult = baseDevice->QueryInterface(__uuidof(ID3D11Device1), (void**)deviceOut);
	assert(SUCCEEDED(hResult));
	baseDevice->Release();

	hResult = baseDeviceContext->QueryInterface(__uuidof(ID3D11DeviceContext1), (void**)deviceContextOut);
	assert(SUCCEEDED(hResult));
	baseDeviceContext->Release();

	return true;
}

void application::D3D11::createDxgiFactory(ID3D11Device1* device, IDXGIFactory2** factoryOut) {
	IDXGIDevice1* dxgiDevice;
	HRESULT hResult = device->QueryInterface(__uuidof(IDXGIDevice1), (void**)&dxgiDevice);
	assert(SUCCEEDED(hResult));

	IDXGIAdapter* dxgiAdapter;
	hResult = dxgiDevice->GetAdapter(&dxgiAdapter);
	assert(SUCCEEDED(hResult));
	dxgiDevice->Release();

	DXGI_ADAPTER_DESC adapterDesc;
	dxgiAdapter->GetDesc(&adapterDesc);

	OutputDebugStringA("Graphics Device: ");
	OutputDebugStringW(adapterDesc.Description);

	hResult = dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), (void**)factoryOut);
	assert(SUCCEEDED(hResult));
	dxgiAdapter->Release();
}

void application::D3D11::createSwapChain(HWND hwnd, ID3D11Device1* device, IDXGIFactory2* factory, IDXGISwapChain1** swapChainOut) {
	DXGI_SWAP_CHAIN_DESC1 d3d11SwapChainDesc = {};
	d3d11SwapChainDesc.Width = 0;	// use window width
	d3d11SwapChainDesc.Height = 0;	// use window height
	// Use UNORM (not _SRGB) for flip model compatibility
	d3d11SwapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	d3d11SwapChainDesc.SampleDesc.Count = 1;
	d3d11SwapChainDesc.SampleDesc.Quality = 0;
	d3d11SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	// Flip model requires BufferCount >= 2
	d3d11SwapChainDesc.BufferCount = 2;
	d3d11SwapChainDesc.Scaling = DXGI_SCALING_STRETCH;
	// Use flip sequential for modern presentation (required for tearing support)
	d3d11SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	d3d11SwapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	// Enable allow-tearing so Present can present without vsync when supported
	d3d11SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

	HRESULT hResult = factory->CreateSwapChainForHwnd(device, hwnd, &d3d11SwapChainDesc, 0, 0, swapChainOut);
	assert(SUCCEEDED(hResult));
}

void application::D3D11::createRenderTarget(ID3D11Device1* device, IDXGISwapChain1* swapChain, ID3D11RenderTargetView** FramebufferViewOut) {
	ID3D11Texture2D* d3d11Framebuffer;
	HRESULT hResult = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&d3d11Framebuffer);
	assert(SUCCEEDED(hResult));

	hResult = device->CreateRenderTargetView(d3d11Framebuffer, 0, FramebufferViewOut);
	assert(SUCCEEDED(hResult));
	d3d11Framebuffer->Release();
}
