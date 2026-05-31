#pragma once

namespace kke {
/**
 * @brief Defines font weight (thickness) levels for text rendering.
 *
 * These values represent commonly used typographic weight categories.
 */
enum class FontWeight {
	/**
	 * @brief Light weight.
	 *
	 * Thin and less prominent than normal text.
	 */
	LIGHT,

	/**
	 * @brief Normal weight.
	 *
	 * Standard/default font thickness.
	 */
	NORMAL,

	/**
	 * @brief Medium weight.
	 *
	 * Slightly thicker than normal.
	 */
	MEDIUM,

	/**
	 * @brief Semi-bold weight.
	 *
	 * Between medium and bold; provides moderate emphasis.
	 */
	SEMI_BOLD,

	/**
	 * @brief Bold weight.
	 *
	 * Thick and highly emphasized text.
	 */
	BOLD
};
} // namespace kke