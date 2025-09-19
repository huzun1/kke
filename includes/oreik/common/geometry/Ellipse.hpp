#pragma once

#include <d2d1.h>

#include "oreik/common/Point.hpp"

namespace oreik {
struct Ellipse {
	oreik::Point2f point;
	float radius;

	Ellipse(float x, float y, float radius);

	D2D1_ELLIPSE ellipse() const;
};
};	// namespace oreik
