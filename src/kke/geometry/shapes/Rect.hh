#pragma once

#include "kke/geometry/Boundable.hh"
#include "kke/geometry/primitives/Point.hh"

namespace kke {
struct Rect : public Boundable {
	kke::Point min;
	kke::Point max;

	Rect();

	Rect(kke::Point const& min, kke::Point const& max);

	[[nodiscard]] Boundary getBounding() const override;

	[[nodiscard]] float width() const;

	[[nodiscard]] float height() const;

	[[nodiscard]] kke::Point center() const;

	[[nodiscard]] Rect shrink(float amount) const;

	[[nodiscard]] Rect expand(float amount) const;
};

#include "RectOperators.hh"

};	// namespace kke
