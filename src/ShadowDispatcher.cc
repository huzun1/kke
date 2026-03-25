#include <d2d1.h>
#include <d2d1_1.h>
#include <d2d1effects.h>
#include <d2d1helper.h>
#include <wincodec.h>
#include <wrl/client.h>

#include <cstdint>
#include <stdexcept>
#include <memory>

#include <kke/ShadowDispatcher.hh>

#include "internal/HResult.hh"
#include "kke/common/Scale.hh"
#include "kke/effect/EffectInstance.hh"
#include "kke/effect/impl/BlurEffect.hh"

using Microsoft::WRL::ComPtr;
using kke::internal::throwIfFailed;

kke::ShadowDisaptcher::ShadowDisaptcher(ID2D1DeviceContext* context, kke::ResourceAllocator* resourceAllocator)
	: deviceContext(context),
	  resourceAllocator(resourceAllocator) {
}

void kke::ShadowDisaptcher::dispatch(kke::Rect const& dimension, float deviation,
									 std::function<void()> drawFunc, ID2D1Image** output) {
	if (output == nullptr) {
		throw std::invalid_argument("ShadowDisaptcher::dispatch requires a non-null output pointer.");
	}

	const kke::Scale2f geometryScale = {dimension.x2 - dimension.x1, dimension.y2 - dimension.y1};

	float dpiX = 0.0f;
	float dpiY = 0.0f;
	deviceContext->GetDpi(&dpiX, &dpiY);
	const D2D1_BITMAP_PROPERTIES1 properties = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET,
		deviceContext->GetPixelFormat(),
		dpiX,
		dpiY);

	ComPtr<ID2D1Bitmap1> shadowRenderTarget;
	throwIfFailed(
		deviceContext->CreateBitmap(
			{static_cast<uint32_t>(geometryScale.x + bufferPad), static_cast<uint32_t>(geometryScale.y + bufferPad)},
			nullptr,
			0,
			properties,
			shadowRenderTarget.GetAddressOf()),
		"Failed to create shadow render target");

	ComPtr<ID2D1Image> previousTarget;
	deviceContext->GetTarget(previousTarget.GetAddressOf());

	deviceContext->SetTarget(shadowRenderTarget.Get());
	deviceContext->Clear();
	deviceContext->SetTransform(D2D1::Matrix3x2F::Translation(-dimension.x1 + bufferPad / 2.0f, -dimension.y1 + bufferPad / 2.0f));
	drawFunc();
	throwIfFailed(deviceContext->Flush(), "Failed to flush shadow source draw calls");

	auto blurEffect = std::make_shared<kke::BlurEffect>();
	blurEffect->setDeviation(deviation);

	ComPtr<ID2D1Bitmap1> finalOutputRenderTarget;
	throwIfFailed(
		deviceContext->CreateBitmap(
			{static_cast<uint32_t>(geometryScale.x + bufferPad), static_cast<uint32_t>(geometryScale.y + bufferPad)},
			nullptr,
			0,
			properties,
			finalOutputRenderTarget.GetAddressOf()),
		"Failed to create blurred shadow render target");

	deviceContext->SetTarget(finalOutputRenderTarget.Get());
	deviceContext->SetTransform(D2D1::Matrix3x2F::Identity());
	deviceContext->Clear();

	EffectInstance* blurEffectInstance = resourceAllocator->acquireOrCreateEffect(blurEffect);
	blurEffectInstance->lock();
	ComPtr<ID2D1Effect> d2d1Effect = blurEffectInstance->getD2D1Effect();
	d2d1Effect->SetInput(0, shadowRenderTarget.Get());
	blurEffect->setProperties(d2d1Effect);
	deviceContext->DrawImage(d2d1Effect.Get());

	deviceContext->SetTarget(previousTarget.Get());
	finalOutputRenderTarget.CopyTo(output);
}

kke::Point2f kke::ShadowDisaptcher::computeOffset(kke::Rect const& dimension) const {
	return {dimension.x1 - bufferPad / 2.0f, dimension.y1 - bufferPad / 2.0f};
}
