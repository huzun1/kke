#include "ShadowEffectRenderer.hh"

#include "kke/utils/Hasher.hh"

using namespace kke;
using Microsoft::WRL::ComPtr;

PositionIndependentEffectCache::RenderResult ShadowEffectRenderer::render(
	D2dEngineContext& context,
	EffectSource const& source,
	EffectSourceAppearance const& sourceAppearance,
	ShadowEffect const& effect,
	std::optional<EffectClipSource> const& clip) {
	if (cache.supports(source)) {
		return cache.render(
			context,
			source,
			sourceAppearance,
			hashEffect(effect),
			clip,
			[&](D2dEngineContext& renderContext, ComPtr<ID2D1Image> sourceImage) {
				return render(renderContext, sourceImage, effect);
			});
	}

	std::shared_ptr<D2dCanvas> sourceCanvas = sourceRenderer.render(context, source, sourceAppearance);
	if (!sourceCanvas) {
		return {nullptr, {0.0f, 0.0f}};
	}

	return {render(context, sourceCanvas->getCommandList(), effect), {0.0f, 0.0f}};
}

ComPtr<ID2D1Image> ShadowEffectRenderer::render(
	D2dEngineContext& context,
	ComPtr<ID2D1Image> sourceImage,
	ShadowEffect const& effect) const {
	ID2D1DeviceContext* deviceContext = context.getD2dContext()->getDeviceContext();

	ComPtr<ID2D1Effect> shadowEffect = createShadowEffect(deviceContext, sourceImage, effect);
	if (!shadowEffect) {
		return nullptr;
	}

	ComPtr<ID2D1Effect> offsetEffect = createOffsetEffect(deviceContext, shadowEffect.Get(), effect);
	if (!offsetEffect) {
		return nullptr;
	}

	switch (effect.mode) {
	case ShadowMode::ShadowOnly: {
		ComPtr<ID2D1Image> outputImage;
		offsetEffect->GetOutput(&outputImage);
		return outputImage;
	}
	case ShadowMode::OuterShadowOnly:
		return createOuterShadowImage(deviceContext, offsetEffect.Get(), sourceImage);
	case ShadowMode::ShadowWithSource:
	default:
		break;
	}

	ComPtr<ID2D1Image> outerShadowImage = createOuterShadowImage(deviceContext, offsetEffect.Get(), sourceImage);
	if (!outerShadowImage) {
		return nullptr;
	}

	ComPtr<ID2D1Effect> compositeEffect;
	HRESULT compositeResult = deviceContext->CreateEffect(CLSID_D2D1Composite, &compositeEffect);
	if (FAILED(compositeResult) || !compositeEffect) {
		return nullptr;
	}

	compositeEffect->SetInput(0, outerShadowImage.Get());
	compositeEffect->SetInput(1, sourceImage.Get());

	ComPtr<ID2D1Image> outputImage;
	compositeEffect->GetOutput(&outputImage);
	return outputImage;
}

uint64_t ShadowEffectRenderer::hashEffect(ShadowEffect const& effect) {
	Hasher hasher;
	hasher.combine(effect.offset.x);
	hasher.combine(effect.offset.y);
	hasher.combine(effect.blurStandardDeviation);
	hasher.combine(effect.color.r);
	hasher.combine(effect.color.g);
	hasher.combine(effect.color.b);
	hasher.combine(effect.color.a);
	hasher.combine(effect.mode);
	return hasher.get();
}

ComPtr<ID2D1Effect> ShadowEffectRenderer::createShadowEffect(
	ID2D1DeviceContext* deviceContext,
	ComPtr<ID2D1Image> sourceImage,
	ShadowEffect const& effect) const {
	ComPtr<ID2D1Effect> shadowEffect;
	HRESULT shadowResult = deviceContext->CreateEffect(CLSID_D2D1Shadow, &shadowEffect);
	if (FAILED(shadowResult) || !shadowEffect) {
		return nullptr;
	}

	shadowEffect->SetInput(0, sourceImage.Get());
	shadowEffect->SetValue(D2D1_SHADOW_PROP_BLUR_STANDARD_DEVIATION, effect.blurStandardDeviation);
	shadowEffect->SetValue(D2D1_SHADOW_PROP_COLOR, D2D1::Vector4F(effect.color.r, effect.color.g, effect.color.b, effect.color.a));
	return shadowEffect;
}

ComPtr<ID2D1Effect> ShadowEffectRenderer::createOffsetEffect(
	ID2D1DeviceContext* deviceContext,
	ID2D1Effect* shadowEffect,
	ShadowEffect const& effect) const {
	ComPtr<ID2D1Effect> offsetEffect;
	HRESULT offsetResult = deviceContext->CreateEffect(CLSID_D2D12DAffineTransform, &offsetEffect);
	if (FAILED(offsetResult) || !offsetEffect) {
		return nullptr;
	}

	offsetEffect->SetInputEffect(0, shadowEffect);
	offsetEffect->SetValue(D2D1_2DAFFINETRANSFORM_PROP_TRANSFORM_MATRIX, D2D1::Matrix3x2F::Translation(effect.offset.x, effect.offset.y));
	return offsetEffect;
}

ComPtr<ID2D1Image> ShadowEffectRenderer::createOuterShadowImage(
	ID2D1DeviceContext* deviceContext,
	ID2D1Effect* offsetEffect,
	ComPtr<ID2D1Image> sourceImage) const {
	ComPtr<ID2D1Effect> compositeEffect;
	HRESULT compositeResult = deviceContext->CreateEffect(CLSID_D2D1Composite, &compositeEffect);
	if (FAILED(compositeResult) || !compositeEffect) {
		return nullptr;
	}

	compositeEffect->SetInputEffect(0, offsetEffect);
	compositeEffect->SetInput(1, sourceImage.Get());
	compositeEffect->SetValue(D2D1_COMPOSITE_PROP_MODE, D2D1_COMPOSITE_MODE_DESTINATION_OUT);

	ComPtr<ID2D1Image> outputImage;
	compositeEffect->GetOutput(&outputImage);
	return outputImage;
}
