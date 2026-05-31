#pragma once

#include <string>

#include "FontWeight.hh"

namespace kke {
/**
 * @brief Defines font styling properties for text rendering.
 *
 * This structure describes how text should be visually rendered,
 * including font family, size, and weight.
 */
struct FontAppearance {
	/**
	 * @brief The font family name.
	 *
	 * Typically a system or embedded font identifier (e.g., "Arial", "Roboto").
	 */
	std::string fontFamily;

	/**
	 * @brief The font size.
	 *
	 * The size of the font in logical units (e.g., pixels or points,
	 * depending on the rendering context).
	 */
	float fontSize;

	/**
	 * @brief The font weight.
	 *
	 * Defines the thickness of the glyphs (e.g., normal, bold).
	 */
	FontWeight weight;
};
}; // namespace kke