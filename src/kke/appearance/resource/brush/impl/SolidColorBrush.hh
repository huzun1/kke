#pragma once

#include "kke/appearance/Color.hh"

namespace kke {
/**
 * @brief Brush that paints with a single solid color.
 */
class SolidColorBrush {
	Color color;

  public:
	/**
	 * @brief Constructs a brush from a color value.
	 */
	SolidColorBrush(Color color);

	/**
	 * @brief Returns the color used by the brush.
	 */
	Color const& getColor() const;
};
} // namespace kke
