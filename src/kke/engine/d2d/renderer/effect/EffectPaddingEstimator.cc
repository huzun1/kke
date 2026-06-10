#include "EffectPaddingEstimator.hh"

#include <algorithm>
#include <cmath>

using namespace kke;

float EffectPaddingEstimator::estimate(Effect const& effect) {
	return std::visit([](auto const& effectVariant) -> float { return estimate(effectVariant); }, effect);
}

float EffectPaddingEstimator::estimate(EffectCompose const& effectCompose) {
	float padding = 0.0f;

	for (Effect const& effect : effectCompose.getCompose()) {
		padding += estimate(effect);
	}

	return padding;
}

float EffectPaddingEstimator::estimate(ShadowEffect const& effect) {
	float shadowPadding = effect.blurStandardDeviation * 6.0f;
	return shadowPadding + std::max(std::abs(effect.offset.x), std::abs(effect.offset.y));
}

float EffectPaddingEstimator::estimate(BlurEffect const& effect) {
	return effect.appearance.radius * 6.0f;
}

float EffectPaddingEstimator::estimate(DirectionalBlurEffect const& effect) {
	return effect.standardDeviation * 6.0f;
}

float EffectPaddingEstimator::estimate(ColorMatrixEffect const& effect) {
	(void)effect;
	return 0.0f;
}
