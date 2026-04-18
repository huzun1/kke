#include <kke/geometry/primitives/Point.hh>

kke::Point kke::operator+(kke::Point lhs, float rhs) {
	lhs += rhs;
	return lhs;
}

kke::Point kke::operator+(kke::Point lhs, kke::Point const& rhs) {
	lhs += rhs;
	return lhs;
}

kke::Point kke::operator-(kke::Point lhs, float rhs) {
	lhs -= rhs;
	return lhs;
}

kke::Point kke::operator-(kke::Point lhs, kke::Point const& rhs) {
	lhs -= rhs;
	return lhs;
}

kke::Point kke::operator*(kke::Point lhs, float rhs) {
	lhs *= rhs;
	return lhs;
}

kke::Point kke::operator*(kke::Point lhs, kke::Point const& rhs) {
	lhs *= rhs;
	return lhs;
}

kke::Point kke::operator/(kke::Point lhs, float rhs) {
	lhs /= rhs;
	return lhs;
}

kke::Point kke::operator/(kke::Point lhs, kke::Point const& rhs) {
	lhs /= rhs;
	return lhs;
}

kke::Point& kke::operator+=(kke::Point& lhs, float rhs) {
	lhs.x += rhs;
	lhs.y += rhs;
	return lhs;
}

kke::Point& kke::operator+=(kke::Point& lhs, kke::Point const& rhs) {
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	return lhs;
}

kke::Point& kke::operator-=(kke::Point& lhs, float rhs) {
	lhs.x -= rhs;
	lhs.y -= rhs;
	return lhs;
}

kke::Point& kke::operator-=(kke::Point& lhs, kke::Point const& rhs) {
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	return lhs;
}

kke::Point& kke::operator*=(kke::Point& lhs, float rhs) {
	lhs.x *= rhs;
	lhs.y *= rhs;
	return lhs;
}

kke::Point& kke::operator*=(kke::Point& lhs, kke::Point const& rhs) {
	lhs.x *= rhs.x;
	lhs.y *= rhs.y;
	return lhs;
}

kke::Point& kke::operator/=(kke::Point& lhs, float rhs) {
	lhs.x /= rhs;
	lhs.y /= rhs;
	return lhs;
}

kke::Point& kke::operator/=(kke::Point& lhs, kke::Point const& rhs) {
	lhs.x /= rhs.x;
	lhs.y /= rhs.y;
	return lhs;
}
