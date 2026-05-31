#pragma once

namespace kke {
/**
 * @brief Controls whether an effect source is filled or stroked.
 */
enum class EffectSourceDrawMode {
	/**
	 * @brief Render the interior of the source.
	 */
	Fill,

	/**
	 * @brief Render only the outline of the source.
	 */
	Stroke
};
} // namespace kke
