#include "Scale.hh"

namespace kke {
Scale operator+(Scale lhs, float rhs) {
	lhs += rhs;
	return lhs;
}

Scale operator+(Scale lhs, Scale const& rhs) {
	lhs += rhs;
	return lhs;
}

Scale operator-(Scale lhs, float rhs) {
	lhs -= rhs;
	return lhs;
}

Scale operator-(Scale lhs, Scale const& rhs) {
	lhs -= rhs;
	return lhs;
}

Scale operator*(Scale lhs, float rhs) {
	lhs *= rhs;
	return lhs;
}

Scale operator*(Scale lhs, Scale const& rhs) {
	lhs *= rhs;
	return lhs;
}

Scale operator/(Scale lhs, float rhs) {
	lhs /= rhs;
	return lhs;
}

Scale operator/(Scale lhs, Scale const& rhs) {
	lhs /= rhs;
	return lhs;
}

Scale& operator+=(Scale& lhs, float rhs) {
	lhs.x += rhs;
	lhs.y += rhs;
	return lhs;
}

Scale& operator+=(Scale& lhs, Scale const& rhs) {
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	return lhs;
}

Scale& operator-=(Scale& lhs, float rhs) {
	lhs.x -= rhs;
	lhs.y -= rhs;
	return lhs;
}

Scale& operator-=(Scale& lhs, Scale const& rhs) {
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	return lhs;
}

Scale& operator*=(Scale& lhs, float rhs) {
	lhs.x *= rhs;
	lhs.y *= rhs;
	return lhs;
}

Scale& operator*=(Scale& lhs, Scale const& rhs) {
	lhs.x *= rhs.x;
	lhs.y *= rhs.y;
	return lhs;
}

Scale& operator/=(Scale& lhs, float rhs) {
	lhs.x /= rhs;
	lhs.y /= rhs;
	return lhs;
}

Scale& operator/=(Scale& lhs, Scale const& rhs) {
	lhs.x /= rhs.x;
	lhs.y /= rhs.y;
	return lhs;
}
} // namespace kke
