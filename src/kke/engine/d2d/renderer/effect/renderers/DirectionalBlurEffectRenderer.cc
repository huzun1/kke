#include "DirectionalBlurEffectRenderer.hh"

using namespace kke;
using Microsoft::WRL::ComPtr;

ComPtr<ID2D1Image> DirectionalBlurEffectRenderer::render(
	D2dEngineContext& context, ComPtr<ID2D1Image> sourceImage, DirectionalBlurEffect const& effect
) const {
	ComPtr<ID2D1Effect> directionalBlurEffect;
	ID2D1DeviceContext* deviceContext = context.getD2dContext()->getDeviceContext();
	directionalBlurEffect = context.getResourceProviders()->getEffectPool()->acquire(
		deviceContext,
		CLSID_D2D1DirectionalBlur
	);
	if (!directionalBlurEffect) {
		return nullptr;
	}

	directionalBlurEffect->SetInput(0, sourceImage.Get());
	directionalBlurEffect->SetValue(
		D2D1_DIRECTIONALBLUR_PROP_STANDARD_DEVIATION,
		effect.standardDeviation
	);
	directionalBlurEffect->SetValue(D2D1_DIRECTIONALBLUR_PROP_ANGLE, effect.angle);
	directionalBlurEffect->SetValue(
		D2D1_DIRECTIONALBLUR_PROP_OPTIMIZATION,
		mapOptimization(effect.optimization)
	);
	directionalBlurEffect->SetValue(
		D2D1_DIRECTIONALBLUR_PROP_BORDER_MODE,
		mapBorderMode(effect.borderMode)
	);

	ComPtr<ID2D1Image> outputImage;
	directionalBlurEffect->GetOutput(&outputImage);
	return outputImage;
}

D2D1_BORDER_MODE DirectionalBlurEffectRenderer::mapBorderMode(BlurBorderMode mode) {
	switch (mode) {
	case BlurBorderMode::HARD:
		return D2D1_BORDER_MODE_HARD;
	case BlurBorderMode::SOFT:
	default:
		return D2D1_BORDER_MODE_SOFT;
	}
}

D2D1_DIRECTIONALBLUR_OPTIMIZATION
DirectionalBlurEffectRenderer::mapOptimization(BlurOptimization optimization) {
	switch (optimization) {
	case BlurOptimization::SPEED:
		return D2D1_DIRECTIONALBLUR_OPTIMIZATION_SPEED;
	case BlurOptimization::QUALITY:
		return D2D1_DIRECTIONALBLUR_OPTIMIZATION_QUALITY;
	case BlurOptimization::BALANCED:
	default:
		return D2D1_DIRECTIONALBLUR_OPTIMIZATION_BALANCED;
	}
}
