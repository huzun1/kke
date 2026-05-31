#pragma once

#include "kke/appearance/resource/effect/blur/BlurBorderMode.hh"
#include "kke/appearance/resource/effect/blur/BlurOptimization.hh"

namespace kke {
/**
 * @brief Applies a blur biased toward a specific direction.
 */
struct DirectionalBlurEffect {
	/**
	 * @brief Blur strength, expressed as standard deviation.
	 */
	float standardDeviation = 3.0f;

	/**
	 * @brief Direction of the blur in degrees.
	 */
	float angle = 0.0f;

	/**
	 * @brief Border handling mode used while sampling near the source edges.
	 */
	BlurBorderMode borderMode = BlurBorderMode::SOFT;

	/**
	 * @brief Performance versus quality tradeoff used by the backend.
	 */
	BlurOptimization optimization = BlurOptimization::BALANCED;
};
} // namespace kke
