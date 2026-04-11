#include <kke/geometry/shapes/curved/Ellipse.hh>

kke::Ellipse kke::operator+(kke::Ellipse lhs, kke::Point2f const& rhs) {
	lhs.point += rhs;
	return lhs;
}

kke::Ellipse kke::operator+(kke::Ellipse lhs, kke::Ellipse const& rhs) {
	lhs.point += rhs.point;
	lhs.radius += rhs.radius;
	return lhs;
}

kke::Ellipse kke::operator-(kke::Ellipse lhs, kke::Point2f const& rhs) {
	lhs.point -= rhs;
	return lhs;
}

kke::Ellipse kke::operator-(kke::Ellipse lhs, kke::Ellipse const& rhs) {
	lhs.point -= rhs.point;
	lhs.radius -= rhs.radius;
	return lhs;
}

kke::Ellipse& kke::operator+=(kke::Ellipse& lhs, kke::Point2f const& rhs) {
	lhs.point += rhs;
	return lhs;
}

kke::Ellipse& kke::operator+=(kke::Ellipse& lhs, kke::Ellipse const& rhs) {
	lhs.point += rhs.point;
	lhs.radius += rhs.radius;
	return lhs;
}

kke::Ellipse& kke::operator-=(kke::Ellipse& lhs, kke::Point2f const& rhs) {
	lhs.point -= rhs;
	return lhs;
}

kke::Ellipse& kke::operator-=(kke::Ellipse& lhs, kke::Ellipse const& rhs) {
	lhs.point -= rhs.point;
	lhs.radius -= rhs.radius;
	return lhs;
}
