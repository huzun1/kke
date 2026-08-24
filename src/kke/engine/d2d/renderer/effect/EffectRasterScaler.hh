#pragma once

#include <wrl/client.h>

#include "kke/appearance/resource/effect/Effect.hh"
#include "kke/engine/d2d/context/D2dEngineContext.hh"

namespace kke {
class EffectRasterScaler {
  public:
	Microsoft::WRL::ComPtr<ID2D1Image> scaleSource(
		D2dEngineContext& context, Microsoft::WRL::ComPtr<ID2D1Image> sourceImage, float rasterScale
	) const;

	Effect scaleEffect(Effect const& effect, float rasterScale) const;
};
} // namespace kke
