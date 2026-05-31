#pragma once

namespace kke {
/**
 * @brief Determines which portion of a blur result is kept.
 */
enum class BlurMode {
	/**
	 * @brief Keep the full blurred result.
	 */
	Normal,

	/**
	 * @brief Keep only the region outside the original source.
	 */
	OuterOnly
};
}	// namespace kke
