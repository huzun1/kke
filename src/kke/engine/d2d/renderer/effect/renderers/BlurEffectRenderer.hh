#pragma once

#include "kke/appearance/resource/effect/blur/BlurEffect.hh"
#include "kke/engine/d2d/context/D2dEngineContext.hh"

namespace kke {
class BlurEffectRenderer {
public:
	Microsoft::WRL::ComPtr<ID2D1Image> render(
		D2dEngineContext& context,
		Microsoft::WRL::ComPtr<ID2D1Image> sourceImage,
		BlurEffect const& effect) const;

private:
	Microsoft::WRL::ComPtr<ID2D1Effect> createBlurEffect(
		D2dEngineContext& context,
		Microsoft::WRL::ComPtr<ID2D1Image> sourceImage,
		BlurEffect const& effect) const;

	static D2D1_BORDER_MODE mapBorderMode(BlurBorderMode mode);

	static D2D1_GAUSSIANBLUR_OPTIMIZATION mapOptimization(BlurOptimization optimization);
};
}	// namespace kke
