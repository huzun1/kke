#pragma once

namespace kke {
/**
 * @brief Controls which parts of a shadow effect are rendered.
 */
enum class ShadowMode {
	/**
	 * @brief Render only the shadow, including any overlap with the source.
	 */
	ShadowOnly,

	/**
	 * @brief Render only the outer portion of the shadow.
	 */
	OuterShadowOnly,

	/**
	 * @brief Render both the source and its shadow.
	 */
	ShadowWithSource
};
}	// namespace kke
