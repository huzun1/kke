#pragma once

namespace kke {
/**
 * @brief Texture sampling mode used during drawing.
 */
enum class TextureInterpolation {
	/**
	 * @brief Samples the nearest texel without smoothing.
	 */
	Nearest,

	/**
	 * @brief Linearly blends neighboring texels for smoother scaling.
	 */
	Linear,
};
}	// namespace kke
