#pragma once

#include "BlurBorderMode.hh"
#include "BlurMode.hh"
#include "BlurOptimization.hh"

namespace kke {
/**
 * @brief Common blur settings shared by blur-based effects.
 */
struct BlurAppearance {
	/**
	 * @brief Blur radius expressed as standard deviation.
	 */
	float radius = 3.0f;

	/**
	 * @brief Border handling mode used while sampling near the source edges.
	 */
	BlurBorderMode borderMode = BlurBorderMode::SOFT;

	/**
	 * @brief Performance versus quality tradeoff used by the backend.
	 */
	BlurOptimization optimization = BlurOptimization::BALANCED;

	/**
	 * @brief Controls which portion of the blurred result is kept.
	 */
	BlurMode mode = BlurMode::Normal;
};
}	// namespace kke
