#pragma once

#include <cstdint>
#include <cstring>
#include <vector>

#include <d3d11.h>

#include "kke/engine/d2d/D2dEngine.hh"
#include "kke/engine/d2d/context/D2dContext.hh"

namespace kke::test {
class D2dTestTarget {
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID2D1Factory1> factory;
	Microsoft::WRL::ComPtr<ID2D1Device> d2dDevice;
	Microsoft::WRL::ComPtr<ID2D1DeviceContext> context;
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> target;
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> readback;
	D2dEngine engine;

  public:
	bool initialize() {
		HRESULT deviceResult = D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			D3D11_CREATE_DEVICE_BGRA_SUPPORT,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&device,
			nullptr,
			nullptr
		);
		if (FAILED(deviceResult)) {
			deviceResult = D3D11CreateDevice(
				nullptr,
				D3D_DRIVER_TYPE_WARP,
				nullptr,
				D3D11_CREATE_DEVICE_BGRA_SUPPORT,
				nullptr,
				0,
				D3D11_SDK_VERSION,
				&device,
				nullptr,
				nullptr
			);
		}
		if (FAILED(deviceResult)) {
			return false;
		}
		Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
		D2D1_FACTORY_OPTIONS options{};
		if (FAILED(device.As(&dxgiDevice)) ||
			FAILED(D2D1CreateFactory(
				D2D1_FACTORY_TYPE_SINGLE_THREADED,
				__uuidof(ID2D1Factory1),
				&options,
				reinterpret_cast<void**>(factory.GetAddressOf())
			)) ||
			FAILED(factory->CreateDevice(dxgiDevice.Get(), &d2dDevice)) ||
			FAILED(d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &context))) {
			return false;
		}
		auto format = D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED);
		auto properties = D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET, format, 96, 96);
		if (FAILED(context->CreateBitmap({256, 256}, nullptr, 0, properties, &target))) {
			return false;
		}
		properties.bitmapOptions = D2D1_BITMAP_OPTIONS_CPU_READ | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
		return SUCCEEDED(context->CreateBitmap({256, 256}, nullptr, 0, properties, &readback));
	}

	D2dEngine& begin() {
		engine.beginDraw(D2dContext(factory.Get(), context.Get()), target.Get());
		engine.clear();
		return engine;
	}

	D2dContext getContext() const {
		return D2dContext(factory.Get(), context.Get());
	}

	std::vector<uint8_t> finish() {
		engine.endDraw();
		if (FAILED(readback->CopyFromBitmap(nullptr, target.Get(), nullptr))) {
			return {};
		}
		D2D1_MAPPED_RECT mapped{};
		if (FAILED(readback->Map(D2D1_MAP_OPTIONS_READ, &mapped))) {
			return {};
		}
		std::vector<uint8_t> pixels(256 * 256 * 4);
		for (size_t y = 0; y < 256; ++y) {
			std::memcpy(pixels.data() + y * 256 * 4, mapped.bits + y * mapped.pitch, 256 * 4);
		}
		readback->Unmap();
		return pixels;
	}

	D2dLayerStatistics const& statistics() const {
		return engine.getLayerStatistics();
	}
};
} // namespace kke::test
