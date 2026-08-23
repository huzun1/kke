#pragma once

#include <optional>

#include "kke/appearance/resource/effect/EffectSourceAppearance.hh"
#include "kke/appearance/resource/effect/shadow/ShadowEffect.hh"
#include "kke/engine/Sources.hh"
#include "kke/engine/d2d/context/D2dEngineContext.hh"
#include "kke/engine/d2d/renderer/effect/EffectSourceRenderer.hh"
#include "kke/engine/d2d/renderer/effect/renderers/PositionIndependentEffectCache.hh"

namespace kke {
class ShadowEffectRenderer {
	EffectSourceRenderer sourceRenderer;
	PositionIndependentEffectCache cache;

  public:
	PositionIndependentEffectCache::RenderResult render(
		D2dEngineContext& context,
		EffectSource const& source,
		EffectSourceAppearance const& sourceAppearance,
		ShadowEffect const& effect,
		std::optional<EffectClipSource> const& clip
	);

	Microsoft::WRL::ComPtr<ID2D1Image> render(
		D2dEngineContext& context,
		Microsoft::WRL::ComPtr<ID2D1Image> sourceImage,
		ShadowEffect const& effect
	) const;

  private:
	Microsoft::WRL::ComPtr<ID2D1Effect> createShadowEffect(
		D2dEngineContext& context,
		Microsoft::WRL::ComPtr<ID2D1Image> sourceImage,
		ShadowEffect const& effect
	) const;

	Microsoft::WRL::ComPtr<ID2D1Effect> createOffsetEffect(
		D2dEngineContext& context, ID2D1Effect* shadowEffect, ShadowEffect const& effect
	) const;

	Microsoft::WRL::ComPtr<ID2D1Image> createOuterShadowImage(
		D2dEngineContext& context,
		ID2D1Effect* offsetEffect,
		Microsoft::WRL::ComPtr<ID2D1Image> sourceImage
	) const;

	static uint64_t hashEffect(ShadowEffect const& effect);
};
} // namespace kke
