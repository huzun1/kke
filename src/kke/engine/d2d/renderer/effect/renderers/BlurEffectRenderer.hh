#pragma once

#include <optional>

#include "kke/appearance/resource/effect/EffectSourceAppearance.hh"
#include "kke/appearance/resource/effect/blur/BlurEffect.hh"
#include "kke/engine/Sources.hh"
#include "kke/engine/d2d/context/D2dEngineContext.hh"
#include "kke/engine/d2d/renderer/effect/EffectSourceRenderer.hh"
#include "kke/engine/d2d/renderer/effect/renderers/PositionIndependentEffectCache.hh"

namespace kke {
class BlurEffectRenderer {
	EffectSourceRenderer sourceRenderer;
	PositionIndependentEffectCache cache;

  public:
	PositionIndependentEffectCache::RenderResult render(
		D2dEngineContext& context,
		EffectSource const& source,
		EffectSourceAppearance const& sourceAppearance,
		BlurEffect const& effect,
		std::optional<EffectClipSource> const& clip
	);

	Microsoft::WRL::ComPtr<ID2D1Image> render(
		D2dEngineContext& context,
		Microsoft::WRL::ComPtr<ID2D1Image> sourceImage,
		BlurEffect const& effect
	) const;

  private:
	Microsoft::WRL::ComPtr<ID2D1Effect> createBlurEffect(
		D2dEngineContext& context,
		Microsoft::WRL::ComPtr<ID2D1Image> sourceImage,
		BlurEffect const& effect
	) const;

	static D2D1_BORDER_MODE mapBorderMode(BlurBorderMode mode);

	static D2D1_GAUSSIANBLUR_OPTIMIZATION mapOptimization(BlurOptimization optimization);

	static uint64_t hashEffect(BlurEffect const& effect);
};
} // namespace kke
