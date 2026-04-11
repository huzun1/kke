#include <d2d1.h>
#include <d2d1_1.h>
#include <d2d1helper.h>
#include <wincodec.h>
#include <wrl/client.h>

#include <kke/ShadowDispatcher.hh>

#include "kke/common/Scale.hh"
#include "kke/effect/EffectContainer.hh"
#include "kke/effect/EffectInstance.hh"
#include "kke/effect/impl/BlurEffect.hh"

kke::ShadowDisaptcher::ShadowDisaptcher(ID2D1DeviceContext* context, kke::ResourceAllocator* resourceAllocator, kke::EffectContainer* container)
	: deviceContext(context), resourceAllocator(resourceAllocator), effectContainer(container) {
}

void kke::ShadowDisaptcher::dispatch(kke::Rect const& dimension, float deviation,
									 std::function<void()> drawFunc, ID2D1Image** output) {
	const kke::Scale2f geometryScale = {dimension.x2 - dimension.x1, dimension.y2 - dimension.y1};

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
	drawFunc();

	// TODO: Remove this flush when possible
	deviceContext->Flush();	 // To apply current commands to the render target

	//// Draw the effect image into the bitmap
	std::shared_ptr<BlurEffect> blurEffect = effectContainer->acquireOrCreateEffect<BlurEffect>();
	blurEffect->setDeviation(deviation);

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
	EffectInstance* blurEffectInstance = resourceAllocator->acquireOrCreateEffect(blurEffect);
	blurEffectInstance->lock();
	ComPtr<ID2D1Effect> d2d1Effect = blurEffectInstance->getD2D1Effect();
	d2d1Effect->SetInput(0, shadowRenderTarget);
	blurEffect->setProperties(d2d1Effect);
	deviceContext->DrawImage(d2d1Effect.Get());

	deviceContext->SetTarget(previousTarget);
	previousTarget->Release();	// prevent resource leaking
	shadowRenderTarget->Release();

	*output = finalOutputRenderTarget;
}

kke::Point2f kke::ShadowDisaptcher::computeOffset(kke::Rect const& dimension) const {
	return {dimension.x1 - bufferPad / 2.0f, dimension.y1 - bufferPad / 2.0f};
}
