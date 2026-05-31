#pragma once

#include <string>
#include <variant>

#include "kke/appearance/resource/font/FontAppearance.hh"
#include "kke/geometry/primitives/Point.hh"

namespace kke {
/**
 * @brief Represents a renderable text object.
 *
 * This structure encapsulates text content along with its position
 * and font styling information for rendering.
 */
struct Text {
	/**
	 * @brief The text content.
	 *
	 * Supports both narrow (UTF-8 or locale-dependent) and wide strings.
	 */
	std::variant<std::string, std::wstring> text;

	/**
	 * @brief The position where the text will be rendered.
	 */
	Point position;

	/**
	 * @brief Font appearance settings used for rendering the text.
	 */
	FontAppearance fontAppearance;
};
}	// namespace kke
