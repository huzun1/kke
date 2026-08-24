#include "EffectRasterScaler.hh"

#include <type_traits>

#include <d2d1effects.h>

#include "kke/engine/d2d/context/D2dContext.hh"

using namespace kke;
using Microsoft::WRL::ComPtr;

ComPtr<ID2D1Image> EffectRasterScaler::scaleSource(
	D2dEngineContext& context, ComPtr<ID2D1Image> sourceImage, float rasterScale
) const {
	if (rasterScale == 1.0f) {
		return sourceImage;
	}

	ComPtr<ID2D1Effect> scaleEffect;
	ID2D1DeviceContext* deviceContext = context.getD2dContext()->getDeviceContext();
	scaleEffect =
		context.getResourceProviders()->getEffectPool()->acquire(deviceContext, CLSID_D2D1Scale);
	if (!scaleEffect) {
		return nullptr;
	}

	scaleEffect->SetInput(0, sourceImage.Get());
	scaleEffect->SetValue(D2D1_SCALE_PROP_SCALE, D2D1::Vector2F(rasterScale, rasterScale));
	scaleEffect->SetValue(D2D1_SCALE_PROP_INTERPOLATION_MODE, D2D1_SCALE_INTERPOLATION_MODE_LINEAR);

	ComPtr<ID2D1Image> scaledImage;
	scaleEffect->GetOutput(&scaledImage);
	return scaledImage;
}

Effect EffectRasterScaler::scaleEffect(Effect const& effect, float rasterScale) const {
	return std::visit(
		[rasterScale](auto scaledEffect) -> Effect {
			using EffectType = std::decay_t<decltype(scaledEffect)>;
			if constexpr (std::is_same_v<EffectType, BlurEffect>) {
				scaledEffect.appearance.radius *= rasterScale;
			} else if constexpr (std::is_same_v<EffectType, DirectionalBlurEffect>) {
				scaledEffect.standardDeviation *= rasterScale;
			} else if constexpr (std::is_same_v<EffectType, ShadowEffect>) {
				scaledEffect.offset.x *= rasterScale;
				scaledEffect.offset.y *= rasterScale;
				scaledEffect.blurStandardDeviation *= rasterScale;
			}
			return scaledEffect;
		},
		effect
	);
}
