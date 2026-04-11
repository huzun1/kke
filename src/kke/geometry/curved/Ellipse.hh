#pragma once

#include <d2d1.h>

#include "kke/geometry/impl/primitives/Point.hh"
#include "kke/geometry/Polygon.hh"

namespace kke {
struct Ellipse : public Polygon {
	kke::Point2f point;
	float radius;

	Ellipse();

	Ellipse(kke::Point2f const& point, float radius);

	Boundary getBounding() const override;
};

#include "EllipseOperators.hh"

};	// namespace kke
