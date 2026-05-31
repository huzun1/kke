#pragma once

#include "kke/appearance/resource/effect/blur/BlurAppearance.hh"

namespace kke {
/**
 * @brief Applies a configurable blur to the source.
 */
struct BlurEffect {
	/**
	 * @brief Parameters controlling the blur result.
	 */
	BlurAppearance appearance {};

	/**
	 * @brief Constructs a blur effect with the default appearance.
	 */
	BlurEffect() = default;

	/**
	 * @brief Constructs a normal blur using the supplied radius.
	 */
	BlurEffect(float radius)
		: appearance({radius, BlurBorderMode::SOFT, BlurOptimization::BALANCED, BlurMode::Normal}) {
	}

	/**
	 * @brief Constructs a blur effect from a fully specified appearance.
	 */
	BlurEffect(BlurAppearance const& appearance)
		: appearance(appearance) {
	}
};
}	// namespace kke
