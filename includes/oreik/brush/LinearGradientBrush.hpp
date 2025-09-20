#pragma once

#include <optional>
#include <vector>

#include "Brush.hpp"
#include "oreik/common/Color.hpp"
#include "oreik/common/Point.hpp"

namespace oreik {
class LinearGradientBrush : public Brush {
	std::vector<oreik::Color4f> colors;
	oreik::Point2f startPoint;
	oreik::Point2f endPoint;
	std::optional<float> angle;

public:
	LinearGradientBrush(oreik::Color4f const& startColor, oreik::Color4f const& endColor, oreik::Point2f const& startPoint, oreik::Point2f const& endPoint);

	LinearGradientBrush(std::vector<oreik::Color4f> const& colors, oreik::Point2f const& startPoint, oreik::Point2f const& endPoint);

	void setAngle(float angle);

	void create(ID2D1DeviceContext* context, ID2D1Brush** output) const override;

	uint64_t hash() const override;
};
};	// namespace oreik
