#pragma once

#include "kke/geometry/Polygon.hh"
#include "kke/geometry/primitives/Point.hh"

namespace kke {
struct Rect : public Polygon {
	kke::Point2f min;
	kke::Point2f max;

	Rect();

	Rect(kke::Point2f const& min, kke::Point2f const& max);

	[[nodiscard]] Boundary getBounding() const override;

	[[nodiscard]] float width() const;

	[[nodiscard]] float height() const;

	[[nodiscard]] kke::Point2f center() const;

	[[nodiscard]] Rect shrink(float amount) const;

	[[nodiscard]] Rect expand(float amount) const;
};

#include "RectOperators.hh"

};	// namespace kke
