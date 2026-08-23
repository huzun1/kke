#include "ShadowEffectRenderer.hh"

#include "kke/appearance/ColorQuantizer.hh"
#include "kke/utils/Hasher.hh"

using namespace kke;
using Microsoft::WRL::ComPtr;

PositionIndependentEffectCache::RenderResult ShadowEffectRenderer::render(
	D2dEngineContext& context,
	EffectSource const& source,
	EffectSourceAppearance const& sourceAppearance,
	ShadowEffect const& effect,
	std::optional<EffectClipSource> const& clip
) {
	if (cache.supports(source)) {
		return cache.render(
			context,
			source,
			sourceAppearance,
			hashEffect(effect),
			clip,
			[&](D2dEngineContext& renderContext, ComPtr<ID2D1Image> sourceImage) {
				return render(renderContext, sourceImage, effect);
			}
		);
	}

	std::shared_ptr<D2dCanvas> sourceCanvas =
		sourceRenderer.render(context, source, sourceAppearance);
	if (!sourceCanvas) {
		return {nullptr, {0.0f, 0.0f}};
	}

	return {render(context, sourceCanvas->getCommandList(), effect), {0.0f, 0.0f}};
}

ComPtr<ID2D1Image> ShadowEffectRenderer::render(
	D2dEngineContext& context, ComPtr<ID2D1Image> sourceImage, ShadowEffect const& effect
) const {
	ComPtr<ID2D1Effect> shadowEffect = createShadowEffect(context, sourceImage, effect);
	if (!shadowEffect) {
		return nullptr;
	}

	ComPtr<ID2D1Effect> offsetEffect = createOffsetEffect(context, shadowEffect.Get(), effect);
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
		return createOuterShadowImage(context, offsetEffect.Get(), sourceImage);
	case ShadowMode::ShadowWithSource:
	default:
		break;
	}

	ComPtr<ID2D1Image> outerShadowImage =
		createOuterShadowImage(context, offsetEffect.Get(), sourceImage);
	if (!outerShadowImage) {
		return nullptr;
	}

	ID2D1DeviceContext* deviceContext = context.getD2dContext()->getDeviceContext();
	ComPtr<ID2D1Effect> compositeEffect = context.getResourceProviders()->getEffectPool()->acquire(
		deviceContext,
		CLSID_D2D1Composite
	);
	if (!compositeEffect) {
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
	hasher.combine(ColorQuantizer::quantizeComponent(effect.color.r));
	hasher.combine(ColorQuantizer::quantizeComponent(effect.color.g));
	hasher.combine(ColorQuantizer::quantizeComponent(effect.color.b));
	hasher.combine(ColorQuantizer::quantizeComponent(effect.color.a));
	hasher.combine(effect.mode);
	return hasher.get();
}

ComPtr<ID2D1Effect> ShadowEffectRenderer::createShadowEffect(
	D2dEngineContext& context, ComPtr<ID2D1Image> sourceImage, ShadowEffect const& effect
) const {
	ID2D1DeviceContext* deviceContext = context.getD2dContext()->getDeviceContext();
	ComPtr<ID2D1Effect> shadowEffect =
		context.getResourceProviders()->getEffectPool()->acquire(deviceContext, CLSID_D2D1Shadow);
	if (!shadowEffect) {
		return nullptr;
	}

	shadowEffect->SetInput(0, sourceImage.Get());
	Color quantizedColor = ColorQuantizer::quantize(effect.color);
	shadowEffect->SetValue(D2D1_SHADOW_PROP_BLUR_STANDARD_DEVIATION, effect.blurStandardDeviation);
	shadowEffect->SetValue(
		D2D1_SHADOW_PROP_COLOR,
		D2D1::Vector4F(quantizedColor.r, quantizedColor.g, quantizedColor.b, quantizedColor.a)
	);
	return shadowEffect;
}

ComPtr<ID2D1Effect> ShadowEffectRenderer::createOffsetEffect(
	D2dEngineContext& context, ID2D1Effect* shadowEffect, ShadowEffect const& effect
) const {
	ID2D1DeviceContext* deviceContext = context.getD2dContext()->getDeviceContext();
	ComPtr<ID2D1Effect> offsetEffect = context.getResourceProviders()->getEffectPool()->acquire(
		deviceContext,
		CLSID_D2D12DAffineTransform
	);
	if (!offsetEffect) {
		return nullptr;
	}

	offsetEffect->SetInputEffect(0, shadowEffect);
	offsetEffect->SetValue(
		D2D1_2DAFFINETRANSFORM_PROP_TRANSFORM_MATRIX,
		D2D1::Matrix3x2F::Translation(effect.offset.x, effect.offset.y)
	);
	return offsetEffect;
}

ComPtr<ID2D1Image> ShadowEffectRenderer::createOuterShadowImage(
	D2dEngineContext& context, ID2D1Effect* offsetEffect, ComPtr<ID2D1Image> sourceImage
) const {
	ID2D1DeviceContext* deviceContext = context.getD2dContext()->getDeviceContext();
	ComPtr<ID2D1Effect> compositeEffect = context.getResourceProviders()->getEffectPool()->acquire(
		deviceContext,
		CLSID_D2D1Composite
	);
	if (!compositeEffect) {
		return nullptr;
	}

	compositeEffect->SetInputEffect(0, offsetEffect);
	compositeEffect->SetInput(1, sourceImage.Get());
	compositeEffect->SetValue(D2D1_COMPOSITE_PROP_MODE, D2D1_COMPOSITE_MODE_DESTINATION_OUT);

	ComPtr<ID2D1Image> outputImage;
	compositeEffect->GetOutput(&outputImage);
	return outputImage;
}
