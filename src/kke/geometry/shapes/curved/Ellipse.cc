#include "Ellipse.hh"

#include <cstdint>

#include "kke/common/Geometry.hh"
#include "kke/internal/Hasher.hh"

using namespace kke;

Ellipse::Ellipse()
	: point(kke::Point2f{0.0f, 0.0f}), radius(0.0f) {
}

Ellipse::Ellipse(kke::Point2f const& point, float radius)
	: point(point), radius(radius) {
}

D2D1_ELLIPSE Ellipse::asD2d1Ellipse() const {
	return {point.asD2dPoint2f(), radius, radius};
}