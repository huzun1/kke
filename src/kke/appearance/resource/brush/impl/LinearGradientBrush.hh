#pragma once

#include <optional>
#include <vector>

#include "kke/appearance/Color.hh"
#include "kke/geometry/primitives/Point.hh"

namespace kke {
class LinearGradientBrush {
	std::vector<Color> colors;
	kke::Point startPoint;
	kke::Point endPoint;
	float angle;

public:
	LinearGradientBrush(std::vector<kke::Color> const& colors,
						kke::Point const& startPoint, kke::Point const& endPoint, float angle = 0.0f);

	std::vector<kke::Color> const& getColors() const;

	kke::Point const& getStartPoint() const;

	kke::Point const& getEndPoint() const;

	float getAngle() const;
};
};	// namespace kke
