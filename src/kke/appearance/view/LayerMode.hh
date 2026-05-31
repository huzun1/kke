#pragma once

namespace kke {
/**
 * @brief Controls how a layer mask is interpreted.
 */
enum class LayerMode {
	/**
	 * @brief Draw only inside the supplied mask.
	 */
	Normal,

	/**
	 * @brief Draw everywhere except inside the supplied mask.
	 */
	Inverted
};
}	// namespace kke
