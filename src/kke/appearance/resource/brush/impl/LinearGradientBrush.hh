#pragma once

#include <optional>
#include <vector>

#include "kke/appearance/Color.hh"
#include "kke/geometry/primitives/Point.hh"

namespace kke {
/**
 * @brief Brush that interpolates colors along a line segment.
 */
class LinearGradientBrush {
	std::vector<Color> colors;
	Point startPoint;
	Point endPoint;
	float angle;

public:
	/**
	 * @brief Constructs a linear gradient brush.
	 *
	 * @param colors Gradient stops in order from start to end.
	 * @param startPoint Gradient start position.
	 * @param endPoint Gradient end position.
	 * @param angle Optional angle override for backends that support it.
	 */
	LinearGradientBrush(std::vector<Color> const& colors, Point const& startPoint, Point const& endPoint, float angle = 0.0f);

	/**
	 * @brief Returns the colors used by the gradient.
	 */
	std::vector<Color> const& getColors() const;

	/**
	 * @brief Returns the start point of the gradient line.
	 */
	Point const& getStartPoint() const;

	/**
	 * @brief Returns the end point of the gradient line.
	 */
	Point const& getEndPoint() const;

	/**
	 * @brief Returns the configured gradient angle in degrees.
	 */
	float getAngle() const;
};
}	// namespace kke
