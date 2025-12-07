#include <kke/common/Point.hh>

#include "kke/internal/Hasher.hh"

D2D1_POINT_2F kke::Point2f::point2f() const {
	return {x, y};
}

uint64_t kke::Point2f::hash() const {
	kke::Hasher hash;
	hash.combine(x);
	hash.combine(y);
	return hash.get();
}

kke::Point2f kke::Point2f::offset(float amount) const {
	return {x + amount, y + amount};
}

kke::Point2f kke::Point2f::offset(Point2f const& amount) const {
	return {x + amount.x, y + amount.y};
}

kke::Point2f kke::Point2f::operator*(const Point2f& param) const {
	return {x * param.x, y * param.y};
}
