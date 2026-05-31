#pragma once

namespace kke {
/**
 * @brief Performance and quality hint for blur-based effects.
 */
enum class BlurOptimization {
	/**
	 * @brief Prefers faster execution, potentially with more approximation.
	 */
	SPEED,

	/**
	 * @brief Balances quality and performance for general use.
	 */
	BALANCED,

	/**
	 * @brief Prefers higher-quality sampling at the cost of speed.
	 */
	QUALITY
};
}	// namespace kke
