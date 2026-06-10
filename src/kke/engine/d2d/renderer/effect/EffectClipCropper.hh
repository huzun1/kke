#pragma once

#include "kke/appearance/resource/effect/EffectCompose.hh"
#include "kke/engine/Sources.hh"
#include "kke/engine/d2d/context/D2dEngineContext.hh"

namespace kke {
class EffectClipCropper {
  public:
	Microsoft::WRL::ComPtr<ID2D1Image> crop(
		D2dEngineContext& context,
		Microsoft::WRL::ComPtr<ID2D1Image> sourceImage,
		Effect const& effect,
		EffectClipSource const& clip
	) const;

	Microsoft::WRL::ComPtr<ID2D1Image> crop(
		D2dEngineContext& context,
		Microsoft::WRL::ComPtr<ID2D1Image> sourceImage,
		EffectCompose const& effectCompose,
		EffectClipSource const& clip
	) const;

  private:
	static D2D1_RECT_F expandRect(D2D1_RECT_F const& rect, float amount);
};
} // namespace kke
