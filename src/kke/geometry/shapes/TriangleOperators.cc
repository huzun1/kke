#include <kke/geometry/shapes/Triangle.hh>

kke::Triangle kke::operator+(kke::Triangle lhs, kke::Point const& rhs) {
	lhs.a += rhs;
	lhs.b += rhs;
	lhs.c += rhs;
	return lhs;
}

kke::Triangle kke::operator+(kke::Triangle lhs, kke::Triangle const& rhs) {
	lhs.a += rhs.a;
	lhs.b += rhs.b;
	lhs.c += rhs.c;
	return lhs;
}

kke::Triangle kke::operator-(kke::Triangle lhs, kke::Point const& rhs) {
	lhs.a -= rhs;
	lhs.b -= rhs;
	lhs.c -= rhs;
	return lhs;
}

kke::Triangle kke::operator-(kke::Triangle lhs, kke::Triangle const& rhs) {
	lhs.a -= rhs.a;
	lhs.b -= rhs.b;
	lhs.c -= rhs.c;
	return lhs;
}

kke::Triangle& kke::operator+=(kke::Triangle& lhs, kke::Point const& rhs) {
	lhs.a += rhs;
	lhs.b += rhs;
	lhs.c += rhs;
	return lhs;
}

kke::Triangle& kke::operator+=(kke::Triangle& lhs, kke::Triangle const& rhs) {
	lhs.a += rhs.a;
	lhs.b += rhs.b;
	lhs.c += rhs.c;
	return lhs;
}

kke::Triangle& kke::operator-=(kke::Triangle& lhs, kke::Point const& rhs) {
	lhs.a -= rhs;
	lhs.b -= rhs;
	lhs.c -= rhs;
	return lhs;
}

kke::Triangle& kke::operator-=(kke::Triangle& lhs, kke::Triangle const& rhs) {
	lhs.a -= rhs.a;
	lhs.b -= rhs.b;
	lhs.c -= rhs.c;
	return lhs;
}
