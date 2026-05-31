#pragma once

#include "kke/appearance/resource/effect/directional_blur/DirectionalBlurEffect.hh"
#include "kke/engine/d2d/context/D2dEngineContext.hh"

namespace kke {
class DirectionalBlurEffectRenderer {
  public:
	Microsoft::WRL::ComPtr<ID2D1Image> render(
		D2dEngineContext& context,
		Microsoft::WRL::ComPtr<ID2D1Image> sourceImage,
		DirectionalBlurEffect const& effect
	) const;

  private:
	static D2D1_BORDER_MODE mapBorderMode(BlurBorderMode mode);

	static D2D1_DIRECTIONALBLUR_OPTIMIZATION mapOptimization(BlurOptimization optimization);
};
} // namespace kke
