#pragma once

#include "kke/appearance/resource/effect/EffectCompose.hh"

namespace kke {
class EffectPaddingEstimator {
  public:
	static float estimate(Effect const& effect);

	static float estimate(EffectCompose const& effectCompose);

  private:
	static float estimate(ShadowEffect const& effect);

	static float estimate(BlurEffect const& effect);

	static float estimate(DirectionalBlurEffect const& effect);

	static float estimate(ColorMatrixEffect const& effect);
};
} // namespace kke
