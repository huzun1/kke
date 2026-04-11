#pragma once

#include <d2d1.h>

#include "kke/geometry/primitives/Point.hh"
#include "kke/geometry/Geometry.hh"

namespace kke {
struct Ellipse : public Geometry {
	kke::Point2f point;
	float radius;

	Ellipse();

	Ellipse(kke::Point2f const& point, float radius);

	Boundary getBounding() const override;

	D2D1_ELLIPSE asD2d1Ellipse() const;
};

#include "EllipseOperators.hh"

};	// namespace kke
