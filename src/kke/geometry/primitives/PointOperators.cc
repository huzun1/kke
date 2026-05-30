#include <kke/geometry/primitives/Point.hh>

namespace kke {
Point operator+(Point lhs, float rhs) {
	lhs += rhs;
	return lhs;
}

Point operator+(Point lhs, Point const& rhs) {
	lhs += rhs;
	return lhs;
}

Point operator-(Point lhs, float rhs) {
	lhs -= rhs;
	return lhs;
}

Point operator-(Point lhs, Point const& rhs) {
	lhs -= rhs;
	return lhs;
}

Point operator*(Point lhs, float rhs) {
	lhs *= rhs;
	return lhs;
}

Point operator*(Point lhs, Point const& rhs) {
	lhs *= rhs;
	return lhs;
}

Point operator/(Point lhs, float rhs) {
	lhs /= rhs;
	return lhs;
}

Point operator/(Point lhs, Point const& rhs) {
	lhs /= rhs;
	return lhs;
}

Point& operator+=(Point& lhs, float rhs) {
	lhs.x += rhs;
	lhs.y += rhs;
	return lhs;
}

Point& operator+=(Point& lhs, Point const& rhs) {
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	return lhs;
}

Point& operator-=(Point& lhs, float rhs) {
	lhs.x -= rhs;
	lhs.y -= rhs;
	return lhs;
}

Point& operator-=(Point& lhs, Point const& rhs) {
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	return lhs;
}

Point& operator*=(Point& lhs, float rhs) {
	lhs.x *= rhs;
	lhs.y *= rhs;
	return lhs;
}

Point& operator*=(Point& lhs, Point const& rhs) {
	lhs.x *= rhs.x;
	lhs.y *= rhs.y;
	return lhs;
}

Point& operator/=(Point& lhs, float rhs) {
	lhs.x /= rhs;
	lhs.y /= rhs;
	return lhs;
}

Point& operator/=(Point& lhs, Point const& rhs) {
	lhs.x /= rhs.x;
	lhs.y /= rhs.y;
	return lhs;
}
}	// namespace kke
