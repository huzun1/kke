#pragma once

#include "kke/geometry/Boundable.hh"
#include "kke/geometry/primitives/Point.hh"

namespace kke {
struct Ellipse : public Boundable {
	kke::Point point;
	float radius;

	~Ellipse() = default;

	Ellipse();

	Ellipse(kke::Point const& point, float radius);

	Boundary getBounding() const override;
};

#include "EllipseOperators.hh"

};	// namespace kke
