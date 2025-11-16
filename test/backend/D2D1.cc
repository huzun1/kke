#include "D2D1.hpp"

#include <d2d1_1.h>
#include <d3d11_1.h>
#include <dxgi1_2.h>

#include "D3D11.hpp"

void application::D2D1::init(D3D11& context) {
	ID3D11Device1* d11Device = context.getDevice();
	IDXGISwapChain1* swapChain = context.getSwapChain();

	createFactory(&d2d1Factory);
	createDevice(d11Device, &d2d1Device);
	createDeviceContext(&d2d1DeviceContext);
	createAndSetRenderTarget(swapChain, d2d1DeviceContext, &renderTarget);
}

ID2D1Factory* application::D2D1::getFactory() {
    return d2d1Factory;
}

ID2D1DeviceContext* application::D2D1::getDeviceContext() {
	return d2d1DeviceContext;
}

ID2D1Bitmap1* application::D2D1::getRenderTarget() {
	return renderTarget;
}

void application::D2D1::createFactory(ID2D1Factory3** factoryOut) {
	D2D1_FACTORY_OPTIONS factoryOptions = {D2D1_DEBUG_LEVEL_NONE};
	if (FAILED(D2D1CreateFactory(
			D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory3),
			&factoryOptions, (void**)factoryOut))) {
		return;
	}
}

void application::D2D1::createDevice(ID3D11Device1* d11Device, ID2D1Device** deviceOut) {
	IDXGIDevice* dxgiDevice;
	if (FAILED(d11Device->QueryInterface(IID_PPV_ARGS(&dxgiDevice)))) {
		return;
	}

	if (FAILED(d2d1Factory->CreateDevice(dxgiDevice, deviceOut))) {
		return;
	}
}

void application::D2D1::createDeviceContext(ID2D1DeviceContext** deviceContextOut) {
	D2D1_DEVICE_CONTEXT_OPTIONS deviceOptions = D2D1_DEVICE_CONTEXT_OPTIONS_NONE;
	if (FAILED(d2d1Device->CreateDeviceContext(deviceOptions, deviceContextOut))) {
		return;
	}
}

void application::D2D1::createAndSetRenderTarget(IDXGISwapChain1* swapChain, ID2D1DeviceContext* deviceContext, ID2D1Bitmap1** renderTargetOut) {
	IDXGISurface2* surface;
	if (FAILED(swapChain->GetBuffer(0, IID_PPV_ARGS(&surface)))) {
		return;
	}

	D2D1_BITMAP_PROPERTIES1 props = ::D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		::D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED), 96.0, 96.0);
	if (FAILED(deviceContext->CreateBitmapFromDxgiSurface(surface, props, renderTargetOut))) {
		return;
	}
}
