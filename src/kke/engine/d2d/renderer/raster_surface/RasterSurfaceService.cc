#include "RasterSurfaceService.hh"

#include <cmath>
#include <limits>

#include "kke/engine/d2d/resource/raster_surface/D2dRasterSurface.hh"
#include "kke/utils/DebugLog.hh"

using namespace kke;
using Microsoft::WRL::ComPtr;

std::shared_ptr<D2dRasterSurface> RasterSurfaceService::create(
	D2dEngineContext const& context, Scale const& logicalSize, float rasterScale
) {
	if (!std::isfinite(logicalSize.x) || !std::isfinite(logicalSize.y) ||
		!std::isfinite(rasterScale) || logicalSize.x <= 0.0f || logicalSize.y <= 0.0f ||
		rasterScale <= 0.0f) {
		return nullptr;
	}

	double pixelWidth = std::ceil(logicalSize.x * rasterScale);
	double pixelHeight = std::ceil(logicalSize.y * rasterScale);
	if (pixelWidth > (std::numeric_limits<UINT32>::max)() ||
		pixelHeight > (std::numeric_limits<UINT32>::max)()) {
		return nullptr;
	}

	D2D1_BITMAP_PROPERTIES1 properties = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
		96.0f * rasterScale,
		96.0f * rasterScale
	);
	ComPtr<ID2D1Bitmap1> bitmap;
	HRESULT result = context.getD2dContext()->getDeviceContext()->CreateBitmap(
		{static_cast<UINT32>(pixelWidth), static_cast<UINT32>(pixelHeight)},
		nullptr,
		0,
		&properties,
		&bitmap
	);
	if (FAILED(result)) {
		kke::debug::log(
			"[kke][RasterSurfaceService] CreateBitmap failed: 0x%08x",
			static_cast<unsigned int>(result)
		);
		return nullptr;
	}

	return std::make_shared<D2dRasterSurface>(bitmap);
}

bool RasterSurfaceService::begin(
	D2dEngineContext const& context, std::shared_ptr<RasterSurface> const& surface
) {
	auto d2dSurface = std::dynamic_pointer_cast<D2dRasterSurface>(surface);
	if (d2dSurface == nullptr || d2dSurface->getBitmap() == nullptr) {
		kke::debug::log("[kke][RasterSurfaceService] begin received an invalid raster surface");
		return false;
	}

	pushCurrentRenderTarget(context);
	ID2D1DeviceContext* deviceContext = context.getD2dContext()->getDeviceContext();
	float surfaceDpiX = 96.0f;
	float surfaceDpiY = 96.0f;
	d2dSurface->getBitmap()->GetDpi(&surfaceDpiX, &surfaceDpiY);
	deviceContext->SetTarget(d2dSurface->getBitmap().Get());
	deviceContext->SetDpi(surfaceDpiX, surfaceDpiY);
	deviceContext->SetTransform(D2D1::Matrix3x2F::Identity());
	deviceContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
	deviceContext->Clear(D2D1::ColorF(0.0f, 0.0f));
	return true;
}

bool RasterSurfaceService::end(D2dEngineContext const& context) {
	if (renderTargetStates.empty()) {
		kke::debug::log("[kke][RasterSurfaceService] end called with an empty target stack");
		return false;
	}

	RenderTargetState state = renderTargetStates.top();
	renderTargetStates.pop();
	ID2D1DeviceContext* deviceContext = context.getD2dContext()->getDeviceContext();
	deviceContext->SetTarget(state.target.Get());
	deviceContext->SetDpi(state.dpiX, state.dpiY);
	deviceContext->SetTransform(state.transform);
	deviceContext->SetTextAntialiasMode(state.textAntialiasMode);
	return true;
}

void RasterSurfaceService::draw(
	D2dEngineContext const& context,
	std::shared_ptr<RasterSurface> const& surface,
	Rect const& destination,
	float opacity
) {
	auto d2dSurface = std::dynamic_pointer_cast<D2dRasterSurface>(surface);
	if (d2dSurface == nullptr || d2dSurface->getBitmap() == nullptr) {
		kke::debug::log("[kke][RasterSurfaceService] draw received an invalid raster surface");
		return;
	}

	context.getD2dContext()->getDeviceContext()->DrawBitmap(
		d2dSurface->getBitmap().Get(),
		{destination.min.x, destination.min.y, destination.max.x, destination.max.y},
		opacity,
		D2D1_INTERPOLATION_MODE_LINEAR
	);
}

void RasterSurfaceService::pushCurrentRenderTarget(D2dEngineContext const& context) {
	ID2D1DeviceContext* deviceContext = context.getD2dContext()->getDeviceContext();
	ComPtr<ID2D1Image> target;
	D2D1_MATRIX_3X2_F transform;
	float dpiX = 96.0f;
	float dpiY = 96.0f;
	deviceContext->GetTarget(&target);
	deviceContext->GetTransform(&transform);
	deviceContext->GetDpi(&dpiX, &dpiY);
	renderTargetStates.push({
		.target = target,
		.transform = transform,
		.textAntialiasMode = deviceContext->GetTextAntialiasMode(),
		.dpiX = dpiX,
		.dpiY = dpiY,
	});
}
