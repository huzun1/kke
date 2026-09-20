#include <chrono>
#include <cstdio>
#include <cstring>

#include "ShadowBenchmarkDevice.hh"

using namespace kke::benchmark;
using Microsoft::WRL::ComPtr;

bool ShadowBenchmarkDevice::initialize() {
	if (FAILED(D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			D3D11_CREATE_DEVICE_BGRA_SUPPORT,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&device,
			nullptr,
			&immediateContext
		))) {
		std::fprintf(stderr, "Hardware D3D11 device required for this benchmark.\n");
		return false;
	}
	ComPtr<IDXGIDevice> dxgiDevice;
	ComPtr<IDXGIAdapter> adapter;
	DXGI_ADAPTER_DESC adapterDescription{};
	D2D1_FACTORY_OPTIONS options{};
	if (FAILED(device.As(&dxgiDevice)) || FAILED(dxgiDevice->GetAdapter(&adapter)) ||
		FAILED(adapter->GetDesc(&adapterDescription)) ||
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
	std::printf(
		"adapter=%ls target=1920x1080 dpi=96 hardware=true\n",
		adapterDescription.Description
	);
	auto properties = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
		96,
		96
	);
	if (FAILED(context->CreateBitmap({1920, 1080}, nullptr, 0, properties, &target))) {
		return false;
	}
	D3D11_QUERY_DESC description{D3D11_QUERY_TIMESTAMP_DISJOINT, 0};
	if (FAILED(device->CreateQuery(&description, &disjoint))) {
		return false;
	}
	description.Query = D3D11_QUERY_TIMESTAMP;
	return SUCCEEDED(device->CreateQuery(&description, &startTimestamp)) &&
		   SUCCEEDED(device->CreateQuery(&description, &endTimestamp));
}

kke::D2dContext ShadowBenchmarkDevice::drawingContext() const {
	return {factory.Get(), context.Get()};
}

ID2D1Bitmap1* ShadowBenchmarkDevice::bitmap() const {
	return target.Get();
}

void ShadowBenchmarkDevice::beginTiming() {
	immediateContext->Begin(disjoint.Get());
	immediateContext->End(startTimestamp.Get());
}

void ShadowBenchmarkDevice::endTiming() {
	immediateContext->End(endTimestamp.Get());
	immediateContext->End(disjoint.Get());
	immediateContext->Flush();
}

std::optional<double> ShadowBenchmarkDevice::resolveMilliseconds() {
	D3D11_QUERY_DATA_TIMESTAMP_DISJOINT interval{};
	UINT64 start = 0;
	UINT64 end = 0;
	if (!waitForQuery(disjoint.Get(), &interval, sizeof(interval)) ||
		!waitForQuery(startTimestamp.Get(), &start, sizeof(start)) ||
		!waitForQuery(endTimestamp.Get(), &end, sizeof(end)) || interval.Disjoint ||
		interval.Frequency == 0 || end < start) {
		return std::nullopt;
	}
	return double(end - start) * 1000.0 / double(interval.Frequency);
}

bool ShadowBenchmarkDevice::hasShadowPixels(bool expected) {
	ComPtr<ID2D1Bitmap1> readback;
	auto properties = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_CPU_READ | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
		96,
		96
	);
	D2D1_RECT_U region{24, 50, 25, 51};
	if (FAILED(context->CreateBitmap({1, 1}, nullptr, 0, properties, &readback)) ||
		FAILED(readback->CopyFromBitmap(nullptr, target.Get(), &region))) {
		return false;
	}
	D2D1_MAPPED_RECT mapped{};
	if (FAILED(readback->Map(D2D1_MAP_OPTIONS_READ, &mapped))) {
		return false;
	}
	bool hasAlpha = mapped.bits[3] != 0;
	readback->Unmap();
	return hasAlpha == expected;
}

std::optional<std::vector<uint8_t>> ShadowBenchmarkDevice::readPixels() {
	ComPtr<ID2D1Bitmap1> readback;
	auto size = target->GetPixelSize();
	auto properties = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_CPU_READ | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		target->GetPixelFormat(),
		96,
		96
	);
	if (FAILED(context->CreateBitmap(size, nullptr, 0, properties, &readback)) ||
		FAILED(readback->CopyFromBitmap(nullptr, target.Get(), nullptr))) {
		return std::nullopt;
	}
	std::vector<uint8_t> pixels(std::size_t(size.width) * size.height * 4);
	D2D1_MAPPED_RECT mapped{};
	if (FAILED(readback->Map(D2D1_MAP_OPTIONS_READ, &mapped))) {
		return std::nullopt;
	}
	for (UINT y = 0; y < size.height; ++y) {
		std::memcpy(
			pixels.data() + std::size_t(y) * size.width * 4,
			mapped.bits + std::size_t(y) * mapped.pitch,
			std::size_t(size.width) * 4
		);
	}
	if (FAILED(readback->Unmap())) {
		return std::nullopt;
	}
	return pixels;
}

bool ShadowBenchmarkDevice::waitForQuery(ID3D11Query* query, void* result, UINT size) {
	auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(5);
	while (std::chrono::steady_clock::now() < deadline) {
		HRESULT status =
			immediateContext->GetData(query, result, size, D3D11_ASYNC_GETDATA_DONOTFLUSH);
		if (status == S_OK) {
			return true;
		}
		if (FAILED(status)) {
			return false;
		}
		SwitchToThread();
	}
	std::fprintf(stderr, "GPU query timed out.\n");
	return false;
}
