#include "D2dEntrance.hh"

#include <utility>

#include <d2d1.h>
#include <dxgi1_2.h>

#include "kke/engine/d2d/D2dEngine.hh"
#include "kke/engine/d2d/context/D2dContext.hh"

#include "PageController.hh"

using Microsoft::WRL::ComPtr;

namespace kke::example::overlay {
D2dEntrance::D2dEntrance() = default;

D2dEntrance::~D2dEntrance() = default;

void D2dEntrance::renderFrame(IDXGISwapChain* swapChain) {
	std::scoped_lock lock(renderMutex);
	if (swapChain == nullptr) {
		return;
	}

	ComPtr<ID3D11Device> currentDevice;
	if (FAILED(swapChain->GetDevice(IID_PPV_ARGS(&currentDevice)))) {
		return;
	}

	ComPtr<ID3D11DeviceContext> currentContext;
	currentDevice->GetImmediateContext(currentContext.GetAddressOf());
	if (!currentContext) {
		return;
	}

	if (d3d11Device.Get() != currentDevice.Get()) {
		releaseDevices();
		if (!initialize(currentDevice.Get())) {
			return;
		}
	}

	if (!ensureRenderTarget(swapChain)) {
		return;
	}

	ComPtr<ID3DDeviceContextState> previousState = contextIsolation.activate(currentContext.Get());
	if (!previousState) {
		return;
	}

	kke::D2dContext drawContext(d2dFactory.Get(), d2dDeviceContext.Get());
	engine->beginDraw(drawContext, renderTarget.Get());
	pageController->render(engine->getViewportSize());
	engine->endDraw();

	contextIsolation.restore(currentContext.Get(), previousState);
}

void D2dEntrance::releaseBackBuffer() {
	std::scoped_lock lock(renderMutex);
	clearRenderTarget();
}

void D2dEntrance::shutdown() {
	std::scoped_lock lock(renderMutex);
	releaseDevices();
}

bool D2dEntrance::initialize(ID3D11Device* device) {
	d3d11Device = device;
	if (!createRenderDevices(device)) {
		releaseDevices();
		return false;
	}

	if (!contextIsolation.initialize(device)) {
		releaseDevices();
		return false;
	}
	engine = std::make_unique<kke::D2dEngine>();
	pageController = std::make_unique<PageController>(*engine);
	pageController->initialize();
	return true;
}

bool D2dEntrance::createRenderDevices(ID3D11Device* device) {
	D2D1_FACTORY_OPTIONS factoryOptions{D2D1_DEBUG_LEVEL_NONE};
	HRESULT factoryResult = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_MULTI_THREADED,
		__uuidof(ID2D1Factory3),
		&factoryOptions,
		reinterpret_cast<void**>(d2dFactory.GetAddressOf())
	);
	if (FAILED(factoryResult)) {
		return false;
	}

	ComPtr<IDXGIDevice> dxgiDevice;
	if (FAILED(device->QueryInterface(IID_PPV_ARGS(&dxgiDevice))) ||
		FAILED(d2dFactory->CreateDevice(dxgiDevice.Get(), d2dDevice.GetAddressOf()))) {
		return false;
	}

	return SUCCEEDED(d2dDevice->CreateDeviceContext(
		D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
		d2dDeviceContext.GetAddressOf()
	));
}

bool D2dEntrance::ensureRenderTarget(IDXGISwapChain* swapChain) {
	if (renderTarget && renderTargetSwapChain == swapChain) {
		return true;
	}

	clearRenderTarget();
	return createRenderTarget(swapChain);
}

bool D2dEntrance::createRenderTarget(IDXGISwapChain* swapChain) {
	if (!d2dDeviceContext) {
		return false;
	}

	ComPtr<IDXGISurface> surface;
	if (FAILED(swapChain->GetBuffer(0, IID_PPV_ARGS(&surface)))) {
		return false;
	}

	D2D1_BITMAP_PROPERTIES1 properties = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_IGNORE),
		96.0f,
		96.0f
	);
	if (FAILED(d2dDeviceContext->CreateBitmapFromDxgiSurface(
			surface.Get(),
			&properties,
			renderTarget.GetAddressOf()
		))) {
		return false;
	}

	renderTargetSwapChain = swapChain;
	return true;
}

void D2dEntrance::clearRenderTarget() {
	if (d2dDeviceContext) {
		d2dDeviceContext->SetTarget(nullptr);
	}
	renderTarget = nullptr;
	renderTargetSwapChain = nullptr;
}

void D2dEntrance::releaseDevices() {
	clearRenderTarget();
	pageController.reset();
	engine.reset();
	contextIsolation.release();
	d2dDeviceContext = nullptr;
	d2dDevice = nullptr;
	d2dFactory = nullptr;
	d3d11Device = nullptr;
}
} // namespace kke::example::overlay
