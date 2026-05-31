#pragma once

#include "kke/appearance/Color.hh"
#include "kke/appearance/resource/effect/shadow/ShadowMode.hh"
#include "kke/geometry/primitives/Point.hh"

namespace kke {
/**
 * @brief Draws a shadow from the supplied source.
 */
struct ShadowEffect {
	/**
	 * @brief Offset applied to the generated shadow.
	 */
	Point offset{0.0f, 0.0f};

	/**
	 * @brief Shadow softness, expressed as standard deviation.
	 */
	float blurStandardDeviation = 3.0f;

	/**
	 * @brief Color applied to the generated shadow.
	 */
	Color color{0.0f, 0.0f, 0.0f, 1.0f};

	/**
	 * @brief Determines whether the source, the shadow, or both are rendered.
	 */
	ShadowMode mode = ShadowMode::ShadowWithSource;
};
} // namespace kke
