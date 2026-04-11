#include <cmath>

#include <kke/geometry/shapes/polygon/Triangle.hh>

#include "kke/geometry/primitives/Point.hh"

using namespace kke;

Triangle::Triangle()
	: a(kke::Point2f{0.0f, 0.0f}),
	  b(kke::Point2f{0.0f, 0.0f}),
	  c(kke::Point2f{0.0f, 0.0f}) {
}

Triangle::Triangle(kke::Point2f const& a, kke::Point2f const& b, kke::Point2f const& c)
	: a(a), b(b), c(c) {
}

kke::Point2f moveToward(kke::Point2f const& from, kke::Point2f const& to, float amount) {
	kke::Point2f delta = to - from;
	float length = std::sqrt(delta.x * delta.x + delta.y * delta.y);
	if (length == 0.0f) {
		return from;
	}
	return from + delta / length * amount;
}

Triangle Triangle::shrink(float amount) {
	Point2f center = (a + b + c) / 3.0f;

	return Triangle(
		moveToward(a, center, amount),
		moveToward(b, center, amount),
		moveToward(c, center, amount));
}

Triangle Triangle::expand(float amount) {
	Point2f center = (a + b + c) / 3.0f;

	return Triangle(
		moveToward(a, center, -amount),
		moveToward(b, center, -amount),
		moveToward(c, center, -amount));
}
