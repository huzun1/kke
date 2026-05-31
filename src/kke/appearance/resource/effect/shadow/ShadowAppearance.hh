#pragma once

namespace kke {
/**
 * @brief Lightweight shadow tuning data shared by some higher-level APIs.
 */
struct ShadowAppearance {
	/**
	 * @brief Blur deviation used to soften the shadow.
	 */
	float deviation;
};
} // namespace kke
