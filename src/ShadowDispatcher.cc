#include <oreik/ShadowDispatcher.hpp>

#include "oreik/common/Scale.hpp"
#include "oreik/effect/EffectContainer.hpp"
#include "oreik/effect/impl/BlurEffect.hpp"

oreik::ShadowDisaptcher::ShadowDisaptcher(ID2D1DeviceContext* context, oreik::EffectContainer* container)
	: deviceContext(context), effectContainer(container) {
}

oreik::ShadowDisaptcherResult oreik::ShadowDisaptcher::dispatch(oreik::Rect const& dimension, float deviation, std::function<void(oreik::Point2f const& start)> drawFunc) {
	constexpr float bufferPad = 20.0f;
	const oreik::Scale2f geometryScale = {dimension.x2 - dimension.x1, dimension.y2 - dimension.y1};
	const oreik::Point2f shadowRenderOffset = {bufferPad / 2.0f, bufferPad / 2.0f};

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

	deviceContext->SetTarget(shadowRenderTarget);
	deviceContext->Clear();
	deviceContext->SetTransform(D2D1::Matrix3x2F::Translation(-dimension.x1 + bufferPad / 2.0f, -dimension.y1 + bufferPad / 2.0f));
	drawFunc(shadowRenderOffset);
	deviceContext->Flush();	 // To apply current commands to the render target

	deviceContext->SetTarget(previousTarget);
	previousTarget->Release();

	std::shared_ptr<BlurEffect> blurEffect = effectContainer->acquireOrCreateEffect<BlurEffect>();
	blurEffect->setInput(shadowRenderTarget);
	blurEffect->setDeviation(deviation);
	blurEffect->setProperties();

	Microsoft::WRL::ComPtr<ID2D1Image> output;
	blurEffect->createOutput(&output);

	shadowRenderTarget->Release();

	return {output,
			{dimension.x1 - bufferPad / 2.0f, dimension.y1 - bufferPad / 2.0f}};
}
