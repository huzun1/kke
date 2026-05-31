#pragma once

namespace kke {
/**
 * @brief Border sampling strategy used by blur-based effects.
 */
enum class BlurBorderMode {
	/**
	 * @brief Produces antialiased, softer edges near cropped boundaries.
	 */
	SOFT,

	/**
	 * @brief Clamps boundaries to preserve a harder edge.
	 */
	HARD
};
} // namespace kke
