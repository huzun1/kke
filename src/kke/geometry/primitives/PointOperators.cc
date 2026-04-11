#include <kke/geometry/primitives/Point.hh>

kke::Point2f kke::operator+(kke::Point2f lhs, float rhs) {
	lhs += rhs;
	return lhs;
}

kke::Point2f kke::operator+(kke::Point2f lhs, kke::Point2f const& rhs) {
	lhs += rhs;
	return lhs;
}

kke::Point2f kke::operator-(kke::Point2f lhs, float rhs) {
	lhs -= rhs;
	return lhs;
}

kke::Point2f kke::operator-(kke::Point2f lhs, kke::Point2f const& rhs) {
	lhs -= rhs;
	return lhs;
}

kke::Point2f kke::operator*(kke::Point2f lhs, float rhs) {
	lhs *= rhs;
	return lhs;
}

kke::Point2f kke::operator*(kke::Point2f lhs, kke::Point2f const& rhs) {
	lhs *= rhs;
	return lhs;
}

kke::Point2f kke::operator/(kke::Point2f lhs, float rhs) {
	lhs /= rhs;
	return lhs;
}

kke::Point2f kke::operator/(kke::Point2f lhs, kke::Point2f const& rhs) {
	lhs /= rhs;
	return lhs;
}

kke::Point2f& kke::operator+=(kke::Point2f& lhs, float rhs) {
	lhs.x += rhs;
	lhs.y += rhs;
	return lhs;
}

kke::Point2f& kke::operator+=(kke::Point2f& lhs, kke::Point2f const& rhs) {
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	return lhs;
}

kke::Point2f& kke::operator-=(kke::Point2f& lhs, float rhs) {
	lhs.x -= rhs;
	lhs.y -= rhs;
	return lhs;
}

kke::Point2f& kke::operator-=(kke::Point2f& lhs, kke::Point2f const& rhs) {
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	return lhs;
}

kke::Point2f& kke::operator*=(kke::Point2f& lhs, float rhs) {
	lhs.x *= rhs;
	lhs.y *= rhs;
	return lhs;
}

kke::Point2f& kke::operator*=(kke::Point2f& lhs, kke::Point2f const& rhs) {
	lhs.x *= rhs.x;
	lhs.y *= rhs.y;
	return lhs;
}

kke::Point2f& kke::operator/=(kke::Point2f& lhs, float rhs) {
	lhs.x /= rhs;
	lhs.y /= rhs;
	return lhs;
}

kke::Point2f& kke::operator/=(kke::Point2f& lhs, kke::Point2f const& rhs) {
	lhs.x /= rhs.x;
	lhs.y /= rhs.y;
	return lhs;
}
