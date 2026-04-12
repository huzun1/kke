#pragma once

#include <optional>
#include <vector>

#include "Brush.hh"
#include "kke/common/Color.hh"
#include "kke/common/Point.hh"

namespace kke {
class LinearGradientBrush : public Brush {
	std::vector<kke::Color4f> colors;
	kke::Point2f startPoint;
	kke::Point2f endPoint;
	std::optional<float> angle;

public:
	LinearGradientBrush(kke::Color4f const& startColor, kke::Color4f const& endColor, kke::Point2f const& startPoint, kke::Point2f const& endPoint);

	LinearGradientBrush(std::vector<kke::Color4f> const& colors, kke::Point2f const& startPoint, kke::Point2f const& endPoint);
};
};	// namespace kke
