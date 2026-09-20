#include <algorithm>
#include <cmath>

#include "ShadowRasterization.hh"

using namespace kke::benchmark;
using Microsoft::WRL::ComPtr;

void ShadowRasterization::configure(float scale) {
	rasterScale = scale;
	generatedBytes = 0;
	generatedCount = 0;
}

float ShadowRasterization::scale() {
	return rasterScale;
}

ComPtr<ID2D1Bitmap1> ShadowRasterization::create(
	D2dEngineContext& context, ComPtr<ID2D1Image> const& image, D2D1_RECT_F const& bounds
) {
	auto* deviceContext = context.getD2dContext()->getDeviceContext();
	D2D1_SIZE_U pixelSize{
		static_cast<UINT32>((std::max)(1.0f, std::ceil((bounds.right - bounds.left) * rasterScale))
		),
		static_cast<UINT32>((std::max)(1.0f, std::ceil((bounds.bottom - bounds.top) * rasterScale)))
	};
	auto properties = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
		96.0f * rasterScale,
		96.0f * rasterScale
	);
	ComPtr<ID2D1Bitmap1> bitmap;
	if (FAILED(deviceContext->CreateBitmap(pixelSize, nullptr, 0, properties, &bitmap))) {
		return nullptr;
	}
	ComPtr<ID2D1Image> previousTarget;
	D2D1_MATRIX_3X2_F previousTransform;
	deviceContext->GetTarget(&previousTarget);
	deviceContext->GetTransform(&previousTransform);
	deviceContext->SetTarget(bitmap.Get());
	deviceContext->SetTransform(D2D1::Matrix3x2F::Scale(rasterScale, rasterScale));
	deviceContext->Clear();
	deviceContext->DrawImage(image.Get(), {-bounds.left, -bounds.top});
	HRESULT result = deviceContext->Flush();
	deviceContext->SetTarget(previousTarget.Get());
	deviceContext->SetTransform(previousTransform);
	if (FAILED(result)) {
		return nullptr;
	}
	return bitmap;
}

void ShadowRasterization::recordBitmap(ID2D1Bitmap1* bitmap) {
	auto size = bitmap->GetPixelSize();
	generatedBytes += std::size_t(size.width) * size.height * 4;
	++generatedCount;
}

std::size_t ShadowRasterization::bitmapBytes() {
	return generatedBytes;
}

std::size_t ShadowRasterization::bitmapCount() {
	return generatedCount;
}
