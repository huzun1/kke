#pragma once

#include <d2d1.h>

#include <cstdint>

namespace kke {
struct Point2f {
	float x, y;

	D2D1_POINT_2F point2f() const;

	uint64_t hash() const;

	Point2f offset(float amount) const;

	Point2f offset(Point2f const& amount) const;

	Point2f operator *(const Point2f& param) const;
};
};	// namespace kke
