#include <kke/geometry/shapes/Rect.hh>

kke::Rect kke::operator+(kke::Rect lhs, kke::Point2f const& rhs) {
	lhs.min += rhs;
	lhs.max += rhs;
	return lhs;
}

kke::Rect kke::operator+(kke::Rect lhs, kke::Rect const& rhs) {
	lhs.min += rhs.min;
	lhs.max += rhs.max;
	return lhs;
}

kke::Rect kke::operator-(kke::Rect lhs, kke::Point2f const& rhs) {
	lhs.min -= rhs;
	lhs.max -= rhs;
	return lhs;
}

kke::Rect kke::operator-(kke::Rect lhs, kke::Rect const& rhs) {
	lhs.min -= rhs.min;
	lhs.max -= rhs.max;
	return lhs;
}

kke::Rect& kke::operator+=(kke::Rect& lhs, kke::Point2f const& rhs) {
	lhs.min += rhs;
	lhs.max += rhs;
	return lhs;
}

kke::Rect& kke::operator+=(kke::Rect& lhs, kke::Rect const& rhs) {
	lhs.min += rhs.min;
	lhs.max += rhs.max;
	return lhs;
}

kke::Rect& kke::operator-=(kke::Rect& lhs, kke::Point2f const& rhs) {
	lhs.min -= rhs;
	lhs.max -= rhs;
	return lhs;
}

kke::Rect& kke::operator-=(kke::Rect& lhs, kke::Rect const& rhs) {
	lhs.min -= rhs.min;
	lhs.max -= rhs.max;
	return lhs;
}
