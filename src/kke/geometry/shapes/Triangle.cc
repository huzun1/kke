#include <algorithm>
#include <cmath>

#include <kke/geometry/shapes/Triangle.hh>

#include "kke/geometry/primitives/Point.hh"

using namespace kke;

Triangle::Triangle()
	: a(kke::Point{0.0f, 0.0f}), b(kke::Point{0.0f, 0.0f}), c(kke::Point{0.0f, 0.0f}) {
}

Triangle::Triangle(kke::Point const& a, kke::Point const& b, kke::Point const& c)
	: a(a), b(b), c(c) {
}

Boundary Triangle::getBounding() const {
	float minX = std::min({a.x, b.x, c.x});
	float minY = std::min({a.y, b.y, c.y});
	float maxX = std::max({a.x, b.x, c.x});
	float maxY = std::max({a.y, b.y, c.y});
	return {minY, minX, maxX, maxY};
}

kke::Point moveToward(kke::Point const& from, kke::Point const& to, float amount) {
	kke::Point delta = to - from;
	float length = std::sqrt(delta.x * delta.x + delta.y * delta.y);
	if (length == 0.0f) {
		return from;
	}
	return from + delta / length * amount;
}

Triangle Triangle::shrink(float amount) const {
	Point center = (a + b + c) / 3.0f;

	return Triangle(
		moveToward(a, center, amount),
		moveToward(b, center, amount),
		moveToward(c, center, amount)
	);
}

Triangle Triangle::expand(float amount) const {
	Point center = (a + b + c) / 3.0f;

	return Triangle(
		moveToward(a, center, -amount),
		moveToward(b, center, -amount),
		moveToward(c, center, -amount)
	);
}
