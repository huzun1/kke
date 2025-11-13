#pragma once

#include <d2d1.h>

#include <cstdint>

#include "Rect.hpp"
#include "kke/common/Geometry.hpp"
#include "kke/common/Point.hpp"

namespace kke {
struct Ellipse : public Geometry {
	kke::Point2f point;
	float radius;

	Ellipse(float x, float y, float radius);

	D2D1_ELLIPSE ellipse() const;

	kke::Rect dimension() const;

	uint64_t hash(bool positionDependent) const;
};
};	// namespace kke
