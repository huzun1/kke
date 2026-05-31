#pragma once

namespace kke {
/**
 * @brief Alpha interpretation used when applying a color matrix.
 */
enum class ColorMatrixAlphaMode {
	/**
	 * @brief Treat colors as premultiplied by alpha.
	 */
	Premultiplied,

	/**
	 * @brief Treat colors as straight, unpremultiplied components.
	 */
	Straight
};
}	// namespace kke
