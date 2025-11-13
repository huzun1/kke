#include <d2d1_1.h>
#include <d2d1helper.h>
#include <wincodec.h>
#include <wrl/client.h>

#include <kke/ShadowDispatcher.hpp>

#include "kke/common/Scale.hpp"
#include "kke/effect/EffectContainer.hpp"
#include "kke/effect/impl/BlurEffect.hpp"

kke::ShadowDisaptcher::ShadowDisaptcher(ID2D1DeviceContext* context, kke::EffectContainer* container)
	: deviceContext(context), effectContainer(container) {
}

void kke::ShadowDisaptcher::dispatch(kke::Rect const& dimension, float deviation, std::function<void(kke::Point2f const& start)> drawFunc, ID2D1Image** output) {
	const kke::Scale2f geometryScale = {dimension.x2 - dimension.x1, dimension.y2 - dimension.y1};
	const kke::Point2f shadowRenderOffset = {bufferPad / 2.0f, bufferPad / 2.0f};

	ID2D1Bitmap1* shadowRenderTarget;
	float dpiX, dpiY;
	deviceContext->GetDpi(&dpiX, &dpiY);
	D2D1_BITMAP_PROPERTIES1 properties = D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET, deviceContext->GetPixelFormat(), dpiX, dpiY);
	deviceContext->CreateBitmap(
		{static_cast<uint32_t>(geometryScale.x + bufferPad), static_cast<uint32_t>(geometryScale.y + bufferPad)},
		nullptr,
		0,
		properties,
		&shadowRenderTarget);

	ID2D1Image* previousTarget;
	deviceContext->GetTarget(&previousTarget);

	//// Draw the geometry onto the temp buffer
	deviceContext->SetTarget(shadowRenderTarget);
	deviceContext->Clear();
	// This is incredibly confusing, but it's like magic—just draw shapes in drawFunc without worrying about anything, and it works.
	deviceContext->SetTransform(D2D1::Matrix3x2F::Translation(-dimension.x1 + bufferPad / 2.0f, -dimension.y1 + bufferPad / 2.0f));
	drawFunc(shadowRenderOffset);
	deviceContext->Flush();	 // To apply current commands to the render target

	//// Draw the effect image into the bitmap
	std::shared_ptr<BlurEffect> blurEffect = effectContainer->acquireOrCreateEffect<BlurEffect>();
	blurEffect->setInput(shadowRenderTarget);
	blurEffect->setDeviation(deviation);
	blurEffect->setProperties();

	ID2D1Bitmap1* finalOutputRenderTarget;
	deviceContext->CreateBitmap(
		{static_cast<uint32_t>(geometryScale.x + bufferPad), static_cast<uint32_t>(geometryScale.y + bufferPad)},
		nullptr,
		0,
		properties,
		&finalOutputRenderTarget);
	deviceContext->SetTarget(finalOutputRenderTarget);
	deviceContext->SetTransform(D2D1::Matrix3x2F::Identity());	// Reset the matrix
	deviceContext->Clear();
	// Draw the blur output
	deviceContext->DrawImage(blurEffect->output());
	deviceContext->Flush();	 // To apply current commands to the render target

	deviceContext->SetTarget(previousTarget);
	previousTarget->Release();	// prevent resource leaking
	shadowRenderTarget->Release();

	*output = finalOutputRenderTarget;
}

kke::Point2f kke::ShadowDisaptcher::computeOffset(kke::Rect const& dimension) const {
	return {dimension.x1 - bufferPad / 2.0f, dimension.y1 - bufferPad / 2.0f};
}
