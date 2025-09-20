#pragma once

#include <d2d1.h>

#include <cstdint>

#include "oreik/common/Geometry.hpp"
#include "oreik/common/Point.hpp"

namespace oreik {
struct Ellipse : public Geometry {
	oreik::Point2f point;
	float radius;

	Ellipse(float x, float y, float radius);

	D2D1_ELLIPSE ellipse() const;

	uint64_t hash(bool positionDependent) const;
};
};	// namespace oreik
