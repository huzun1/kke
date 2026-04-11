#pragma once

#include "kke/geometry/Geometry.hh"
#include "kke/geometry/primitives/Point.hh"

namespace kke {
struct Rect : public Geometry {
	kke::Point2f min;
	kke::Point2f max;

	Rect();

	Rect(kke::Point2f const& min, kke::Point2f const& max);

	Boundary getBounding() const override;

	float width() const;

	float height() const;

	kke::Point2f center() const;

	Rect shrink(float amount) const;

	Rect expand(float amount) const;
};

#include "RectOperators.hh"

};	// namespace kke
