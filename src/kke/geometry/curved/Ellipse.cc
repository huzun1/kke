#include "Ellipse.hh"

using namespace kke;

Ellipse::Ellipse() : point(kke::Point{0.0f, 0.0f}), radius(0.0f) {
}

Ellipse::Ellipse(kke::Point const& point, float radius) : point(point), radius(radius) {
}

Boundary Ellipse::getBounding() const {
	return Boundary{point.x - radius, point.y - radius, point.x + radius, point.y + radius};
}
