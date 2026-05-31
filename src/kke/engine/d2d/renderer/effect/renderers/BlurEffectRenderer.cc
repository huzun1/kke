#include "BlurEffectRenderer.hh"

#include "kke/utils/Hasher.hh"

using namespace kke;
using Microsoft::WRL::ComPtr;

PositionIndependentEffectCache::RenderResult BlurEffectRenderer::render(
	D2dEngineContext& context,
	EffectSource const& source,
	EffectSourceAppearance const& sourceAppearance,
	BlurEffect const& effect,
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

ComPtr<ID2D1Image> BlurEffectRenderer::render(
	D2dEngineContext& context, ComPtr<ID2D1Image> sourceImage, BlurEffect const& effect
) const {
	ComPtr<ID2D1Effect> blurEffect = createBlurEffect(context, sourceImage, effect);
	if (!blurEffect) {
		return nullptr;
	}

	if (effect.appearance.mode == BlurMode::OuterOnly) {
		ComPtr<ID2D1Effect> compositeEffect;
		HRESULT compositeResult = context.getD2dContext()->getDeviceContext()->CreateEffect(
			CLSID_D2D1Composite,
			&compositeEffect
		);
		if (FAILED(compositeResult) || !compositeEffect) {
			return nullptr;
		}

		compositeEffect->SetInputEffect(0, blurEffect.Get());
		compositeEffect->SetInput(1, sourceImage.Get());
		compositeEffect->SetValue(D2D1_COMPOSITE_PROP_MODE, D2D1_COMPOSITE_MODE_DESTINATION_OUT);

		ComPtr<ID2D1Image> outerBlurImage;
		compositeEffect->GetOutput(&outerBlurImage);
		return outerBlurImage;
	}

	ComPtr<ID2D1Image> outputImage;
	blurEffect->GetOutput(&outputImage);
	return outputImage;
}

ComPtr<ID2D1Effect> BlurEffectRenderer::createBlurEffect(
	D2dEngineContext& context, ComPtr<ID2D1Image> sourceImage, BlurEffect const& effect
) const {
	ComPtr<ID2D1Effect> blurEffect;
	HRESULT result = context.getD2dContext()->getDeviceContext()->CreateEffect(
		CLSID_D2D1GaussianBlur,
		&blurEffect
	);
	if (FAILED(result) || !blurEffect) {
		return nullptr;
	}

	blurEffect->SetInput(0, sourceImage.Get());
	blurEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION, effect.appearance.radius);
	blurEffect->SetValue(
		D2D1_GAUSSIANBLUR_PROP_BORDER_MODE,
		mapBorderMode(effect.appearance.borderMode)
	);
	blurEffect->SetValue(
		D2D1_GAUSSIANBLUR_PROP_OPTIMIZATION,
		mapOptimization(effect.appearance.optimization)
	);
	return blurEffect;
}

D2D1_BORDER_MODE BlurEffectRenderer::mapBorderMode(BlurBorderMode mode) {
	switch (mode) {
	case BlurBorderMode::HARD:
		return D2D1_BORDER_MODE_HARD;
	case BlurBorderMode::SOFT:
	default:
		return D2D1_BORDER_MODE_SOFT;
	}
}

D2D1_GAUSSIANBLUR_OPTIMIZATION BlurEffectRenderer::mapOptimization(BlurOptimization optimization) {
	switch (optimization) {
	case BlurOptimization::SPEED:
		return D2D1_GAUSSIANBLUR_OPTIMIZATION_SPEED;
	case BlurOptimization::QUALITY:
		return D2D1_GAUSSIANBLUR_OPTIMIZATION_QUALITY;
	case BlurOptimization::BALANCED:
	default:
		return D2D1_GAUSSIANBLUR_OPTIMIZATION_BALANCED;
	}
}

uint64_t BlurEffectRenderer::hashEffect(BlurEffect const& effect) {
	Hasher hasher;
	hasher.combine(effect.appearance.radius);
	hasher.combine(effect.appearance.borderMode);
	hasher.combine(effect.appearance.optimization);
	hasher.combine(effect.appearance.mode);
	return hasher.get();
}
